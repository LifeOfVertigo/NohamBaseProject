/*
 * Copyright (C) 2024 Michael Saller - All Rights Reserved
 * Published 2025 by TECHTILE media via FAB.com
 */
// NEONMessageHandler.cpp
#include "NEONMessageHandler.h"

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

#include "NEONLogging.h"
#include "UNEONWidget.h"
#include "NEONClient.h"

CefString GetErrorMessage(ENEONErrorCode ErrorCode)
{
  switch (ErrorCode)
  {
  case ENEONErrorCode::InvalidJson:
    return "Invalid JSON data";
  case ENEONErrorCode::MissingDelegateTypeField:
    return "Missing delegate type field";
  case ENEONErrorCode::InvalidDelegateType:
    return "Invalid delegate type";
  case ENEONErrorCode::MissingDelegateField:
    return "Missing delegate field";
  case ENEONErrorCode::MissingParametersField:
    return "Missing parameters field";
  case ENEONErrorCode::DelegateNotFound:
    return "Delegate not found";
  case ENEONErrorCode::UnsupportedPropertyType:
    return "Unsupported property type";
  case ENEONErrorCode::InvalidInput:
    return "Invalid input";
  case ENEONErrorCode::UnexpectedParameterType:
    return "Unexpected parameter type";
  case ENEONErrorCode::MissingParameter:
    return "Missing parameter";
  default:
    return "Unknown error";
  }
}

inline FString NEONMessageHandler::GetJsonTypeAsString(EJson Type)
{
  switch (Type)
  {
  case EJson::None:
    return TEXT("None");
  case EJson::Null:
    return TEXT("Null");
  case EJson::String:
    return TEXT("String");
  case EJson::Number:
    return TEXT("Number");
  case EJson::Boolean:
    return TEXT("Boolean");
  case EJson::Array:
    return TEXT("Array");
  case EJson::Object:
    return TEXT("Object");
  default:
    return TEXT("Unknown");
  }
}

bool NEONMessageHandler::OnQuery(CefRefPtr<CefBrowser> Browser,
                                 CefRefPtr<CefFrame> Frame,
                                 int64 QueryId,
                                 const CefString &Request,
                                 bool Persistent,
                                 CefRefPtr<Callback> Callback)
{
  FString requestUnrealString = Request.ToString().c_str();
  UE_LOG(LogNEONMessageHandler, Verbose, TEXT("NEONMessageHandler OnQuery: %s"), *requestUnrealString);

  // Parse the JSON string
  TSharedPtr<FJsonObject> jsonObject;
  TSharedRef<TJsonReader<>> reader = TJsonReaderFactory<>::Create(requestUnrealString);
  if (!FJsonSerializer::Deserialize(reader, jsonObject) || !jsonObject.IsValid())
  {
    UE_LOG(LogNEONMessageHandler, Error, TEXT("Failed to parse JSON data: %s"), *requestUnrealString);
    Callback->Failure(static_cast<int>(ENEONErrorCode::InvalidJson), GetErrorMessage(ENEONErrorCode::InvalidJson));
    return true;
  }

  // Check and assign "type", "delegate", "parameters"
  // - type
  if (!jsonObject->HasField(TEXT("type")) || jsonObject->GetStringField(TEXT("type")).IsEmpty())
  {
    UE_LOG(LogNEONMessageHandler, Error, TEXT("No delegate type field in JSON data"));
    Callback->Failure(static_cast<int>(ENEONErrorCode::MissingDelegateTypeField), GetErrorMessage(ENEONErrorCode::MissingDelegateTypeField));
    return true;
  }
  FString type = jsonObject->GetStringField(TEXT("type"));
  if (type != TEXT("function") && type != TEXT("event"))
  {
    UE_LOG(LogNEONMessageHandler, Error, TEXT("Invalid delegate type: %s"), *type);
    Callback->Failure(static_cast<int>(ENEONErrorCode::InvalidDelegateType), GetErrorMessage(ENEONErrorCode::InvalidDelegateType));
    return true;
  }

  // - delegate
  if (!jsonObject->HasField(TEXT("delegate")) || jsonObject->GetStringField(TEXT("delegate")).IsEmpty())
  {
    UE_LOG(LogNEONMessageHandler, Error, TEXT("No delegate field in JSON data"));
    Callback->Failure(static_cast<int>(ENEONErrorCode::MissingDelegateField), GetErrorMessage(ENEONErrorCode::MissingDelegateField));
    return true;
  }
  FString delegate = jsonObject->GetStringField(TEXT("delegate"));

  // - parameters
  if (!jsonObject->HasField(TEXT("parameters")) || !jsonObject->GetObjectField(TEXT("parameters")).IsValid())
  {
    UE_LOG(LogNEONMessageHandler, Error, TEXT("No parameters field in JSON data"));
    Callback->Failure(static_cast<int>(ENEONErrorCode::MissingParametersField), GetErrorMessage(ENEONErrorCode::MissingParametersField));
    return true;
  }

  if (type == TEXT("function"))
  {
    return InvokeFunction(delegate, jsonObject->GetObjectField(TEXT("parameters")), Callback);
  }
  else if (type == TEXT("event"))
  {
    return InvokeEvent(delegate, jsonObject->GetObjectField(TEXT("parameters")), Callback);
  }

  // This shouldn't happen as all delegate types are accounted for. Return false means query was not handled
  UE_LOG(LogNEONMessageHandler, Error, TEXT("Unhandled delegate type: %s"), *type);
  return false;
}

bool NEONMessageHandler::BuildParamsBuffer(UFunction *DelegateFunction, TSharedPtr<FJsonObject> JSON, uint8 *ParamsBuffer, CefRefPtr<Callback> Callback)
{
  FMemory::Memzero(ParamsBuffer, DelegateFunction->ParmsSize);

  for (TFieldIterator<FProperty> propertyIt(DelegateFunction); propertyIt; ++propertyIt)
  {
    FProperty *property = *propertyIt;

    // Skip properties that are not parameters & return value
    if (!property->HasAnyPropertyFlags(CPF_Parm))
      continue;

    // Only skip purely out parameters (no in-out params)
    if (property->HasAllPropertyFlags(CPF_OutParm) && !property->HasAnyPropertyFlags(CPF_ReferenceParm))
    {
      UE_LOG(LogNEONMessageHandler, Verbose, TEXT("Skipping property '%s' when building input params for delegate '%s'."), *property->GetName(), *DelegateFunction->GetName());
      continue;
    }
    UE_LOG(LogNEONMessageHandler, Verbose, TEXT("Processing input  property: %s"), *property->GetName());

    FString propertyName = property->GetName();

    // Check if the property exists in the JSON object
    if (!JSON->HasField(propertyName))
    {
      UE_LOG(LogNEONMessageHandler, Error, TEXT("Delegate function '%s' missing parameter in passed arguments: %s"), *DelegateFunction->GetName(), *propertyName);
      CefString paramName = *propertyName;

      // Concatenate error message
      CefString errorMessage = GetErrorMessage(ENEONErrorCode::MissingParameter).ToWString() + L": " + paramName.ToWString();

      // Call the Failure method with the correct arguments
      Callback->Failure(static_cast<int>(ENEONErrorCode::MissingParameter), errorMessage);
      continue;
    }

    // Assign and validate the property value
    uint8 *propertyAddress = property->ContainerPtrToValuePtr<uint8>(ParamsBuffer);
    if (!propertyAddress)
    {
      UE_LOG(LogNEONMessageHandler, Error, TEXT("BuildParamsBuffer: Invalid property pointer for %s"), *propertyName);
      return false;
    }

    TSharedPtr<FJsonValue> fieldValue = JSON->TryGetField(propertyName);
    if (!fieldValue.IsValid())
    {
      UE_LOG(LogNEONMessageHandler, Error, TEXT("BuildParamsBuffer: Field '%s' invalid."), *propertyName);
      return false;
    }

    // Set the property value based on its type
    // - boolean
    if (FBoolProperty *boolProperty = CastField<FBoolProperty>(property))
    {
      if (fieldValue->Type != EJson::Boolean)
      {
        UE_LOG(LogNEONMessageHandler, Warning, TEXT("Unexpected parameter type for %s. Expected boolean, got %s"), *propertyName, *GetJsonTypeAsString(fieldValue->Type));
      }

      bool value = fieldValue->AsBool();
      boolProperty->SetPropertyValue(propertyAddress, value);
      UE_LOG(LogNEONMessageHandler, Log, TEXT("Set input property: %s to %s"), *propertyName, value ? TEXT("true") : TEXT("false"));
    }
    // - numeric (float, double, int32, int64)
    else if (FNumericProperty *numericProperty = CastField<FNumericProperty>(property))
    {
      if (fieldValue->Type != EJson::Number)
      {
        UE_LOG(LogNEONMessageHandler, Warning, TEXT("Unexpected parameter type for %s. Expected number, got %s"), *propertyName, *GetJsonTypeAsString(fieldValue->Type));
      }

      double numberValue = fieldValue->AsNumber();
      if (numericProperty->IsFloatingPoint())
      {
        numericProperty->SetFloatingPointPropertyValue(propertyAddress, numberValue);
      }
      else if (numericProperty->IsInteger())
      {
        int64 intValue = static_cast<int64>(numberValue);
        numericProperty->SetIntPropertyValue(propertyAddress, intValue);
      }
      else
      {
        UE_LOG(LogNEONMessageHandler, Error, TEXT("Unsupported numeric property type: %s"), *propertyName);
        Callback->Failure(static_cast<int>(ENEONErrorCode::UnsupportedPropertyType), GetErrorMessage(ENEONErrorCode::UnsupportedPropertyType));
        return false;
      }
      UE_LOG(LogNEONMessageHandler, Verbose, TEXT("Set input property: %s to %f"), *propertyName, numberValue);
    }
    // - string
    else if (FStrProperty *stringProperty = CastField<FStrProperty>(property))
    {
      if (fieldValue->Type != EJson::String)
      {
        UE_LOG(LogNEONMessageHandler, Warning, TEXT("Unexpected parameter type for %s. Expected string, got %s"), *propertyName, *GetJsonTypeAsString(fieldValue->Type));
      }

      FString value = fieldValue->AsString();
      stringProperty->SetPropertyValue(propertyAddress, value);
      UE_LOG(LogNEONMessageHandler, Verbose, TEXT("Set input property: %s to %s"), *propertyName, *value);
    }
    // - JsonObjectWrapper
    else if (FStructProperty *structProperty = CastField<FStructProperty>(property))
    {
      if (structProperty->Struct->GetFName() != FName(TEXT("JsonObjectWrapper")))
      {
        UE_LOG(LogNEONMessageHandler, Error, TEXT("Unsupported struct property type: %s"), *propertyName);
        Callback->Failure(static_cast<int>(ENEONErrorCode::UnsupportedPropertyType), GetErrorMessage(ENEONErrorCode::UnsupportedPropertyType));
        return false;
      }

      uint8 *propAddr = property->ContainerPtrToValuePtr<uint8>(ParamsBuffer);
      if (!propAddr)
      {
        UE_LOG(LogNEONMessageHandler, Error, TEXT("BuildParamsBuffer: Invalid property pointer for %s"), *propertyName);
        return false;
      }

      if (fieldValue->Type != EJson::Object)
      {
        UE_LOG(LogNEONMessageHandler, Error, TEXT("Unexpected parameter type for %s"), *propertyName);
        Callback->Failure(static_cast<int>(ENEONErrorCode::UnexpectedParameterType), GetErrorMessage(ENEONErrorCode::UnexpectedParameterType));
        return false;
      }
      TSharedPtr<FJsonObject> inputObject = fieldValue->AsObject();
      if (!inputObject.IsValid())
      {
        UE_LOG(LogNEONMessageHandler, Error, TEXT("Invalid JsonObjectWrapper input for property: %s"), *propertyName);
        Callback->Failure(static_cast<int>(ENEONErrorCode::InvalidInput), GetErrorMessage(ENEONErrorCode::InvalidInput));
        return false;
      }

      FString subJsonString;
      {
        TSharedRef<TJsonWriter<>> writer = TJsonWriterFactory<>::Create(&subJsonString);
        FJsonSerializer::Serialize(inputObject.ToSharedRef(), writer);
      }

      FJsonObjectWrapper &wrapperRef = *reinterpret_cast<FJsonObjectWrapper *>(propAddr);
      wrapperRef.JsonString = subJsonString;

      TSharedPtr<FJsonObject> duplicatedObject = MakeShared<FJsonObject>(*inputObject);
      wrapperRef.JsonObject = duplicatedObject;

      UE_LOG(LogNEONMessageHandler, Verbose, TEXT("Set input property: %s to JsonObjectWrapper(%s)"), *propertyName, *subJsonString);
    }
    else if (FArrayProperty *arrayProperty = CastField<FArrayProperty>(property))
    {
      if (fieldValue->Type != EJson::Array)
      {
        UE_LOG(LogNEONMessageHandler, Warning, TEXT("Unexpected parameter type for %s. Expected array, got %s"), *propertyName, *GetJsonTypeAsString(fieldValue->Type));
      }

      TArray<TSharedPtr<FJsonValue>> jsonArray = fieldValue->AsArray();
      FScriptArrayHelper arrayHelper(arrayProperty, propertyAddress);
      int32 numElements = jsonArray.Num();
      arrayHelper.Resize(numElements);

      FProperty *innerProperty = arrayProperty->Inner;
      for (int32 i = 0; i < numElements; ++i)
      {
        void *elementAddr = arrayHelper.GetRawPtr(i);
        TSharedPtr<FJsonValue> jsonElement = jsonArray[i];

        if (FBoolProperty *boolInner = CastField<FBoolProperty>(innerProperty))
        {
          if (jsonElement->Type != EJson::Boolean)
          {
            UE_LOG(LogNEONMessageHandler, Warning, TEXT("Unexpected array element type for %s. Expected boolean, got %s"), *propertyName, *GetJsonTypeAsString(jsonElement->Type));
          }
          bool value = jsonElement->AsBool();
          boolInner->SetPropertyValue(elementAddr, value);
        }
        else if (FNumericProperty *numericInner = CastField<FNumericProperty>(innerProperty))
        {
          if (jsonElement->Type != EJson::Number)
          {
            UE_LOG(LogNEONMessageHandler, Warning, TEXT("Unexpected array element type for %s. Expected number, got %s"), *propertyName, *GetJsonTypeAsString(jsonElement->Type));
          }
          double numberValue = jsonElement->AsNumber();
          if (numericInner->IsFloatingPoint())
          {
            numericInner->SetFloatingPointPropertyValue(elementAddr, numberValue);
          }
          else if (numericInner->IsInteger())
          {
            int64 intValue = static_cast<int64>(numberValue);
            numericInner->SetIntPropertyValue(elementAddr, intValue);
          }
          else
          {
            UE_LOG(LogNEONMessageHandler, Error, TEXT("Unsupported numeric type in array for property: %s"), *propertyName);
            Callback->Failure(static_cast<int>(ENEONErrorCode::UnsupportedPropertyType), GetErrorMessage(ENEONErrorCode::UnsupportedPropertyType));
            return false;
          }
        }
        else if (FStrProperty *stringInner = CastField<FStrProperty>(innerProperty))
        {
          if (jsonElement->Type != EJson::String)
          {
            UE_LOG(LogNEONMessageHandler, Warning, TEXT("Unexpected array element type for %s. Expected string, got %s"), *propertyName, *GetJsonTypeAsString(jsonElement->Type));
          }
          FString value = jsonElement->AsString();
          stringInner->SetPropertyValue(elementAddr, value);
        }
        else if (FStructProperty *structInner = CastField<FStructProperty>(innerProperty))
        {
          if (structInner->Struct->GetFName() != FName(TEXT("JsonObjectWrapper")))
          {
            UE_LOG(LogNEONMessageHandler, Error, TEXT("Unsupported struct type in array for property: %s"), *propertyName);
            Callback->Failure(static_cast<int>(ENEONErrorCode::UnsupportedPropertyType), GetErrorMessage(ENEONErrorCode::UnsupportedPropertyType));
            return false;
          }
          if (jsonElement->Type != EJson::Object)
          {
            UE_LOG(LogNEONMessageHandler, Error, TEXT("Unexpected array element type for %s. Expected object for JsonObjectWrapper"), *propertyName);
            Callback->Failure(static_cast<int>(ENEONErrorCode::UnexpectedParameterType), GetErrorMessage(ENEONErrorCode::UnexpectedParameterType));
            return false;
          }
          TSharedPtr<FJsonObject> inputObject = jsonElement->AsObject();
          if (!inputObject.IsValid())
          {
            UE_LOG(LogNEONMessageHandler, Error, TEXT("Invalid JsonObjectWrapper in array for property: %s"), *propertyName);
            Callback->Failure(static_cast<int>(ENEONErrorCode::InvalidInput), GetErrorMessage(ENEONErrorCode::InvalidInput));
            return false;
          }
          FString subJsonString;
          {
            TSharedRef<TJsonWriter<>> writer = TJsonWriterFactory<>::Create(&subJsonString);
            FJsonSerializer::Serialize(inputObject.ToSharedRef(), writer);
          }
          FJsonObjectWrapper &wrapperRef = *reinterpret_cast<FJsonObjectWrapper *>(elementAddr);
          wrapperRef.JsonString = subJsonString;
          TSharedPtr<FJsonObject> duplicatedObject = MakeShared<FJsonObject>(*inputObject);
          wrapperRef.JsonObject = duplicatedObject;
        }
        else
        {
          UE_LOG(LogNEONMessageHandler, Error, TEXT("Unsupported array element type for property: %s"), *propertyName);
          Callback->Failure(static_cast<int>(ENEONErrorCode::UnsupportedPropertyType), GetErrorMessage(ENEONErrorCode::UnsupportedPropertyType));
          return false;
        }
      }
      UE_LOG(LogNEONMessageHandler, Verbose, TEXT("Set input property: %s array with %d elements"), *propertyName, numElements);
    }

    else
    {
      UE_LOG(LogNEONMessageHandler, Error, TEXT("Unsupported property type: %s"), *propertyName);
      Callback->Failure(static_cast<int>(ENEONErrorCode::UnsupportedPropertyType), GetErrorMessage(ENEONErrorCode::UnsupportedPropertyType));
      return true;
    }
  }

  return true;
}

bool NEONMessageHandler::InvokeFunction(FString Name, TSharedPtr<FJsonObject> JSON, CefRefPtr<Callback> Callback)
{
  // Find the function by name
  UFunction *delegateFunction = _Widget->FindFunction(*Name);
  if (!delegateFunction)
  {
    UE_LOG(LogNEONMessageHandler, Error, TEXT("Function not found: %s"), *Name);
    Callback->Failure(static_cast<int>(ENEONErrorCode::DelegateNotFound), GetErrorMessage(ENEONErrorCode::DelegateNotFound));
    return true;
  }

  // Prepare the parameters buffer and construct frame
  uint8 *paramsBuffer = (uint8 *)FMemory_Alloca(delegateFunction->ParmsSize);
  if (!BuildParamsBuffer(delegateFunction, JSON, paramsBuffer, Callback))
  {
    return true;
  }

  // UE_LOG(LogNEONMessageHandler, Verbose, TEXT("Parameters assembled, invoking function %s"), *Name);
  _Widget->ProcessEvent(delegateFunction, paramsBuffer);

  TSharedPtr<FJsonObject> jsonOut = MakeShared<FJsonObject>();
  for (TFieldIterator<FProperty> iteratedProperty(delegateFunction); iteratedProperty; ++iteratedProperty)
  {
    FProperty *property = *iteratedProperty;

    if (!property->HasAnyPropertyFlags(CPF_Parm))
      continue;

    if (!property->HasAllPropertyFlags(CPF_OutParm) || property->HasAnyPropertyFlags(CPF_ReferenceParm))
    {
      UE_LOG(LogNEONMessageHandler, Verbose, TEXT("Skipping property '%s' when building output params for delegate '%s'"), *property->GetName(), *delegateFunction->GetName());
      continue;
    }

    const FString propertyName = property->GetNameCPP();
    UE_LOG(LogNEONMessageHandler, Verbose, TEXT("Processing output property: %s"), *propertyName);

    void *propertyValue = property->ContainerPtrToValuePtr<void>(paramsBuffer);

    if (FBoolProperty *boolProperty = CastField<FBoolProperty>(property))
    {
      bool value = boolProperty->GetPropertyValue(propertyValue);
      jsonOut->SetBoolField(propertyName, value);
      UE_LOG(LogNEONMessageHandler, Verbose, TEXT("Set output property: %s to %s"), *propertyName, value ? TEXT("true") : TEXT("false"));
    }
    else if (FNumericProperty *numericProperty = CastField<FNumericProperty>(property))
    {
      if (numericProperty->IsFloatingPoint())
      {
        // Floating-point property (e.g., float, double)
        double value = numericProperty->GetFloatingPointPropertyValue(propertyValue);
        jsonOut->SetNumberField(propertyName, value);
        UE_LOG(LogNEONMessageHandler, Verbose, TEXT("Set output property: %s to float/double: %f"), *propertyName, value);
      }
      else if (numericProperty->IsInteger())
      {
        // Integer property (e.g., int32, int64)
        int64 value = static_cast<int64>(numericProperty->GetSignedIntPropertyValue(propertyValue));
        jsonOut->SetNumberField(propertyName, value);
        UE_LOG(LogNEONMessageHandler, Verbose, TEXT("Set output property: %s to integer: %lld"), *propertyName, value);
      }
      else
      {
        UE_LOG(LogNEONMessageHandler, Error, TEXT("Unsupported numeric property type: %s"), *propertyName);
        Callback->Failure(static_cast<int>(ENEONErrorCode::UnsupportedPropertyType), GetErrorMessage(ENEONErrorCode::UnsupportedPropertyType));
        return true;
      }
    }
    else if (FStrProperty *stringProperty = CastField<FStrProperty>(property))
    {
      FString value = stringProperty->GetPropertyValue(propertyValue);
      jsonOut->SetStringField(propertyName, value);
      UE_LOG(LogNEONMessageHandler, Verbose, TEXT("Set output property: %s to %s"), *propertyName, *value);
    }
    else if (FStructProperty *structProperty = CastField<FStructProperty>(property))
    {
      if (structProperty->Struct->GetFName() != "JsonObjectWrapper")
      {
        UE_LOG(LogNEONMessageHandler, Error, TEXT("Unsupported output property type: %s"), *propertyName);
        Callback->Failure(static_cast<int>(ENEONErrorCode::UnsupportedPropertyType), GetErrorMessage(ENEONErrorCode::UnsupportedPropertyType));
        return true;
      }

      // Extract the JsonObjectWrapper
      FJsonObjectWrapper *jsonObjectWrapper = reinterpret_cast<FJsonObjectWrapper *>(propertyValue);
      TSharedPtr<FJsonObject> outputObject = jsonObjectWrapper->JsonObject;
      if (!outputObject.IsValid())
      {
        UE_LOG(LogNEONMessageHandler, Error, TEXT("Invalid JsonObjectWrapper output for property: %s"), *propertyName);
        Callback->Failure(static_cast<int>(ENEONErrorCode::InvalidInput), GetErrorMessage(ENEONErrorCode::InvalidInput));
        return true;
      }

      jsonOut->SetObjectField(propertyName, outputObject);
      UE_LOG(LogNEONMessageHandler, Verbose, TEXT("Set output property: %s to JsonObjectWrapper(%s)"), *propertyName, *jsonObjectWrapper->JsonString);
    }
    else if (FArrayProperty *arrayProperty = CastField<FArrayProperty>(property))
    {
      FScriptArrayHelper arrayHelper(arrayProperty, propertyValue);
      TArray<TSharedPtr<FJsonValue>> jsonArray;

      for (int32 i = 0; i < arrayHelper.Num(); ++i)
      {
        void *elementAddr = arrayHelper.GetRawPtr(i);
        FProperty *innerProperty = arrayProperty->Inner;

        if (FBoolProperty *boolInner = CastField<FBoolProperty>(innerProperty))
        {
          bool value = boolInner->GetPropertyValue(elementAddr);
          jsonArray.Add(MakeShared<FJsonValueBoolean>(value));
        }
        else if (FNumericProperty *numericInner = CastField<FNumericProperty>(innerProperty))
        {
          if (numericInner->IsFloatingPoint())
          {
            double value = numericInner->GetFloatingPointPropertyValue(elementAddr);
            jsonArray.Add(MakeShared<FJsonValueNumber>(value));
          }
          else if (numericInner->IsInteger())
          {
            int64 value = numericInner->GetSignedIntPropertyValue(elementAddr);
            jsonArray.Add(MakeShared<FJsonValueNumber>(value));
          }
        }
        else if (FStrProperty *stringInner = CastField<FStrProperty>(innerProperty))
        {
          FString value = stringInner->GetPropertyValue(elementAddr);
          jsonArray.Add(MakeShared<FJsonValueString>(value));
        }
        else if (FStructProperty *structInner = CastField<FStructProperty>(innerProperty))
        {
          if (structInner->Struct->GetFName() != FName(TEXT("JsonObjectWrapper")))
          {
            UE_LOG(LogNEONMessageHandler, Error, TEXT("Unsupported struct type in array for property: %s"), *propertyName);
            Callback->Failure(static_cast<int>(ENEONErrorCode::UnsupportedPropertyType),
                              GetErrorMessage(ENEONErrorCode::UnsupportedPropertyType));
            return true;
          }
          FJsonObjectWrapper *jsonObjectWrapper = reinterpret_cast<FJsonObjectWrapper *>(elementAddr);
          if (!jsonObjectWrapper->JsonObject.IsValid())
          {
            UE_LOG(LogNEONMessageHandler, Error, TEXT("Invalid JsonObjectWrapper output for array element in property: %s"), *propertyName);
            Callback->Failure(static_cast<int>(ENEONErrorCode::InvalidInput),
                              GetErrorMessage(ENEONErrorCode::InvalidInput));
            return true;
          }
          jsonArray.Add(MakeShared<FJsonValueObject>(jsonObjectWrapper->JsonObject));
        }
        else
        {
          UE_LOG(LogNEONMessageHandler, Error, TEXT("Unsupported array element type for property: %s"), *propertyName);
          Callback->Failure(static_cast<int>(ENEONErrorCode::UnsupportedPropertyType),
                            GetErrorMessage(ENEONErrorCode::UnsupportedPropertyType));
          return true;
        }
      }
      jsonOut->SetArrayField(propertyName, jsonArray);
    }

    else
    {
      UE_LOG(LogNEONMessageHandler, Error, TEXT("Unsupported output property type: %s"), *propertyName);
      Callback->Failure(static_cast<int>(ENEONErrorCode::UnsupportedPropertyType), GetErrorMessage(ENEONErrorCode::UnsupportedPropertyType));
      return true;
    }
  }

  // Serialize the JSON object and log it
  FString outputString;
  TSharedRef<TJsonWriter<>> writer = TJsonWriterFactory<>::Create(&outputString);
  FJsonSerializer::Serialize(jsonOut.ToSharedRef(), writer);

  UE_LOG(LogNEONMessageHandler, Verbose, TEXT("Function result: %s"), *outputString);

  Callback->Success(*outputString);
  return true;
}

bool NEONMessageHandler::InvokeEvent(FString Name, TSharedPtr<FJsonObject> JSON, CefRefPtr<Callback> Callback)
{
  // Find the function by name
  UFunction *delegateFunction = _Widget->FindFunction(*Name);
  if (!delegateFunction)
  {
    UE_LOG(LogNEONMessageHandler, Error, TEXT("Event not found: %s"), *Name);
    Callback->Failure(static_cast<int>(ENEONErrorCode::DelegateNotFound), GetErrorMessage(ENEONErrorCode::DelegateNotFound));
    return true;
  }

  // Prepare the parameters buffer and construct frame
  uint8 *paramsBuffer = (uint8 *)FMemory_Alloca(delegateFunction->ParmsSize);
  if (!BuildParamsBuffer(delegateFunction, JSON, paramsBuffer, Callback))
  {
    return true;
  }

  UE_LOG(LogNEONMessageHandler, Verbose, TEXT("Parameters assembled, invoking event %s"), *Name);
  _Widget->ProcessEvent(delegateFunction, paramsBuffer);

  // Events don't return values, so we just send an empty success response
  Callback->Success("");
  return true;
}
