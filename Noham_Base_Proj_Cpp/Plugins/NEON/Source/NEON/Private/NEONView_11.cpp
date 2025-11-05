/*
 * Copyright (C) 2024 Michael Saller - All Rights Reserved
 * Published 2025 by TECHTILE media via FAB.com
 */
// NEONView_11.cpp

#include "NEONView_11.h"
#include "Engine/Engine.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "RenderGraphUtils.h"
#include "RenderGraphBuilder.h"
#include "RHI.h"
#include "Logging/LogMacros.h"
#include "Containers/UnrealString.h"
#include "Misc/Paths.h"
#include "Async/Async.h"
#include "Blueprint/WidgetLayoutLibrary.h"

#include "NEONLogging.h"
#include "UNEONWidget.h"

bool NEONView_11::InitializeView(UNEONWidget *Widget)
{
  NEONView::InitializeView(Widget);
  if (!TryCreateD3D11Device())
  {
    UE_LOG(LogNEONView, Verbose, TEXT("Failed to create D3D11.1 device."));
    return false;
  }
  UE_LOG(LogNEONView, Log, TEXT("D3D11 device created."));
  return true;
}

bool NEONView_11::TryCreateD3D11Device()
{
  ERHIInterfaceType currentRHI = RHIGetInterfaceType();
  if (currentRHI != ERHIInterfaceType::D3D11)
  {
    UE_LOG(LogNEONView, Verbose, TEXT("Current RHI is not D3D11."));
    return false;
  }

  D3D_FEATURE_LEVEL featureLevels[] = {D3D_FEATURE_LEVEL_11_1};

  // Create the D3D11 device
  D3D_FEATURE_LEVEL selectedFeatureLevel;
  HRESULT hr = D3D11CreateDevice(
      nullptr,
      D3D_DRIVER_TYPE_HARDWARE,
      nullptr,
      0,
      featureLevels,
      ARRAYSIZE(featureLevels),
      D3D11_SDK_VERSION,
      &_D3D11Device,
      &selectedFeatureLevel,
      &_D3D11Context);

  if (FAILED(hr))
  {
    UE_LOG(LogNEONView, Fatal, TEXT("Failed to create D3D11 device. HRESULT: 0x%08X"), hr);
    return false;
  }

  if (!_D3D11Device)
  {
    UE_LOG(LogNEONView, Fatal, TEXT("D3D11 device is null."));
    return false;
  }
  if (!_D3D11Context)
  {
    UE_LOG(LogNEONView, Fatal, TEXT("D3D11 device context is null."));
    return false;
  }

  // Query interfaces for Device1 and Context1
  hr = _D3D11Device->QueryInterface(__uuidof(ID3D11Device1), (void **)&_D3D11Device1);
  if (FAILED(hr))
  {
    UE_LOG(LogNEONView, Fatal, TEXT("Failed to get D3D11Device1. HRESULT: 0x%08X"), hr);
    return false;
  }

  if (!_D3D11Device1)
  {
    UE_LOG(LogNEONView, Fatal, TEXT("D3D11Device1 is null."));
    return false;
  }

  hr = _D3D11Context->QueryInterface(__uuidof(ID3D11DeviceContext1), (void **)&_D3D11Context1);
  if (FAILED(hr))
  {
    UE_LOG(LogNEONView, Fatal, TEXT("Failed to get D3D11DeviceContext1. HRESULT: 0x%08X"), hr);
    return false;
  }

  if (!_D3D11Context1)
  {
    UE_LOG(LogNEONView, Fatal, TEXT("D3D11DeviceContext1 is null."));
    return false;
  }

  // Set the D3DRHI
  _D3D11RHI = static_cast<ID3D11DynamicRHI *>(GDynamicRHI);
  if (!_D3D11RHI)
  {
    UE_LOG(LogNEONView, Fatal, TEXT("D3D11RHI is null."));
    return false;
  }

  return true;
}

void NEONView_11::DestroyView()
{
  NEONView::DestroyView();

  _D3D11Device.Reset();
  _D3D11Device1.Reset();
  _D3D11Context.Reset();
  _D3D11Context1.Reset();
  _D3D11RHI = nullptr;
  _SharedResourcePopup_D3D11.Reset();
}

void NEONView_11::OnAcceleratedPaint_View(HANDLE SharedHandle)
{
  if (!_IsInUse)
  {
    return;
  }

  if (!_Widget)
  {
    UE_LOG(LogNEONView, Error, TEXT("NEONView::OnAcceleratedPaint_View: _Widget null."));
    return;
  }

  if (!_D3D11Device)
  {
    UE_LOG(LogNEONView, Warning, TEXT("NEONView::OnAcceleratedPaint_View: D3D11 device is null."));
    return;
  }

  // Open the shared resource
  ComPtr<ID3D11Texture2D> sharedTexture;
  HRESULT hr = _D3D11Device1->OpenSharedResource1(SharedHandle, __uuidof(ID3D11Texture2D), (void **)&sharedTexture);
  if (FAILED(hr))
  {
    UE_LOG(LogNEONView, Error, TEXT("NEONView::OnAcceleratedPaint_View: Failed to open shared handle (D3D11). HRESULT: 0x%08X"), hr);
    return;
  }

  D3D11_TEXTURE2D_DESC sharedDesc;
  sharedTexture->GetDesc(&sharedDesc);

  if (sharedDesc.Width != static_cast<int>(_WidgetSize.X) || sharedDesc.Height != static_cast<int>(_WidgetSize.Y))
  {
    UE_LOG(LogNEONView, Log, TEXT("WidgetSize: %d, %d"), static_cast<int>(_WidgetSize.X), static_cast<int>(_WidgetSize.Y));
    UE_LOG(LogNEONView, Log, TEXT("Shared: %d, %d"), sharedDesc.Width, sharedDesc.Height);
    UE_LOG(LogNEONView, Log, TEXT("Dimensions do not match. Invalidating and waiting for next frame"));
    _Widget->InvalidateBrowser();
    return;
  }

  // Get the dynamic texture RHI for the render thread
  FRHITexture *dynamicRHITexture = GetDynamicRHITexture();
  if (!dynamicRHITexture)
  {
    UE_LOG(LogNEONView, Error, TEXT("NEONView::OnAcceleratedPaint_View: dynamic RHI texture is null."));
    return;
  }
  ComPtr<ID3D11Texture2D> dynamicTexture = static_cast<ID3D11Texture2D *>(dynamicRHITexture->GetNativeResource());

  // Copy from the shared resource into our dynamic texture
  ENQUEUE_RENDER_COMMAND(CopyExternalTextureToUTexture)
  (
      [this, dynamicTexture, sharedTexture](FRHICommandListImmediate &RHICmdList) mutable
      {
        _D3D11Context1->CopyResource(dynamicTexture.Get(), sharedTexture.Get());

        // If popup visible, also copy the popup texture
        if (_IsPopupVisible && _SharedResourcePopup_D3D11)
        {
          D3D11_BOX srcRegion;
          srcRegion.left = 0;
          srcRegion.top = 0;
          srcRegion.front = 0;
          srcRegion.right = static_cast<UINT>(_PopupSize.X);
          srcRegion.bottom = static_cast<UINT>(_PopupSize.Y);
          srcRegion.back = 1;

          _D3D11Context1->CopySubresourceRegion(
              dynamicTexture.Get(),
              0,
              static_cast<UINT>(_PopupPosition.X),
              static_cast<UINT>(_PopupPosition.Y),
              0,
              _SharedResourcePopup_D3D11.Get(),
              0,
              &srcRegion);
        }

        _D3D11Context1->Flush();
      });
}

void NEONView_11::OnAcceleratedPaint_View_Popup(HANDLE SharedHandle)
{
  if (!_IsPopupVisible)
  {
    UE_LOG(LogNEONView, Error, TEXT("Popup is not visible (D3D11)."));
    return;
  }
  if (!_D3D11Device1)
  {
    UE_LOG(LogNEONView, Error, TEXT("D3D11Device1 is null."));
    return;
  }

  // Store the popup resource pointer
  HRESULT hr = _D3D11Device1->OpenSharedResource1(SharedHandle, __uuidof(ID3D11Texture2D), (void **)&_SharedResourcePopup_D3D11);
  if (FAILED(hr))
  {
    UE_LOG(LogNEONView, Error, TEXT("Failed to open shared popup handle (D3D11) with HRESULT: 0x%08X"), hr);
    return;
  }

  if (!_Widget)
  {
    return;
  }
  _Widget->InvalidateBrowser();
}

void NEONView_11::SetPopupVisible(bool Visible)
{
  _IsPopupVisible = Visible;
  if (!Visible)
  {
    _SharedResourcePopup_D3D11.Reset();
  }
}