/*
 * Copyright (C) 2024 Michael Saller - All Rights Reserved
 * Published 2025 by TECHTILE media via FAB.com
 */
// NEONClient.h

#pragma once

#include "Windows/AllowWindowsPlatformTypes.h"
THIRD_PARTY_INCLUDES_START
#include "include/cef_client.h"
#include "include/wrapper/cef_message_router.h"

THIRD_PARTY_INCLUDES_END
#include "Windows/HideWindowsPlatformTypes.h"

class UNEONWidget;
class NEONMessageHandler;

class NEONClient
    : public CefClient,
      public CefLifeSpanHandler,
      public CefRenderHandler
{
public:
  NEONClient(NEONMessageHandler *MessageHandler);

  void Stop();

  CefRefPtr<CefRenderHandler> GetRenderHandler() override { return this; }
  CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; }

  bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                CefRefPtr<CefFrame> frame,
                                CefProcessId source_process,
                                CefRefPtr<CefProcessMessage> message) override;

  // RENDER HANDLER
  void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) override;
  bool GetScreenInfo(CefRefPtr<CefBrowser> browser, CefScreenInfo &screen_info) override;
  bool GetRootScreenRect(CefRefPtr<CefBrowser> browser, CefRect &rect) override;

  void OnPaint(CefRefPtr<CefBrowser> browser,
               PaintElementType type,
               const CefRenderHandler::RectList &dirtyRects,
               const void *buffer,
               int width,
               int height) override {}

  void OnAcceleratedPaint(CefRefPtr<CefBrowser> browser,
                          PaintElementType type,
                          const CefRenderHandler::RectList &dirtyRects,
                          const CefAcceleratedPaintInfo &paintInfo) override;

  // - Popups
  bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
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
                     bool *no_javascript_access);
  void OnPopupShow(CefRefPtr<CefBrowser> browser, bool show) override;
  void OnPopupSize(CefRefPtr<CefBrowser> browser, const CefRect &rect) override;
  void UpdateDimensions(int Width, int Height)
  {
    _Width = Width;
    _Height = Height;
  }
  void SetWidget(UNEONWidget *Widget);

private:
  CefRefPtr<CefMessageRouterBrowserSide> _MessageRouter;
  NEONMessageHandler *_MessageHandler;
  UNEONWidget *_Widget = nullptr;

  int _Width = 2048;
  int _Height = 2048;

  IMPLEMENT_REFCOUNTING(NEONClient);
};
