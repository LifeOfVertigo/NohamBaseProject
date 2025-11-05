/*
 * Copyright (C) 2024 Michael Saller - All Rights Reserved
 * Published 2025 by TECHTILE media via FAB.com
 */
// NEONView_12.cpp

#include "NEONView_12.h"
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

bool NEONView_12::InitializeView(UNEONWidget *Widget)
{
  NEONView::InitializeView(Widget);
  if (!TryCreateD3D12Device())
  {
    UE_LOG(LogNEONView, Verbose, TEXT("Failed to create D3D12 device."));
    return false;
  }
  UE_LOG(LogNEONView, Log, TEXT("D3D12 device created."));
  return true;
}

bool NEONView_12::TryCreateD3D12Device()
{
  ERHIInterfaceType currentRHI = RHIGetInterfaceType();
  if (currentRHI != ERHIInterfaceType::D3D12)
  {
    UE_LOG(LogNEONView, Verbose, TEXT("Current RHI is not D3D12."));
    return false;
  }

  HRESULT hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&_D3D12Device));
  if (FAILED(hr))
  {
    UE_LOG(LogNEONView, Fatal, TEXT("Failed to create D3D12 device. HRESULT: 0x%08X"), hr);
    return false;
  }
  if (!_D3D12Device)
  {
    UE_LOG(LogNEONView, Fatal, TEXT("D3D12 device is null."));
    return false;
  }

  // Create the D3D12CommandQueue
  D3D12_COMMAND_QUEUE_DESC queueDesc = {};
  queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
  queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
  queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
  queueDesc.NodeMask = 0;

  hr = _D3D12Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&_D3D12CommandQueue));
  if (FAILED(hr))
  {
    UE_LOG(LogNEONView, Fatal, TEXT("Failed to create D3D12 command queue. HRESULT: 0x%08X"), hr);
    return false;
  }

  _D3D12RHI = static_cast<ID3D12DynamicRHI *>(GDynamicRHI);
  if (!_D3D12RHI)
  {
    UE_LOG(LogNEONView, Fatal, TEXT("D3D12RHI is null."));
    return false;
  }

  return true;
}

void NEONView_12::DestroyView()
{
  NEONView::DestroyView();
  _D3D12Device.Reset();
  _D3D12CommandQueue.Reset();
  _D3D12RHI = nullptr;
  _SharedResourcePopup_D3D12.Reset();
}

void NEONView_12::OnAcceleratedPaint_View(HANDLE SharedHandle)
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

  if (!_D3D12Device)
  {
    UE_LOG(LogNEONView, Error, TEXT("NEONView::OnAcceleratedPaint_View: D3D12 device is null."));
    return;
  }

  // Get the dynamic texture RHI for the render thread
  FRHITexture *dynamicRHITexture = GetDynamicRHITexture();
  if (!dynamicRHITexture)
  {
    UE_LOG(LogNEONView, Warning, TEXT("NEONView::OnAcceleratedPaint_View: dynamicRHITexture is null."));
    return;
  }

  // Open the shared handle
  ComPtr<ID3D12Resource> sharedResource;
  HRESULT hr = _D3D12Device->OpenSharedHandle(SharedHandle, IID_PPV_ARGS(&sharedResource));
  if (FAILED(hr))
  {
    UE_LOG(LogNEONView, Error, TEXT("NEONView::OnAcceleratedPaint_View: Failed to open shared handle. HRESULT: 0x%08X"), hr);
    return;
  }

  // Check if dimensions match
  D3D12_RESOURCE_DESC sharedDesc = sharedResource->GetDesc();
  if (sharedDesc.Width != static_cast<int>(_WidgetSize.X) || sharedDesc.Height != static_cast<int>(_WidgetSize.Y))
  {
    UE_LOG(LogNEONView, Log, TEXT("WidgetSize: %d, %d"), static_cast<int>(_WidgetSize.X), static_cast<int>(_WidgetSize.Y));
    UE_LOG(LogNEONView, Log, TEXT("Shared: %d, %d"), sharedDesc.Width, sharedDesc.Height);
    UE_LOG(LogNEONView, Log, TEXT("Dimensions do not match. Invalidating and waiting for next frame"));
    _Widget->InvalidateBrowser();
    return;
  }

  // Use a render command to copy from the shared texture into dynamic texture
  UE_LOG(LogNEONView, Verbose, TEXT("About to start render (D3D12)."));
  ENQUEUE_RENDER_COMMAND(CopyExternalTextureToUTexture)
  (
      [this, sharedResource, dynamicRHITexture, sharedDesc, SharedHandle](FRHICommandListImmediate &RHICmdList) mutable
      {
        const EPixelFormat format = PF_B8G8R8A8;
        const ETextureCreateFlags texCreateFlags = TexCreate_ShaderResource;
        const FClearValueBinding clearValueBinding = FClearValueBinding::None;

        // Create a FRHITexture from the shared resource
        FRHITexture *sharedRHITexture = _D3D12RHI->RHICreateTexture2DFromResource(format, texCreateFlags, clearValueBinding, sharedResource.Get());
        if (!sharedRHITexture)
        {
          UE_LOG(LogNEONView, Error, TEXT("Failed to create sharedRHITexture (D3D12)."));
          return;
        }

        FRDGBuilder graphBuilder(RHICmdList);

        FRDGTextureRef sourceRDGTexture = graphBuilder.RegisterExternalTexture(CreateRenderTarget(sharedRHITexture, TEXT("SourceRDGTexture_D3D12")));
        FRDGTextureRef destRDGTexture = graphBuilder.RegisterExternalTexture(CreateRenderTarget(dynamicRHITexture, TEXT("DestRDGTexture_D3D12")));

        // Copy main texture
        AddCopyTexturePass(graphBuilder, sourceRDGTexture, destRDGTexture);

        // If popup is visible, also copy the popup
        if (_IsPopupVisible && _SharedResourcePopup_D3D12)
        {
          UE_LOG(LogNEONView, Verbose, TEXT("Popup is visible. Rendering in main (D3D12)."));

          FRHITexture *sharedTexturePopupRHI = _D3D12RHI->RHICreateTexture2DFromResource(format, texCreateFlags, clearValueBinding, _SharedResourcePopup_D3D12.Get());
          if (!sharedTexturePopupRHI)
          {
            UE_LOG(LogNEONView, Error, TEXT("Failed to create sharedTexturePopupRHI (D3D12)."));
            return;
          }

          FRHICopyTextureInfo copyInfo;
          copyInfo.Size = FIntVector(_PopupSize.X, _PopupSize.Y, 1);
          copyInfo.SourcePosition = FIntVector(0, 0, 0);
          copyInfo.DestPosition = FIntVector(_PopupPosition.X, _PopupPosition.Y, 0);

          FRDGTextureRef sourceRDGTexturePopup = graphBuilder.RegisterExternalTexture(CreateRenderTarget(sharedTexturePopupRHI, TEXT("SourceRDGTexturePopup_D3D12")));
          AddCopyTexturePass(graphBuilder, sourceRDGTexturePopup, destRDGTexture, copyInfo);
        }

        graphBuilder.Execute();
        sharedResource.Reset();
        //
      });
}

void NEONView_12::OnAcceleratedPaint_View_Popup(HANDLE SharedHandle)
{
  if (!_IsPopupVisible)
  {
    UE_LOG(LogNEONView, Error, TEXT("Popup is not visible (D3D12)."));
    return;
  }
  if (!_D3D12Device)
  {
    UE_LOG(LogNEONView, Error, TEXT("D3D12 device is null."));
    return;
  }

  // Store the popup texture for usage in the main paint
  HRESULT hr = _D3D12Device->OpenSharedHandle(SharedHandle, IID_PPV_ARGS(&_SharedResourcePopup_D3D12));
  if (FAILED(hr))
  {
    UE_LOG(LogNEONView, Error, TEXT("Failed to open shared handle for popup (D3D12). HRESULT: 0x%08X"), hr);
    return;
  }

  if (!_Widget)
  {
    return;
  }
  _Widget->InvalidateBrowser();
}

void NEONView_12::SetPopupVisible(bool Visible)
{
  UE_LOG(LogNEONView, Log, TEXT("NEONView_12::SetPopupVisible: %d"), Visible);
  _IsPopupVisible = Visible;
  if (!Visible)
  {
    _SharedResourcePopup_D3D12.Reset();
  }
}