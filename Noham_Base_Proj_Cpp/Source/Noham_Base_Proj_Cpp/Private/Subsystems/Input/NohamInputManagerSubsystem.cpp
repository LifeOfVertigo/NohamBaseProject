// Copyright Noham Studios. All Rights Reserved.

#include "Subsystems/Input/NohamInputManagerSubsystem.h"
#include "GameFramework/InputSettings.h"
#include "Framework/Application/SlateApplication.h"
#include "Misc/ConfigCacheIni.h"

void UNohamInputManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UE_LOG(LogTemp, Log, TEXT("NohamInputManagerSubsystem::Initialize"));

	// Detect platform
	DetectPlatform();

	// Detect initial input mode
	DetectInitialInputMode();

	// Register default input prompts
	RegisterDefaultPrompts();

	UE_LOG(LogTemp, Log, TEXT("Input Manager initialized. Mode: %d, Steam Deck: %s"),
		static_cast<int32>(CurrentInputMode),
		bIsSteamDeck ? TEXT("Yes") : TEXT("No"));
}

void UNohamInputManagerSubsystem::Deinitialize()
{
	UE_LOG(LogTemp, Log, TEXT("NohamInputManagerSubsystem::Deinitialize"));
	Super::Deinitialize();
}

bool UNohamInputManagerSubsystem::IsGamepadConnected() const
{
	// Check for connected gamepads via Slate Application
	if (FSlateApplication::IsInitialized())
	{
		return FSlateApplication::Get().IsGamepadAttached();
	}
	return false;
}

void UNohamInputManagerSubsystem::SetInputMode(ENohamInputMode NewMode)
{
	if (CurrentInputMode != NewMode)
	{
		CurrentInputMode = NewMode;
		BroadcastInputModeChange(NewMode);
		UE_LOG(LogTemp, Log, TEXT("Input mode manually set to: %d"), static_cast<int32>(NewMode));
	}
}

FText UNohamInputManagerSubsystem::GetInputPrompt(const FString& ActionName)
{
	// Look up registered prompt
	if (InputPromptRegistry.Contains(ActionName))
	{
		const FNohamInputPromptData& PromptData = InputPromptRegistry[ActionName];

		switch (CurrentInputMode)
		{
		case ENohamInputMode::KeyboardMouse:
			return PromptData.KeyboardMouseText;

		case ENohamInputMode::Gamepad:
		case ENohamInputMode::SteamDeck:
			return PromptData.GamepadText;

		case ENohamInputMode::Touch:
			return FText::FromString("Tap");

		default:
			return PromptData.KeyboardMouseText;
		}
	}

	// Default fallback
	return FText::FromString(ActionName);
}

FString UNohamInputManagerSubsystem::GetInputPromptIconPath(const FString& ActionName)
{
	if (InputPromptRegistry.Contains(ActionName))
	{
		const FNohamInputPromptData& PromptData = InputPromptRegistry[ActionName];

		switch (CurrentInputMode)
		{
		case ENohamInputMode::KeyboardMouse:
			return PromptData.KeyboardMouseIconPath;

		case ENohamInputMode::Gamepad:
		case ENohamInputMode::SteamDeck:
			return PromptData.GamepadIconPath;

		default:
			return PromptData.KeyboardMouseIconPath;
		}
	}

	return FString();
}

void UNohamInputManagerSubsystem::RegisterInputPrompt(const FString& ActionName, const FNohamInputPromptData& PromptData)
{
	InputPromptRegistry.Add(ActionName, PromptData);
	UE_LOG(LogTemp, Verbose, TEXT("Registered input prompt: %s"), *ActionName);
}

void UNohamInputManagerSubsystem::NotifyInputReceived(bool bFromGamepad)
{
	// Debounce input mode changes
	double CurrentTime = FPlatformTime::Seconds();
	if (CurrentTime - LastInputModeChangeTime < INPUT_MODE_CHANGE_DEBOUNCE)
	{
		return;
	}

	// Determine new input mode
	ENohamInputMode NewMode = bFromGamepad
		? (bIsSteamDeck ? ENohamInputMode::SteamDeck : ENohamInputMode::Gamepad)
		: ENohamInputMode::KeyboardMouse;

	// Switch mode if different
	if (NewMode != CurrentInputMode)
	{
		CurrentInputMode = NewMode;
		LastInputModeChangeTime = CurrentTime;
		BroadcastInputModeChange(NewMode);
		UE_LOG(LogTemp, Log, TEXT("Input mode auto-switched to: %d"), static_cast<int32>(NewMode));
	}
}

void UNohamInputManagerSubsystem::DetectPlatform()
{
	// Steam Deck detection
	// Steam Deck sets environment variable SteamDeck=1
	FString SteamDeckEnv = FPlatformMisc::GetEnvironmentVariable(TEXT("SteamDeck"));
	bIsSteamDeck = !SteamDeckEnv.IsEmpty() && SteamDeckEnv.Equals(TEXT("1"));

	// Alternative: Check for specific hardware identifiers
	if (!bIsSteamDeck)
	{
		FString DeviceMake = FPlatformMisc::GetDeviceMakeAndModel();
		bIsSteamDeck = DeviceMake.Contains(TEXT("Valve")) && DeviceMake.Contains(TEXT("Jupiter"));
	}
}

void UNohamInputManagerSubsystem::DetectInitialInputMode()
{
	if (bIsSteamDeck)
	{
		// Steam Deck defaults to gamepad mode
		CurrentInputMode = ENohamInputMode::SteamDeck;
	}
	else if (IsGamepadConnected())
	{
		// Gamepad connected, default to gamepad
		CurrentInputMode = ENohamInputMode::Gamepad;
	}
	else
	{
		// Default to keyboard/mouse
		CurrentInputMode = ENohamInputMode::KeyboardMouse;
	}
}

void UNohamInputManagerSubsystem::RegisterDefaultPrompts()
{
	// Movement
	{
		FNohamInputPromptData Data;
		Data.KeyboardMouseText = FText::FromString("WASD");
		Data.GamepadText = FText::FromString("Left Stick");
		RegisterInputPrompt("Move", Data);
	}

	// Look
	{
		FNohamInputPromptData Data;
		Data.KeyboardMouseText = FText::FromString("Mouse");
		Data.GamepadText = FText::FromString("Right Stick");
		RegisterInputPrompt("Look", Data);
	}

	// Jump
	{
		FNohamInputPromptData Data;
		Data.KeyboardMouseText = FText::FromString("Space");
		Data.GamepadText = FText::FromString("A");
		RegisterInputPrompt("Jump", Data);
	}

	// Crouch
	{
		FNohamInputPromptData Data;
		Data.KeyboardMouseText = FText::FromString("C");
		Data.GamepadText = FText::FromString("B");
		RegisterInputPrompt("Crouch", Data);
	}

	// Sprint
	{
		FNohamInputPromptData Data;
		Data.KeyboardMouseText = FText::FromString("Shift");
		Data.GamepadText = FText::FromString("Left Stick Click");
		RegisterInputPrompt("Sprint", Data);
	}

	// Aim
	{
		FNohamInputPromptData Data;
		Data.KeyboardMouseText = FText::FromString("Right Mouse");
		Data.GamepadText = FText::FromString("Left Trigger");
		RegisterInputPrompt("Aim", Data);
	}

	// Interact
	{
		FNohamInputPromptData Data;
		Data.KeyboardMouseText = FText::FromString("E");
		Data.GamepadText = FText::FromString("X");
		RegisterInputPrompt("Interact", Data);
	}

	// Menu/Pause
	{
		FNohamInputPromptData Data;
		Data.KeyboardMouseText = FText::FromString("Esc");
		Data.GamepadText = FText::FromString("Start");
		RegisterInputPrompt("Menu", Data);
	}

	UE_LOG(LogTemp, Log, TEXT("Registered %d default input prompts"), InputPromptRegistry.Num());
}

void UNohamInputManagerSubsystem::BroadcastInputModeChange(ENohamInputMode NewMode)
{
	OnInputModeChanged.Broadcast(NewMode);
}
