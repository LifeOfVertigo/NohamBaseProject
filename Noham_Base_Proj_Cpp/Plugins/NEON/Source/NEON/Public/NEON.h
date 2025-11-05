/*
 * Copyright (C) 2024 Michael Saller - All Rights Reserved
 * Published 2025 by TECHTILE media via FAB.com
 */
#pragma once

#include "Modules/ModuleManager.h"
#include "HAL/PlatformProcess.h"
#include "Engine/Engine.h"
#include "Engine/World.h"

class FNEONModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	UFUNCTION(BlueprintCallable, Category = "NEON")
	void SetProcessingTime(float ProcessingTime);

private:
	void *_LibecfHandle;

	bool _IsInitialized = false;
	float _ProcessingTime = 0.0f;

	void OnPreWorldInitialization(UWorld *World, const UWorld::InitializationValues IVS);
	void OnWorldTickStart(UWorld *World, ELevelTick TickType, float DeltaSeconds);
	void OnWorldTickEnd(UWorld *World, ELevelTick TickType, float DeltaSeconds);
};
