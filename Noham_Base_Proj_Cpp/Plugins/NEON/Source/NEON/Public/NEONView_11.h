/*
 * Copyright (C) 2024 Michael Saller - All Rights Reserved
 * Published 2025 by TECHTILE media via FAB.com
 */
// NEONView_11.h

#pragma once

#include "NEONView.h"
#include "ID3D11DynamicRHI.h"
#include "Windows/AllowWindowsPlatformTypes.h"
#include <d3d11.h>
#include <d3d11_1.h>
#include "Windows/HideWindowsPlatformTypes.h"

using namespace Microsoft::WRL;

/**
 * NEONView_11 implements D3D11-based resource copying,
 * used by UNEONWidget for OnAcceleratedPaint with a D3D11 RHI.
 *
 * BE AWARE THAT THIS VIEW MIGHT BE REMOVED IN THE FUTURE!
 */
class NEONView_11 : public NEONView
{
public:
  virtual ~NEONView_11() override {}

  virtual bool InitializeView(UNEONWidget *InWidget) override;
  virtual void DestroyView() override;

  virtual void OnAcceleratedPaint_View(HANDLE SharedHandle) override;
  virtual void OnAcceleratedPaint_View_Popup(HANDLE SharedHandle) override;

  virtual void SetPopupVisible(bool Visible) override;

  bool TryCreateD3D11Device();

private:
  // D3D11 device references
  ComPtr<ID3D11Device> _D3D11Device;
  ComPtr<ID3D11Device1> _D3D11Device1;
  ComPtr<ID3D11DeviceContext> _D3D11Context;
  ComPtr<ID3D11DeviceContext1> _D3D11Context1;
  ID3D11DynamicRHI *_D3D11RHI = nullptr;

  // Extra references for dynamic/popup textures
  ComPtr<ID3D11Texture2D> _SharedResourcePopup_D3D11;
};
