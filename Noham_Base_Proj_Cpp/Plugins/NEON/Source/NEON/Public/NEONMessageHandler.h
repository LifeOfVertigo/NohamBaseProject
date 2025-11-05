/*
 * Copyright (C) 2024 Michael Saller - All Rights Reserved
 * Published 2025 by TECHTILE media via FAB.com
 */
// NEONMessageHandler.h
#pragma once

#include "JsonObjectWrapper.h"
#include "Dom/JsonValue.h"

#include "Windows/AllowWindowsPlatformTypes.h"
THIRD_PARTY_INCLUDES_START
#include "include/cef_client.h"
#include "include/wrapper/cef_helpers.h"
#include "include/wrapper/cef_message_router.h"
THIRD_PARTY_INCLUDES_END
#include "Windows/HideWindowsPlatformTypes.h"

// FORWARD DECLARATIONS
class UNEONWidget;
class FJsonObject;

// Enum for error codes
enum class ENEONErrorCode : uint8
{
  Unknown = 0,
  InvalidJson = 1,
  MissingDelegateTypeField = 2,
  InvalidDelegateType = 3,
  MissingDelegateField = 4,
  MissingParametersField = 5,
  DelegateNotFound = 6,
  UnsupportedPropertyType = 7,
  InvalidInput = 8,
  UnexpectedParameterType = 9,
  MissingParameter = 10
};
CefString GetErrorMessage(ENEONErrorCode ErrorCode);

class NEONMessageHandler : public CefMessageRouterBrowserSide::Handler
{
public:
  NEONMessageHandler(UNEONWidget *Widget) : _Widget(Widget) {}
  ~NEONMessageHandler()
  {
    _Widget = nullptr;
  }

  bool OnQuery(CefRefPtr<CefBrowser> Browser,
               CefRefPtr<CefFrame> Frame,
               int64 QueryId,
               const CefString &Request,
               bool Persistent,
               CefRefPtr<Callback> Callback) override;

  bool InvokeFunction(FString Name, TSharedPtr<FJsonObject> JSON, CefRefPtr<Callback> Callback);
  bool InvokeEvent(FString Name, TSharedPtr<FJsonObject> JSON, CefRefPtr<Callback> Callback);

protected:
  bool BuildParamsBuffer(UFunction *DelegateFunction, TSharedPtr<FJsonObject> JSONIn, uint8 *ParamsBuffer, CefRefPtr<Callback> Callback);
  FString GetJsonTypeAsString(EJson Type);

  UNEONWidget *_Widget;
};