// Copyright Epic Games, Inc. All Rights Reserved.

#include "Subsystems/Core/NohamGameMode.h"
#include "Components/Characters/NohamCharacter.h"
#include "Subsystems/UI/NohamUIManagerSubsystem.h"
#include "UObject/ConstructorHelpers.h"

ANohamGameMode::ANohamGameMode()
	: Super()
{
	// Set default pawn class to our character
	DefaultPawnClass = ANohamCharacter::StaticClass();
}

void ANohamGameMode::BeginPlay()
{
	Super::BeginPlay();

	// UI spawning now handled in Blueprint Event Graph
	// See GameMode Blueprint for widget creation logic
	UE_LOG(LogTemp, Log, TEXT("[GameMode] BeginPlay - Use Blueprint Event Graph for UI setup"));
}
