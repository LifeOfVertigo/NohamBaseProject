/*
 * Copyright (C) 2024 Michael Saller - All Rights Reserved
 * Published 2025 by TECHTILE media via FAB.com
 */
// NEONApp.h

#pragma once

#include "Windows/AllowWindowsPlatformTypes.h"
THIRD_PARTY_INCLUDES_START
#include "include/cef_app.h"
THIRD_PARTY_INCLUDES_END
#include "Windows/HideWindowsPlatformTypes.h"

class NEONApp : public CefApp
{
public:
  NEONApp()
  {
  }

  void OnBeforeCommandLineProcessing(const CefString &ProcessType,
                                     CefRefPtr<CefCommandLine> CommandLine) override
  {
    CommandLine->AppendSwitch("enable-gpu");
    CommandLine->AppendSwitch("enable-gpu-compositing");
    CommandLine->AppendSwitch("enable-gpu-rasterization");
    // CommandLine->AppendSwitch("enable-zero-copy");
    CommandLine->AppendSwitch("enable-gpu-vsync");
    CommandLine->AppendSwitch("enable-accelerated-video");
    CommandLine->AppendSwitch("enable-accelerated-video-decode");

    CommandLine->AppendSwitch("enable-begin-frame-scheduling");
    // CommandLine->AppendSwitch("external-begin-frame-enabled");

    // CommandLine->AppendSwitch("enable-gpu-service-tracing");
    // CommandLine->AppendSwitch("enable-gpu-logging");
    // CommandLine->AppendSwitch("enable-draw-fps");
    // CommandLine->AppendSwitch("disable-gpu-shader-disk-cache");
    // CommandLine->AppendSwitch("show-fps-counter");

    CommandLine->AppendSwitch("disable-extensions");
    CommandLine->AppendSwitch("disable-software-rasterizer");
    CommandLine->AppendSwitch("disable-web-security");

    CommandLine->AppendSwitch("allow-universal-access-from-files");

    // CommandLine->AppendSwitch("gpu-startup-dialog");

#if WITH_EDITOR
    CommandLine->AppendSwitchWithValue("log-severity", "verbose");
    CommandLine->AppendSwitchWithValue("v", "3");

    FString pluginBaseDir = IPluginManager::Get().FindPlugin(TEXT("NEON"))->GetBaseDir();
    pluginBaseDir = FPaths::ConvertRelativePathToFull(pluginBaseDir);
    CommandLine->AppendSwitchWithValue("log-file", TCHAR_TO_UTF8(*FPaths::Combine(pluginBaseDir, TEXT("cef.log"))));
#else
    CommandLine->AppendSwitchWithValue("log-severity", "disable");
#endif
  }

  IMPLEMENT_REFCOUNTING(NEONApp);
};