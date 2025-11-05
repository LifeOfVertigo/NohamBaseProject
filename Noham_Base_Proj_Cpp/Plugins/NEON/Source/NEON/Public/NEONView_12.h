/*
 * Copyright (C) 2024 Michael Saller - All Rights Reserved
 * Published 2025 by TECHTILE media via FAB.com
 */
// NEONView_12.h

#pragma once

#include "NEONView.h"
#include "ID3D12DynamicRHI.h"
#include "Windows/AllowWindowsPlatformTypes.h"
#include <d3d12.h>
#include "Windows/HideWindowsPlatformTypes.h"

using namespace Microsoft::WRL;

/**
 * NEONView_12 implements D3D12-based resource copying,
 * used by UNEONWidget for OnAcceleratedPaint with a D3D12 RHI.
 */
class NEONView_12 : public NEONView
{
public:
  virtual ~NEONView_12() override {}

  virtual bool InitializeView(UNEONWidget *InWidget) override;
  virtual void DestroyView() override;

  virtual void OnAcceleratedPaint_View(HANDLE SharedHandle) override;
  virtual void OnAcceleratedPaint_View_Popup(HANDLE SharedHandle) override;

  virtual void SetPopupVisible(bool Visible) override;

  bool TryCreateD3D12Device();

private:
  // D3D12 device references
  ComPtr<ID3D12Device> _D3D12Device;
  ComPtr<ID3D12CommandQueue> _D3D12CommandQueue;
  ID3D12DynamicRHI *_D3D12RHI = nullptr;

  // For popup
  ComPtr<ID3D12Resource> _SharedResourcePopup_D3D12;
};
