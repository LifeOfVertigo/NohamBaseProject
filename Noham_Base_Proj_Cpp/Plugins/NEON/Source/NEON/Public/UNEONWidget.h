/*
 * Copyright (C) 2024 Michael Saller - All Rights Reserved
 * Published 2025 by TECHTILE media via FAB.com
 */
// UNEONWidget.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Engine/Texture2D.h"
#include "TimerManager.h"
#include "JsonObjectWrapper.h"

#include "Windows/AllowWindowsPlatformTypes.h"
THIRD_PARTY_INCLUDES_START
#include "include/cef_client.h"
#include "include/cef_browser.h"
#include "include/cef_render_handler.h"
THIRD_PARTY_INCLUDES_END
#include "Windows/HideWindowsPlatformTypes.h"

#include "NEONClient.h"

#include "UNEONWidget.generated.h"

class NEONView;

UCLASS(BlueprintType, Blueprintable)
class NEON_API UNEONWidget : public UUserWidget
{
  GENERATED_BODY()

public:
  // DEFAULTS
  UNEONWidget(const FObjectInitializer &ObjectInitializer)
      : Super(ObjectInitializer)
  {
    SetIsFocusable(true);
  }

  bool IsInteractable() const
  {
    return true;
  }

  bool SupportsKeyboardFocus() const
  {
    return true;
  }

  virtual void NativeConstruct() override;
  virtual void NativeTick(const FGeometry &MyGeometry, float InDeltaTime) override;
  virtual void NativeDestruct() override;

  // Rendering
  void OnAcceleratedPaint_Widget(HANDLE SharedHandle);
  void OnAcceleratedPaint_Widget_Popup(HANDLE SharedHandle);
  void InvalidateBrowser();

protected:
  // The brush used by _BrowserImage
  FSlateBrush _TextureBrush;

  // Abstract NEONView pointer (either NEONView_11 or NEONView_12).
  NEONView *_View = nullptr;

  // Scale factor used for input transformations.
  float _ScaleFactor;

  // CEF references
  CefRefPtr<NEONClient> _Client;
  CefRefPtr<CefBrowser> _Browser;

public:
  // UMG
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget), Category = "NEON")
  UImage *_BrowserImage;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NEON")
  bool _OpenDevTools = false;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NEON")
  FText _DevelopmentURL;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NEON")
  FText _LiveURL;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NEON")
  bool _ExternalBeginFrame = false;

  UFUNCTION(BlueprintCallable, Category = "NEON", meta = (ToolTip = "Processing time in milliseconds (module wide!)"))
  void SetProcessingTime(float ProcessingTime);

  // FPS
  FTimerHandle _FPSTimerHandle;
  int _FPSTransient = 0;
  int _FPS = 0;
  UFUNCTION(BlueprintCallable, Category = "NEON")
  int GetFPS();

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NEON")
  int _MaxFPS = 60;
  UFUNCTION(BlueprintCallable, Category = "NEON")
  void SetMaxFPS(int MaxFPS);

public:
  // INVOCATION
  UFUNCTION(BlueprintCallable, Category = "NEON")
  void InvokeWebNoParam(const FString &Method);

  UFUNCTION(BlueprintCallable, Category = "NEON")
  void InvokeWeb(const FString &Method, const FJsonObjectWrapper &JsonObjectWrapper);

  UFUNCTION(BlueprintCallable, Category = "NEON")
  void InvokeWebBoolean(const FString &Method, bool Value);

  UFUNCTION(BlueprintCallable, Category = "NEON")
  void InvokeWebInteger(const FString &Method, int32 Value);

  UFUNCTION(BlueprintCallable, Category = "NEON")
  void InvokeWebFloat(const FString &Method, float Value);

  UFUNCTION(BlueprintCallable, Category = "NEON")
  void InvokeWebString(const FString &Method, const FString &Value);

  // - unreal
  void InvokeUnreal(const FString &Data);

  // LIFE CYCLE
  UFUNCTION(BlueprintImplementableEvent, Category = "NEON")
  void OnBrowserCreated();

  UFUNCTION(BlueprintCallable, Category = "NEON")
  void RestartBrowser();

  // NATIVE INPUT
  virtual FReply NativeOnMouseButtonDown(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent) override;
  virtual FReply NativeOnMouseButtonUp(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent) override;
  virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry &InGeometry, const FPointerEvent &InMouseEvent) override;

  virtual FReply NativeOnMouseMove(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent) override;
  virtual FReply NativeOnMouseWheel(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent) override;

  virtual FReply NativeOnKeyDown(const FGeometry &MyGeometry, const FKeyEvent &InKeyEvent) override;
  virtual FReply NativeOnKeyUp(const FGeometry &MyGeometry, const FKeyEvent &InKeyEvent) override;
  virtual FReply NativeOnKeyChar(const FGeometry &MyGeometry, const FCharacterEvent &InCharacterEvent) override;

  // CEF HANDLING
  void HandleMouseButtonEvent(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent, bool bIsMouseUp);
  void HandleMouseMoveEvent(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent);
  void HandleMouseWheelEvent(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent);
  void HandleKeyEvent(const FKeyEvent &InKeyEvent, bool bIsKeyUp);
  void HandleCharacterEvent(const FCharacterEvent &InCharacterEvent);

  // CEF HELPERS
  CefMouseEvent GetCefMouseEvent(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent);
  CefKeyEvent GetCefKeyEvent(const FKeyEvent &InKeyEvent, bool bIsKeyUp);
  CefKeyEvent GetCefCharacterEvent(const FCharacterEvent &InCharacterEvent);

  NEONClient *GetClient() const { return _Client.get(); }
  CefBrowser *GetBrowser() const { return _Browser.get(); }
  FSlateBrush &GetTextureBrush() { return _TextureBrush; }

  // POPUP
  UFUNCTION(BlueprintCallable, Category = "NEON")
  void SetPopupVisible(bool bVisible);

  UFUNCTION(BlueprintCallable, Category = "NEON")
  void SetPopupRect(int InX, int InY, int InWidth, int InHeight);

protected:
  void CreateBrowser();
};
