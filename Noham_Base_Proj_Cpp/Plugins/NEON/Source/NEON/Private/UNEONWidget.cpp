/*
 * Copyright (C) 2024 Michael Saller - All Rights Reserved
 * Published 2025 by TECHTILE media via FAB.com
 */
// NEONWidget.cpp

#include "UNEONWidget.h"
#include "RenderGraphBuilder.h"
#include "RenderGraphUtils.h"
#include "Async/Async.h"
#include "TextureResource.h"
#include "Engine/GameViewportClient.h"
#include "Blueprint/WidgetLayoutLibrary.h"

#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"

#include <chrono>

#include "NEON.h"
#include "NEONMessageHandler.h"

#include "NEONView_11.h"
#include "NEONView_12.h"
#include "NEONLogging.h"

using Microsoft::WRL::ComPtr;

void UNEONWidget::NativeConstruct()
{
  Super::NativeConstruct();

  // Set up widget visibilities
  SetVisibility(ESlateVisibility::Visible);
  if (_BrowserImage)
  {
    _BrowserImage->SetVisibility(ESlateVisibility::Visible);
  }

  // Set up the FPS timer
  GetWorld()->GetTimerManager().SetTimer(
      _FPSTimerHandle,
      [this]()
      {
        _FPS = _FPSTransient;
        _FPSTransient = 0;
      },
      1.0f,
      true
      //
  );

  // Attempt to create the appropriate NEONView
  ERHIInterfaceType currentRHI = RHIGetInterfaceType();
  bool bCreatedView = false;
  if (currentRHI == ERHIInterfaceType::D3D12)
  {
    NEONView_12 *newView = new NEONView_12();
    if (newView->InitializeView(this))
    {
      _View = newView;
      bCreatedView = true;
    }
    else
    {
      delete newView;
    }
  }
  if (!bCreatedView && currentRHI == ERHIInterfaceType::D3D11)
  {
    NEONView_11 *newView = new NEONView_11();
    if (newView->InitializeView(this))
    {
      _View = newView;
      bCreatedView = true;
    }
    else
    {
      delete newView;
    }
  }

  if (!bCreatedView)
  {
    UE_LOG(LogNEONWidget, Fatal, TEXT("Cannot use D3D12 or D3D11. No valid NEONView created."));
    return;
  }

  FNEONModule &NEONModule = FModuleManager::GetModuleChecked<FNEONModule>("NEON");

  // Create the render handler and client
  UE_LOG(LogNEONWidget, Log, TEXT("Creating Message Handler"));
  NEONMessageHandler *messageHandler = new NEONMessageHandler(this);
  UE_LOG(LogNEONWidget, Log, TEXT("Creating NEONClient"));
  _Client = new NEONClient(messageHandler);

  CreateBrowser();
}

void UNEONWidget::SetMaxFPS(int MaxFPS)
{
  _MaxFPS = MaxFPS;
  if (_Browser && _Browser->GetHost())
    _Browser->GetHost()->SetWindowlessFrameRate(_MaxFPS);
  else
    UE_LOG(LogNEONWidget, Error, TEXT("Browser or host is null. Cannot set max FPS."));
}

void UNEONWidget::SetProcessingTime(float ProcessingTime)
{
  // Get module, set processing time
  FNEONModule &NEONModule = FModuleManager::GetModuleChecked<FNEONModule>("NEON");
  NEONModule.SetProcessingTime(ProcessingTime);
}

int UNEONWidget::GetFPS()
{
  return _FPS;
}

void UNEONWidget::CreateBrowser()
{
  // Create the browser
  UE_LOG(LogNEONWidget, Log, TEXT("Creating browser"));
  CefWindowInfo windowInfo;
  windowInfo.SetAsWindowless(nullptr);
  windowInfo.windowless_rendering_enabled = true;
  windowInfo.shared_texture_enabled = true;
  if (_ExternalBeginFrame)
    windowInfo.external_begin_frame_enabled = true;

  CefBrowserSettings browserSettings;
  browserSettings.background_color = CefColorSetARGB(0, 0, 0, 0);

  CefString url;

  // Load development URL in editor, live URL in game
  FText environmentURL;
#if WITH_EDITOR
  environmentURL = _DevelopmentURL;
#else
  environmentURL = _LiveURL;
#endif

  UE_LOG(LogNEONWidget, Log, TEXT("Development URL: %s (%d), Live URL: %s (%d)"), *_DevelopmentURL.ToString(), _DevelopmentURL.IsEmpty(), *_LiveURL.ToString(), _LiveURL.IsEmpty());

  if (environmentURL.IsEmpty())
  {
    UE_LOG(LogNEONWidget, Warning, TEXT("URL is empty."));
    environmentURL = FText::FromString(TEXT("/index.html"));
  }

  if (environmentURL.ToString().StartsWith("http"))
  {
    url = CefString(TCHAR_TO_UTF8(*environmentURL.ToString()));
  }
  else
  {
    if (!environmentURL.ToString().StartsWith("/"))
    {
      UE_LOG(LogNEONWidget, Fatal, TEXT("URL is not valid. Either supply http(s):// or /path/to/file. This file path will be relative to <project directory>/NEON."));
      return;
    }

    FString projectDir = FPaths::ProjectDir();
    projectDir = FPaths::ConvertRelativePathToFull(projectDir).Replace(TEXT("\\"), TEXT("/"));
    FString fileURL = FString::Printf(TEXT("file:///%sNEON%s"), *projectDir, *environmentURL.ToString());

    UE_LOG(LogNEONWidget, Log, TEXT("Trying to open NEON with file URL: %s"), *fileURL);
    url = CefString(TCHAR_TO_UTF8(*fileURL));
  }

  _Browser = CefBrowserHost::CreateBrowserSync(windowInfo, _Client, url, browserSettings, nullptr, nullptr);

  if (!_Browser)
  {
    UE_LOG(LogNEONWidget, Fatal, TEXT("Failed to create browser."));
    return;
  }
  UE_LOG(LogNEONWidget, Log, TEXT("Browser created."));

#if WITH_EDITOR
  if (_OpenDevTools)
  {
    UE_LOG(LogNEONWidget, Log, TEXT("Opening dev tools."));
    _Browser->GetHost()->ShowDevTools(CefWindowInfo(), _Client, CefBrowserSettings(), CefPoint());
  }
#endif

  _Client->SetWidget(this);
  _Browser->GetHost()->SetWindowlessFrameRate(_MaxFPS);

  OnBrowserCreated();
}

void UNEONWidget::NativeTick(const FGeometry &MyGeometry, float InDeltaTime)
{
  Super::NativeTick(MyGeometry, InDeltaTime);

  if (!_Browser || !_View)
  {
    return;
  }

  _ScaleFactor = UWidgetLayoutLibrary::GetViewportScale(GEngine->GameViewport->GetWorld());
  _View->SetWidgetSize(MyGeometry.GetLocalSize() * _ScaleFactor);

  if (_ExternalBeginFrame)
    _Browser->GetHost()->SendExternalBeginFrame();
}

void UNEONWidget::NativeDestruct()
{
  Super::NativeDestruct();

  UE_LOG(LogNEONWidget, Log, TEXT("Destructing NEON Widget"));

  if (_View)
  {
    _View->DestroyView();
    delete _View;
    _View = nullptr;
  }

  if (_Browser)
  {
#if WITH_EDITOR
    UE_LOG(LogNEONWidget, Log, TEXT("Closing DevTools."));
    if (_OpenDevTools)
    {
      _Browser->GetHost()->CloseDevTools();
    }
#endif

    UE_LOG(LogNEONWidget, Log, TEXT("Closing browser."));
    _Browser->GetHost()->CloseBrowser(true);
    _Browser = nullptr;
  }
  if (_Client)
  {
    UE_LOG(LogNEONWidget, Log, TEXT("Stopping client."));
    _Client->Stop();
    _Client = nullptr;
  }

  GetWorld()->GetTimerManager().ClearTimer(_FPSTimerHandle);
}

void UNEONWidget::RestartBrowser()
{
  if (_Browser)
  {
    _Browser->GetHost()->CloseBrowser(true);
    _Browser = nullptr;
  }

  CreateBrowser();
}

void UNEONWidget::OnAcceleratedPaint_Widget(HANDLE SharedHandle)
{
  if (!_View)
  {
    UE_LOG(LogNEONWidget, Error, TEXT("No NEONView available to handle OnAcceleratedPaint_Widget"));
    return;
  }

  // Increment transient FPS each time we receive a main texture update.
  _FPSTransient++;

  _View->OnAcceleratedPaint_View(SharedHandle);
}

void UNEONWidget::OnAcceleratedPaint_Widget_Popup(HANDLE SharedHandle)
{
  if (!_View)
  {
    UE_LOG(LogNEONWidget, Error, TEXT("No NEONView available to handle OnAcceleratedPaint_Widget_Popup"));
    return;
  }
  _View->OnAcceleratedPaint_View_Popup(SharedHandle);
}

void UNEONWidget::InvalidateBrowser()
{
  if (!_Browser)
  {
    return;
  }

  _Browser->GetHost()->Invalidate(PET_VIEW);
}

void UNEONWidget::InvokeWebNoParam(const FString &Method)
{
  if (!_Browser)
  {
    UE_LOG(LogNEONWidget, Error, TEXT("Tried to invoke web when browser is null."));
    return;
  }

  FString Script = FString::Printf(TEXT("NEON_Bridge_Web_Invoke(\"%s\");"), *Method);

  const CefString &CefScript = CefString(TCHAR_TO_UTF8(*Script));
  _Browser->GetMainFrame()->ExecuteJavaScript(CefScript, _Browser->GetMainFrame()->GetURL(), 0);
}

void UNEONWidget::InvokeWeb(const FString &Method, const FJsonObjectWrapper &JsonObjectWrapper)
{
  if (!_Browser)
  {
    UE_LOG(LogNEONWidget, Error, TEXT("Tried to invoke web when browser is null."));
    return;
  }

  if (!JsonObjectWrapper.JsonObject.IsValid())
  {
    UE_LOG(LogNEONWidget, Error, TEXT("Invalid JSON object."));
    return;
  }

  // Serialize the JSON object
  FString JSONData;
  TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JSONData);
  FJsonSerializer::Serialize(JsonObjectWrapper.JsonObject.ToSharedRef(), Writer);

  // Properly escape JSON data for JavaScript
  FString EscapedJson = JSONData.Replace(TEXT("\\"), TEXT("\\\\"))
                            .Replace(TEXT("\""), TEXT("\\\""))
                            .Replace(TEXT("\n"), TEXT("\\n"))
                            .Replace(TEXT("\r"), TEXT("\\r"))
                            .Replace(TEXT("\t"), TEXT("\\t"));

  FString Script = FString::Printf(TEXT("NEON_Bridge_Web_Invoke(\"%s\", \"%s\");"), *Method, *EscapedJson);

  const CefString &CefScript = CefString(TCHAR_TO_UTF8(*Script));
  _Browser->GetMainFrame()->ExecuteJavaScript(CefScript, _Browser->GetMainFrame()->GetURL(), 0);
}

void UNEONWidget::InvokeWebBoolean(const FString &Method, bool Value)
{
  if (!_Browser)
  {
    UE_LOG(LogNEONWidget, Error, TEXT("Tried to invoke web when browser is null."));
    return;
  }

  FString Script = FString::Printf(TEXT("NEON_Bridge_Web_Invoke(\"%s\", %s);"), *Method, Value ? TEXT("true") : TEXT("false"));

  const CefString &CefScript = CefString(TCHAR_TO_UTF8(*Script));
  _Browser->GetMainFrame()->ExecuteJavaScript(CefScript, _Browser->GetMainFrame()->GetURL(), 0);
}
void UNEONWidget::InvokeWebInteger(const FString &Method, int32 Value)
{
  if (!_Browser)
  {
    UE_LOG(LogNEONWidget, Error, TEXT("Tried to invoke web when browser is null."));
    return;
  }

  FString Script = FString::Printf(TEXT("NEON_Bridge_Web_Invoke(\"%s\", %d);"), *Method, Value);

  const CefString &CefScript = CefString(TCHAR_TO_UTF8(*Script));
  _Browser->GetMainFrame()->ExecuteJavaScript(CefScript, _Browser->GetMainFrame()->GetURL(), 0);
}
void UNEONWidget::InvokeWebFloat(const FString &Method, float Value)
{
  if (!_Browser)
  {
    UE_LOG(LogNEONWidget, Error, TEXT("Tried to invoke web when browser is null."));
    return;
  }

  FString Script = FString::Printf(TEXT("NEON_Bridge_Web_Invoke(\"%s\", %f);"), *Method, Value);

  const CefString &CefScript = CefString(TCHAR_TO_UTF8(*Script));
  _Browser->GetMainFrame()->ExecuteJavaScript(CefScript, _Browser->GetMainFrame()->GetURL(), 0);
}
void UNEONWidget::InvokeWebString(const FString &Method, const FString &Value)
{
  if (!_Browser)
  {
    UE_LOG(LogNEONWidget, Error, TEXT("Tried to invoke web when browser is null."));
    return;
  }

  // Properly escape the string for JavaScript
  FString EscapedValue = Value.Replace(TEXT("\\"), TEXT("\\\\"))
                             .Replace(TEXT("\""), TEXT("\\\""))
                             .Replace(TEXT("\n"), TEXT("\\n"))
                             .Replace(TEXT("\r"), TEXT("\\r"))
                             .Replace(TEXT("\t"), TEXT("\\t"));

  FString Script = FString::Printf(TEXT("NEON_Bridge_Web_Invoke(\"%s\", \"%s\");"), *Method, *EscapedValue);

  const CefString &CefScript = CefString(TCHAR_TO_UTF8(*Script));
  _Browser->GetMainFrame()->ExecuteJavaScript(CefScript, _Browser->GetMainFrame()->GetURL(), 0);
}

FReply UNEONWidget::NativeOnMouseButtonDown(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent)
{
  UE_LOG(LogNEONWidget, Warning, TEXT("[CLICK DEBUG] NativeOnMouseButtonDown - Button: %s"), *MouseEvent.GetEffectingButton().ToString());
  HandleMouseButtonEvent(MyGeometry, MouseEvent, false);
  UE_LOG(LogNEONWidget, Warning, TEXT("[CLICK DEBUG] NativeOnMouseButtonDown Reply - IsHandled: TRUE (forced)"));
  return FReply::Handled();
}

FReply UNEONWidget::NativeOnMouseButtonUp(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent)
{
  UE_LOG(LogNEONWidget, Warning, TEXT("[CLICK DEBUG] NativeOnMouseButtonUp - Button: %s"), *MouseEvent.GetEffectingButton().ToString());
  HandleMouseButtonEvent(MyGeometry, MouseEvent, true);
  UE_LOG(LogNEONWidget, Warning, TEXT("[CLICK DEBUG] NativeOnMouseButtonUp Reply - IsHandled: TRUE (forced)"));
  return FReply::Handled();
}

FReply UNEONWidget::NativeOnMouseButtonDoubleClick(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent)
{
  UE_LOG(LogNEONWidget, Warning, TEXT("[CLICK DEBUG] NativeOnMouseButtonDoubleClick - Button: %s"), *MouseEvent.GetEffectingButton().ToString());
  HandleMouseButtonEvent(MyGeometry, MouseEvent, false);
  UE_LOG(LogNEONWidget, Warning, TEXT("[CLICK DEBUG] NativeOnMouseButtonDoubleClick Reply - IsHandled: TRUE (forced)"));
  return FReply::Handled();
}

FReply UNEONWidget::NativeOnMouseMove(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent)
{
  HandleMouseMoveEvent(MyGeometry, MouseEvent);
  return FReply::Handled();
}

FReply UNEONWidget::NativeOnMouseWheel(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent)
{
  HandleMouseWheelEvent(MyGeometry, MouseEvent);
  return FReply::Handled();
}

FReply UNEONWidget::NativeOnKeyDown(const FGeometry &MyGeometry, const FKeyEvent &InKeyEvent)
{
  HandleKeyEvent(InKeyEvent, false);
  return FReply::Handled();
}

FReply UNEONWidget::NativeOnKeyUp(const FGeometry &MyGeometry, const FKeyEvent &InKeyEvent)
{
  HandleKeyEvent(InKeyEvent, true);
  return FReply::Handled();
}

FReply UNEONWidget::NativeOnKeyChar(const FGeometry &MyGeometry, const FCharacterEvent &InCharacterEvent)
{
  HandleCharacterEvent(InCharacterEvent);
  return FReply::Handled();
}

void UNEONWidget::HandleMouseButtonEvent(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent, bool bIsMouseUp)
{
  if (!_Browser)
  {
    UE_LOG(LogNEONWidget, Error, TEXT("[CLICK DEBUG] HandleMouseButtonEvent - Browser is NULL!"));
    return;
  }

  CefMouseEvent cefEvent = GetCefMouseEvent(MyGeometry, MouseEvent);
  CefBrowserHost::MouseButtonType buttonType;
  FString buttonName;

  if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
  {
    buttonType = MBT_LEFT;
    buttonName = TEXT("LEFT");
  }
  else if (MouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
  {
    buttonType = MBT_RIGHT;
    buttonName = TEXT("RIGHT");
  }
  else if (MouseEvent.GetEffectingButton() == EKeys::MiddleMouseButton)
  {
    buttonType = MBT_MIDDLE;
    buttonName = TEXT("MIDDLE");
  }
  else
  {
    UE_LOG(LogNEONWidget, Warning, TEXT("[CLICK DEBUG] HandleMouseButtonEvent - Unknown button type, ignoring"));
    return;
  }

  UE_LOG(LogNEONWidget, Warning, TEXT("[CLICK DEBUG] → Sending to CEF: %s %s at (%d, %d)"),
         *buttonName,
         bIsMouseUp ? TEXT("UP") : TEXT("DOWN"),
         cefEvent.x,
         cefEvent.y);

  _Browser->GetHost()->SendMouseClickEvent(cefEvent, buttonType, bIsMouseUp, 1);
}

void UNEONWidget::HandleMouseMoveEvent(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent)
{
  if (!_Browser)
    return;
  CefMouseEvent cefEvent = GetCefMouseEvent(MyGeometry, MouseEvent);
  bool mouseLeave = false;

  _Browser->GetHost()->SendMouseMoveEvent(cefEvent, mouseLeave);
}

void UNEONWidget::HandleMouseWheelEvent(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent)
{
  if (!_Browser)
    return;
  CefMouseEvent cefEvent = GetCefMouseEvent(MyGeometry, MouseEvent);
  float deltaY = MouseEvent.GetWheelDelta() * 120;

  _Browser->GetHost()->SendMouseWheelEvent(cefEvent, 0, deltaY);
}

void UNEONWidget::HandleKeyEvent(const FKeyEvent &InKeyEvent, bool bIsKeyUp)
{
  if (!_Browser)
    return;
  CefKeyEvent cefEvent = GetCefKeyEvent(InKeyEvent, bIsKeyUp);
  _Browser->GetHost()->SendKeyEvent(cefEvent);
}

void UNEONWidget::HandleCharacterEvent(const FCharacterEvent &InCharacterEvent)
{
  if (!_Browser)
    return;
  CefKeyEvent cefEvent = GetCefCharacterEvent(InCharacterEvent);
  _Browser->GetHost()->SendKeyEvent(cefEvent);
}

CefMouseEvent UNEONWidget::GetCefMouseEvent(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent)
{
  CefMouseEvent cefEvent;
  FVector2D localPos = MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());

  cefEvent.x = static_cast<int>(localPos.X) * _ScaleFactor;
  cefEvent.y = static_cast<int>(localPos.Y) * _ScaleFactor;
  cefEvent.modifiers = 0;

  if (MouseEvent.IsShiftDown())
    cefEvent.modifiers |= EVENTFLAG_SHIFT_DOWN;
  if (MouseEvent.IsControlDown())
    cefEvent.modifiers |= EVENTFLAG_CONTROL_DOWN;
  if (MouseEvent.IsAltDown())
    cefEvent.modifiers |= EVENTFLAG_ALT_DOWN;

  if (MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
    cefEvent.modifiers |= EVENTFLAG_LEFT_MOUSE_BUTTON;
  if (MouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
    cefEvent.modifiers |= EVENTFLAG_RIGHT_MOUSE_BUTTON;
  if (MouseEvent.IsMouseButtonDown(EKeys::MiddleMouseButton))
    cefEvent.modifiers |= EVENTFLAG_MIDDLE_MOUSE_BUTTON;

  return cefEvent;
}

CefKeyEvent UNEONWidget::GetCefKeyEvent(const FKeyEvent &InKeyEvent, bool bIsKeyUp)
{
  CefKeyEvent cefEvent;
  cefEvent.windows_key_code = InKeyEvent.GetKeyCode();
  cefEvent.native_key_code = InKeyEvent.GetKeyCode();
  cefEvent.is_system_key = InKeyEvent.IsCommandDown();
  cefEvent.modifiers = 0;

  if (InKeyEvent.IsShiftDown())
    cefEvent.modifiers |= EVENTFLAG_SHIFT_DOWN;
  if (InKeyEvent.IsControlDown())
    cefEvent.modifiers |= EVENTFLAG_CONTROL_DOWN;
  if (InKeyEvent.IsAltDown())
    cefEvent.modifiers |= EVENTFLAG_ALT_DOWN;

  cefEvent.type = bIsKeyUp ? KEYEVENT_KEYUP : KEYEVENT_RAWKEYDOWN;

  return cefEvent;
}

CefKeyEvent UNEONWidget::GetCefCharacterEvent(const FCharacterEvent &InCharacterEvent)
{
  CefKeyEvent cefEvent;
  cefEvent.type = KEYEVENT_CHAR;
  cefEvent.character = InCharacterEvent.GetCharacter();
  cefEvent.unmodified_character = InCharacterEvent.GetCharacter();
  cefEvent.windows_key_code = InCharacterEvent.GetCharacter();
  cefEvent.native_key_code = InCharacterEvent.GetCharacter();
  cefEvent.modifiers = 0;

  if (InCharacterEvent.IsShiftDown())
    cefEvent.modifiers |= EVENTFLAG_SHIFT_DOWN;
  if (InCharacterEvent.IsControlDown())
    cefEvent.modifiers |= EVENTFLAG_CONTROL_DOWN;
  if (InCharacterEvent.IsAltDown())
    cefEvent.modifiers |= EVENTFLAG_ALT_DOWN;

  return cefEvent;
}

void UNEONWidget::SetPopupVisible(bool Visible)
{
  if (!_View)
  {
    UE_LOG(LogNEONWidget, Error, TEXT("No NEONView to set popup visible."));
    return;
  }
  _View->SetPopupVisible(Visible);
  UE_LOG(LogNEONWidget, Log, TEXT("Popup visibility: %d"), Visible);
}

void UNEONWidget::SetPopupRect(int InX, int InY, int InWidth, int InHeight)
{
  if (!_View)
  {
    UE_LOG(LogNEONWidget, Error, TEXT("No NEONView to set popup rect."));
    return;
  }
  _View->SetPopupPosition(FVector2D(InX, InY));
  _View->SetPopupSize(FVector2D(InWidth, InHeight));
}
