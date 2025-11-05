/*
 * Copyright (C) 2024 Michael Saller - All Rights Reserved
 * Published 2025 by TECHTILE media via FAB.com
 */
// NEONClient.cpp

#include "NEONClient.h"
#include "NEONLogging.h"
#include "NEONMessageHandler.h"
#include "UNEONWidget.h"

NEONClient::NEONClient(NEONMessageHandler *MessageHandler)
    : _MessageHandler(MessageHandler)
{
  CefMessageRouterConfig config;
  _MessageRouter = CefMessageRouterBrowserSide::Create(config);
  _MessageRouter->AddHandler(MessageHandler, false);
}

void NEONClient::Stop()
{
  if (_MessageRouter && _MessageHandler)
  {
    _MessageRouter->RemoveHandler(_MessageHandler);
  }
  _Widget = nullptr;
  delete _MessageHandler;
}

bool NEONClient::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                          CefRefPtr<CefFrame> frame,
                                          CefProcessId source_process,
                                          CefRefPtr<CefProcessMessage> message)
{
  // Pass message to router (for JS <-> UE bridging)
  return _MessageRouter->OnProcessMessageReceived(browser, frame, source_process, message);
}

void NEONClient::SetWidget(UNEONWidget *Widget)
{
  _Widget = Widget;
}

//----------------------------------------------------------------------
// Popup creation (for <select> combos, etc.)
//----------------------------------------------------------------------
bool NEONClient::OnBeforePopup(CefRefPtr<CefBrowser> browser,
                               CefRefPtr<CefFrame> frame,
                               int popup_id,
                               const CefString &target_url,
                               const CefString &target_frame_name,
                               WindowOpenDisposition target_disposition,
                               bool user_gesture,
                               const CefPopupFeatures &popupFeatures,
                               CefWindowInfo &windowInfo,
                               CefRefPtr<CefClient> &client,
                               CefBrowserSettings &settings,
                               CefRefPtr<CefDictionaryValue> &extra_info,
                               bool *no_javascript_access)
{
  // Force windowless OSR with shared textures:
  windowInfo.SetAsWindowless(nullptr);
  windowInfo.windowless_rendering_enabled = true;
  windowInfo.shared_texture_enabled = true;
  // windowInfo.external_begin_frame_enabled = true;

  // Reuse this same client so OnAcceleratedPaint can handle both main + popup.
  client = this;

  // returning false tells CEF “Allow the popup to be created”
  return false;
}

//----------------------------------------------------------------------
// OSR screen info
//----------------------------------------------------------------------
void NEONClient::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect)
{
  rect = CefRect(0, 0, _Width, _Height);
}

bool NEONClient::GetScreenInfo(CefRefPtr<CefBrowser> browser, CefScreenInfo &screen_info)
{
  screen_info.device_scale_factor = 1.0f;
  screen_info.rect = CefRect(0, 0, _Width, _Height);
  screen_info.available_rect = screen_info.rect;
  return true;
}

bool NEONClient::GetRootScreenRect(CefRefPtr<CefBrowser> browser, CefRect &rect)
{
  rect = CefRect(0, 0, _Width, _Height);
  return true;
}

//----------------------------------------------------------------------
// Popup show/hide
//----------------------------------------------------------------------
void NEONClient::OnPopupShow(CefRefPtr<CefBrowser> /*browser*/, bool show)
{
  if (_Widget)
  {
    _Widget->SetPopupVisible(show);
  }
}

// The popup coordinate/size in *browser* coordinates
void NEONClient::OnPopupSize(CefRefPtr<CefBrowser> /*browser*/, const CefRect &rect)
{
  if (_Widget)
  {
    // store popup geometry (in device px)
    _Widget->SetPopupRect(rect.x, rect.y, rect.width, rect.height);
  }
}

//----------------------------------------------------------------------
// Actually handle drawing for main vs popup
//----------------------------------------------------------------------
void NEONClient::OnAcceleratedPaint(CefRefPtr<CefBrowser> browser,
                                    PaintElementType type,
                                    const CefRenderHandler::RectList & /*dirtyRects*/,
                                    const CefAcceleratedPaintInfo &paintInfo)
{
  if (!paintInfo.shared_texture_handle)
  {
    UE_LOG(LogNEON, Error, TEXT("Shared texture handle is null."));
    return;
  }
  if (!_Widget)
  {
    UE_LOG(LogNEON, Warning, TEXT("Widget is null. This might happen when the widget is closed but CEF is still sending frames."));
    return;
  }

  if (type == PET_VIEW)
  {
    _Widget->OnAcceleratedPaint_Widget(paintInfo.shared_texture_handle);
  }
  else if (type == PET_POPUP)
  {
    _Widget->OnAcceleratedPaint_Widget_Popup(paintInfo.shared_texture_handle);
  }
}