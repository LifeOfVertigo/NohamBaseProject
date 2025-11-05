/*
 * Copyright (C) 2024 Michael Saller - All Rights Reserved
 * Published 2025 by TECHTILE media via FAB.com
 */
// NEONView.h

#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "Misc/Paths.h"
#include "RHI.h"
#include "RHIResources.h"
#include "Containers/UnrealString.h"
#include "HAL/Platform.h"

#include "Windows/AllowWindowsPlatformTypes.h"
#include "wrl/client.h"
#include "Windows/HideWindowsPlatformTypes.h"

#include "UNEONWidget.h"

class NEONView
{
public:
  virtual ~NEONView() {}

  virtual bool InitializeView(UNEONWidget *Widget)
  {
    _Widget = Widget;
    _IsInUse = true;
    return true;
  };
  virtual void DestroyView()
  {
    _IsInUse = false;
    _Widget = nullptr;
  };

  /**
   *
   */
  virtual void OnAcceleratedPaint_View(HANDLE SharedHandle) = 0;

  virtual void OnAcceleratedPaint_View_Popup(HANDLE SharedHandle) = 0;

  bool IsPopupVisible() const { return _IsPopupVisible; }
  virtual void SetPopupVisible(bool Visible) = 0;
  const FVector2D &GetPopupPosition() const { return _PopupPosition; }
  void SetPopupPosition(const FVector2D &Position) { _PopupPosition = Position; }
  const FVector2D &GetPopupSize() const { return _PopupSize; }
  void SetPopupSize(const FVector2D &Size) { _PopupSize = Size; }

  /**
   * SetWidgetSize is called on Tick with the current widget dimensions.
   * If the sizes differ, a new dynamic texture is created and the RHI is stored.
   * We call WasResized on the browser host. This will trigger a new paint.
   * The NEONView implementations need to make sure to handle the pointers appropriately in OnAcceleratedPaint:
   *  create a new pointer to use on the render thread each time to avoid painting into stale or initializing textures!
   * If this is properly handled, no race conditions of painting and resizing can happen because Tick and OnAcceleratedPaint are on the same thread.
   */
  void SetWidgetSize(const FVector2D &Size);

protected:
  // set to true on creation, false on destruction
  bool _IsInUse = false;

  // Pointer back to the widget that owns this view.
  UNEONWidget *_Widget = nullptr;
  FVector2D _WidgetSize = FVector2D(1024, 1024);

  // The dynamic texture used to display the CEF output
  UTexture2D *_DynamicTexture = nullptr;

  FRHITexture *GetDynamicRHITexture();

  // Popup state
  bool _IsPopupVisible = false;
  FVector2D _PopupPosition = FVector2D(0, 0);
  FVector2D _PopupSize = FVector2D(1024, 1024);
};