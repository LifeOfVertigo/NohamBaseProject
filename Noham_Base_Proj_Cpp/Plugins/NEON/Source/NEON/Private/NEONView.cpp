/*
 * Copyright (C) 2024 Michael Saller - All Rights Reserved
 * Published 2025 by TECHTILE media via FAB.com
 */
// NEONView.cpp

#include "NEONView.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "RHI.h"
#include "RenderGraphUtils.h"
#include "RenderGraphBuilder.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "Logging/LogMacros.h"

#include "UNEONWidget.h"
#include "NEONLogging.h"

void NEONView::SetWidgetSize(const FVector2D &Size)
{
  if (!_Widget->GetBrowser())
  {
    UE_LOG(LogNEONView, Error, TEXT("NEONView::SetWidgetSize: Browser is null."));
    return;
  }

  // Check if the browser image is present
  if (!_Widget->_BrowserImage)
  {
    UE_LOG(LogNEONView, Fatal, TEXT("NEONView::SetWidgetSize: _BrowserImage is null."));
    return;
  }

  // If size hasn't changed, no action needed
  if (_WidgetSize.X == Size.X && _WidgetSize.Y == Size.Y)
  {
    return;
  }
  _WidgetSize = Size;

  UE_LOG(LogNEONView, Log, TEXT("Resizing NEON view to: %dx%d"), static_cast<int>(Size.X), static_cast<int>(Size.Y));

  // Create the new dynamic texture
  _DynamicTexture = UTexture2D::CreateTransient(
      static_cast<int>(Size.X),
      static_cast<int>(Size.Y),
      PF_B8G8R8A8);
  if (!_DynamicTexture)
  {
    UE_LOG(LogNEONView, Error, TEXT("NEONView::SetWidgetSize: Failed to create dynamic texture."));
    return;
  }
  UE_LOG(LogNEONView, Log, TEXT("NEONView::SetWidgetSize: Created dynamic texture."));
  _DynamicTexture->UpdateResource();

  /**
   * We can't create the RHI texture here, as we need to wait for the render thread.
   * Implementing classes should get the RHI texture from GetDynamicRHITexture on each paint.
   * This also avoids race conditions of texture creation/initialization and painting
   * */

  // Update the widget's brush
  _Widget->GetTextureBrush().SetResourceObject(_DynamicTexture);
  _Widget->GetTextureBrush().ImageSize = FVector2D(Size.X, Size.Y);
  _Widget->_BrowserImage->SetBrush(_Widget->GetTextureBrush());

  _Widget->GetClient()->UpdateDimensions(Size.X, Size.Y);
  _Widget->GetBrowser()->GetHost()->WasResized();
}

FRHITexture *NEONView::GetDynamicRHITexture()
{
  if (!_DynamicTexture)
  {
    UE_LOG(LogNEONView, Error, TEXT("NEONView::GetDynamicRHITexture: _DynamicTexture is null."));
    return nullptr;
  }

  FTextureResource *dynamicTextureResource = _DynamicTexture->GetResource();
  if (!dynamicTextureResource)
  {
    UE_LOG(LogNEONView, Error, TEXT("NEONView::GetDynamicRHITexture: Failed to retrieve dynamic texture resource."));
    return nullptr;
  }
  if (!dynamicTextureResource->TextureRHI)
  {
    UE_LOG(LogNEONView, Warning, TEXT("NEONView::GetDynamicRHITexture: Failed to retrieve dynamic texture resource RHI. This may happen when the RHI texture is not yet initialized."));
    return nullptr;
  }
  FRHITexture *dynamicRHITexture = dynamicTextureResource->TextureRHI->GetTexture2D();
  if (!dynamicRHITexture)
  {
    UE_LOG(LogNEONView, Error, TEXT("NEONView::GetDynamicRHITexture: Failed to retrieve dynamic RHI texture."));
    return nullptr;
  }
  return dynamicRHITexture;
}