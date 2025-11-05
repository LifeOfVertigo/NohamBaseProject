/*
 * Copyright (C) 2024 Michael Saller - All Rights Reserved
 * Published 2025 by TECHTILE media via FAB.com
 */
// NEON.cpp
#include "NEON.h"
#include "Interfaces/IPluginManager.h"
#include "HAL/PlatformFileManager.h"
#include "HttpModule.h"
#include "IPlatformFilePak.h"
#include "Misc/FileHelper.h"
#include "Misc/MessageDialog.h"
#include "Interfaces/IHttpResponse.h"
#include "Misc/Paths.h"

#include "NEONLogging.h"

#include "NEONApp.h"

void FNEONModule::StartupModule()
{
  UE_LOG(LogNEON, Warning, TEXT("Starting NEON module!"));

  FString pluginBaseDir = FPaths::Combine(FPaths::ConvertRelativePathToFull(FPaths::ProjectPluginsDir()), TEXT("NEON"));

  // Load libcef.dll
  FString pluginBinariesDir = FPaths::Combine(pluginBaseDir, TEXT("Binaries/Win64"));

  FPlatformProcess::AddDllDirectory(*pluginBinariesDir);
  UE_LOG(LogNEON, Warning, TEXT("Added plugin binaries directory: %s"), *pluginBinariesDir);

  FString libCefPath = FPaths::Combine(*pluginBinariesDir, TEXT("libcef.dll"));

  UE_LOG(LogNEON, Warning, TEXT("Loaded libcef.dll from %s"), *libCefPath);

  _LibecfHandle = !libCefPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*libCefPath) : nullptr;
  if (!_LibecfHandle)
  {
    UE_LOG(LogNEON, Fatal, TEXT("Failed to load libcef.dll from %s"), *libCefPath);
    return;
  }
#if WITH_EDITOR
  // Get plugin version name
  FString pluginVersionName = IPluginManager::Get().FindPlugin(TEXT("NEON"))->GetDescriptor().VersionName;

  FString pluginCEFLibDir = FPaths::Combine(pluginBaseDir, TEXT("Source/ThirdParty/CEF/lib"));
  FString subLibFilePath = FPaths::Combine(pluginCEFLibDir, FString::Printf(TEXT("NEONCEFsub_%s.exe"), *pluginVersionName));
  FString subBinFilePath = FPaths::Combine(pluginBinariesDir, FString::Printf(TEXT("NEONCEFsub_%s.exe"), *pluginVersionName));

  // Check if NEONCEFsub exe exists, download if no
  if (!FPaths::FileExists(subLibFilePath) || !FPaths::FileExists(subBinFilePath))
  {
    // File URL
    FString Url = TEXT("https://github.com/micon4sure/neon-cefsub/raw/refs/heads/master/Release/NEONCEFsub_") + pluginVersionName + TEXT(".exe");

    TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->OnProcessRequestComplete().BindLambda(
        [subLibFilePath, subBinFilePath, pluginVersionName](FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success)
        {
          // Check for download success
          if (!Success || !Response.IsValid())
          {
            UE_LOG(LogNEON, Fatal, TEXT("Failed to download NEONCEFsub executable. Check https://neon.techtile.media/Documentation#NEONCEFsub."));
            FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Failed to download NEONCEFsub executable. Check https://neon.techtile.media/Documentation#NEONCEFsub.")));
            return;
          }
          UE_LOG(LogNEON, Log, TEXT("Downloaded NEONCEFsub_%s.exe"), *pluginVersionName);

          // Try to save file to lib directory
          if (!FFileHelper::SaveArrayToFile(Response->GetContent(), *subLibFilePath))
          {
            bool libPathExists = FPaths::DirectoryExists(FPaths::GetPath(subLibFilePath));

            UE_LOG(LogNEON, Fatal, TEXT("Failed to save NEONCEFsub executable to %s. Check https://neon.techtile.media/Documentation#NEONCEFsub. Directory exists: %d"), *subLibFilePath, libPathExists);
            FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Failed to save NEONCEFsub executable. Check https://neon.techtile.media/Documentation#NEONCEFsub.")));
            return;
          }
          UE_LOG(LogNEON, Log, TEXT("Saved NEONCEFsub_%s.exe to %s"), *pluginVersionName, *subLibFilePath);

          // Try to copy file to binaries directory
          IPlatformFile &PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
          if (!PlatformFile.CopyFile(*subBinFilePath, *subLibFilePath))
          {
            bool binPathExists = FPaths::DirectoryExists(FPaths::GetPath(subBinFilePath));
            UE_LOG(LogNEON, Fatal, TEXT("Failed to copy NEONCEFsub executable to %s. Directory exists: %d. Check https://neon.techtile.media/Documentation#NEONCEFsub."), *subBinFilePath, binPathExists);
            FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Failed to copy NEONCEFsub executable to binaries directory. Check https://neon.techtile.media/Documentation#NEONCEFsub.")));
            return;
          }

          UE_LOG(LogNEON, Log, TEXT("Copied NEONCEFsub_%s.exe to %s"), *pluginVersionName, *subBinFilePath);
          //
        });

    HttpRequest->SetURL(Url);
    HttpRequest->SetVerb(TEXT("GET"));
    HttpRequest->SetHeader(TEXT("User-Agent"), TEXT("UnrealEngine"));
    HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/octet-stream"));
    HttpRequest->ProcessRequest();
  }
#endif
  FWorldDelegates::OnPreWorldInitialization.AddRaw(this, &FNEONModule::OnPreWorldInitialization);
  FWorldDelegates::OnWorldTickStart.AddRaw(this, &FNEONModule::OnWorldTickStart);

  UE_LOG(LogNEON, Log, TEXT("NEON module has started!"));
}

void FNEONModule::OnWorldTickStart(UWorld *World, ELevelTick TickType, float DeltaSeconds)
{
  CefDoMessageLoopWork();
  if (_ProcessingTime <= 0)
    return;
  FPlatformProcess::Sleep(_ProcessingTime * .001f);
}

// Set processing time in milliseconds
void FNEONModule::SetProcessingTime(float ProcessingTime)
{
  _ProcessingTime = ProcessingTime;
}

void FNEONModule::OnPreWorldInitialization(UWorld *World, const UWorld::InitializationValues IVS)
{
  if (!World)
  {
    UE_LOG(LogNEON, Log, TEXT("NEON Module begin play. Got no world. Skipping."));
    return;
  }

  switch (World->WorldType)
  {
  case EWorldType::PIE:
    UE_LOG(LogNEON, Log, TEXT("Initializing CEF for PIE world."));
    break;

  case EWorldType::GamePreview:
    UE_LOG(LogNEON, Log, TEXT("Initializing CEF for GamePreview world."));
    break;

  case EWorldType::Game:
    UE_LOG(LogNEON, Log, TEXT("Initializing CEF for Game world."));
    break;

  default:
    UE_LOG(LogNEON, Log, TEXT("OnPreWorldInitialization is not PIE, GamePreview, or Game."));
    return;
  }

  // Initialize CEF
  CefMainArgs mainArgs = CefMainArgs(hInstance);

  FString pluginBaseDir = IPluginManager::Get().FindPlugin(TEXT("NEON"))->GetBaseDir();

  // Create log path
  FString CEFLogPath = FPaths::Combine(pluginBaseDir, TEXT("cef.log"));
  FString AbsoluteLogPath = FPaths::ConvertRelativePathToFull(CEFLogPath);
  UE_LOG(LogNEON, Log, TEXT("Log Path: %s"), *AbsoluteLogPath);

  // Create subprocess path
  FString SubprocessPath = FPaths::Combine(pluginBaseDir, *FString::Printf(TEXT("Binaries/Win64/NEONCEFsub.exe")));
  FString AbsoluteSubprocessPath = FPaths::ConvertRelativePathToFull(SubprocessPath);
  UE_LOG(LogNEON, Log, TEXT("Subprocess Path: %s"), *AbsoluteSubprocessPath);

  if (FPaths::FileExists(AbsoluteSubprocessPath))
  {
    UE_LOG(LogNEON, Log, TEXT("Subprocess executable exists at path: %s"), *AbsoluteSubprocessPath);
  }
  else
  {
    UE_LOG(LogNEON, Fatal, TEXT("Subprocess executable does not exist at path: %s"), *AbsoluteSubprocessPath);
  }

  // Initialize CEF settings
  UE_LOG(LogNEON, Log, TEXT("Initializing CEF"));
  CefSettings settings;
  settings.no_sandbox = true;

  settings.background_color = 0;
  settings.command_line_args_disabled = false;

  settings.windowless_rendering_enabled = true; // Enable OSR
  settings.multi_threaded_message_loop = false; // Must be false for windowless rendering

#if WITH_EDITOR
  settings.log_severity = LOGSEVERITY_VERBOSE;
#endif
  CefString(&settings.log_file) = TCHAR_TO_UTF8(*AbsoluteLogPath);

  CefString(&settings.browser_subprocess_path) = TCHAR_TO_UTF8(*AbsoluteSubprocessPath);
  CefRefPtr<NEONApp> app(new NEONApp());

  if (!CefInitialize(mainArgs, settings, app.get(), nullptr))
  {
    int exitCode = CefGetExitCode();
    UE_LOG(LogNEON, Fatal, TEXT("Failed to initialize CEF. Exit code: %d"), exitCode);
    return;
  }
  _IsInitialized = true;

  UE_LOG(LogNEON, Log, TEXT("CEF Initialized"));
}

void FNEONModule::ShutdownModule()
{

  if (_IsInitialized)
  {
    CefShutdown();
  }

  FPlatformProcess::FreeDllHandle(_LibecfHandle);
  _LibecfHandle = nullptr;
}

IMPLEMENT_MODULE(FNEONModule, NEON)