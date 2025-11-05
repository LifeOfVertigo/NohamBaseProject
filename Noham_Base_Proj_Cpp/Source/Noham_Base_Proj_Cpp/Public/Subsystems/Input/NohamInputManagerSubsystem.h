// Copyright Noham Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "NohamInputManagerSubsystem.generated.h"

/**
 * ENohamInputMode
 * Enumeration of supported input modes
 */
UENUM(BlueprintType)
enum class ENohamInputMode : uint8
{
	KeyboardMouse UMETA(DisplayName = "Keyboard & Mouse"),
	Gamepad UMETA(DisplayName = "Gamepad"),
	SteamDeck UMETA(DisplayName = "Steam Deck"),
	Touch UMETA(DisplayName = "Touch") // Future: for mobile/tablet
};

/**
 * FNohamInputPromptData
 * Data structure for input prompt icons and text
 */
USTRUCT(BlueprintType)
struct FNohamInputPromptData
{
	GENERATED_BODY()

	/** Key/button name for keyboard/mouse (e.g., "E", "Left Mouse") */
	UPROPERTY(BlueprintReadWrite, Category = "Input")
	FText KeyboardMouseText = FText::FromString("E");

	/** Button name for gamepad (e.g., "A Button", "X Button") */
	UPROPERTY(BlueprintReadWrite, Category = "Input")
	FText GamepadText = FText::FromString("A");

	/** Icon path for keyboard/mouse (relative to Content folder) */
	UPROPERTY(BlueprintReadWrite, Category = "Input")
	FString KeyboardMouseIconPath;

	/** Icon path for gamepad (relative to Content folder) */
	UPROPERTY(BlueprintReadWrite, Category = "Input")
	FString GamepadIconPath;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInputModeChanged, ENohamInputMode, NewInputMode);

/**
 * UNohamInputManagerSubsystem
 *
 * Manages input device detection and automatic switching
 * Provides dynamic input prompt system for UI
 * Handles Steam Deck specific input considerations
 *
 * References:
 * - docs/architecture/coding-standards.md - Input handling standards
 * - docs/unreal-engine/gameplay/input.md - Enhanced Input system
 */
UCLASS()
class NOHAM_BASE_PROJ_CPP_API UNohamInputManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// Subsystem lifecycle
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/**
	 * Get current input mode
	 */
	UFUNCTION(BlueprintPure, Category = "Noham|Input")
	ENohamInputMode GetCurrentInputMode() const { return CurrentInputMode; }

	/**
	 * Check if gamepad is connected
	 */
	UFUNCTION(BlueprintPure, Category = "Noham|Input")
	bool IsGamepadConnected() const;

	/**
	 * Check if running on Steam Deck
	 */
	UFUNCTION(BlueprintPure, Category = "Noham|Input")
	bool IsSteamDeck() const { return bIsSteamDeck; }

	/**
	 * Manually switch input mode (for testing or user preference)
	 * @param NewMode - Input mode to switch to
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Input")
	void SetInputMode(ENohamInputMode NewMode);

	/**
	 * Get input prompt for current input mode
	 * @param ActionName - Name of the action (e.g., "Interact", "Jump", "Fire")
	 * @return Text to display for the current input mode
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Input")
	FText GetInputPrompt(const FString& ActionName);

	/**
	 * Get input prompt icon path for current input mode
	 * @param ActionName - Name of the action
	 * @return Path to icon asset
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Input")
	FString GetInputPromptIconPath(const FString& ActionName);

	/**
	 * Register an input prompt for an action
	 * @param ActionName - Unique action identifier
	 * @param PromptData - Prompt data for all input modes
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Input")
	void RegisterInputPrompt(const FString& ActionName, const FNohamInputPromptData& PromptData);

	/**
	 * Update input mode based on last input device used
	 * Call this from input handling code when input is detected
	 * @param bFromGamepad - true if input came from gamepad, false for keyboard/mouse
	 */
	UFUNCTION(BlueprintCallable, Category = "Noham|Input")
	void NotifyInputReceived(bool bFromGamepad);

	/**
	 * Event broadcast when input mode changes
	 */
	UPROPERTY(BlueprintAssignable, Category = "Noham|Input")
	FOnInputModeChanged OnInputModeChanged;

private:
	// Current input mode
	UPROPERTY()
	ENohamInputMode CurrentInputMode = ENohamInputMode::KeyboardMouse;

	// Platform detection
	bool bIsSteamDeck = false;

	// Input prompt registry
	UPROPERTY()
	TMap<FString, FNohamInputPromptData> InputPromptRegistry;

	// Internal helpers
	void DetectPlatform();
	void DetectInitialInputMode();
	void RegisterDefaultPrompts();
	void BroadcastInputModeChange(ENohamInputMode NewMode);

	// Last input timestamp for debouncing
	double LastInputModeChangeTime = 0.0;
	static constexpr double INPUT_MODE_CHANGE_DEBOUNCE = 0.5; // seconds
};
