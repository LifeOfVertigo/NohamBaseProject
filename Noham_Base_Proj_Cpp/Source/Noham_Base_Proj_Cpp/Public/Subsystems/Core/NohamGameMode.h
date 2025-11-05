// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NohamGameMode.generated.h"

/**
 * Base game mode for Noham project
 * Sets up default pawn and game rules
 *
 * References:
 * - docs/unreal-engine/core-systems/actors.md - GameMode patterns
 */
UCLASS(minimalapi)
class ANohamGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ANohamGameMode();

protected:
	/**
	 * Called when the game starts or when spawned
	 * NOTE: BeginPlay implementation removed - use Blueprint Event Graph instead
	 */
	virtual void BeginPlay() override;
};
