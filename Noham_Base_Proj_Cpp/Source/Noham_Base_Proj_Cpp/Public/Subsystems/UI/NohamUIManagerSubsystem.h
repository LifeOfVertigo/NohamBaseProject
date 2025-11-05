// Copyright Noham Studios. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "NohamUIManagerSubsystem.generated.h"

/**
 * UNohamUIManagerSubsystem
 *
 * Manages all UI widgets in the game, including NEON-based menus and HUD.
 * Handles showing/hiding widgets, input mode switching, and widget lifecycle.
 */
UCLASS()
class NOHAM_BASE_PROJ_CPP_API UNohamUIManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// Subsystem lifecycle
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/**
	 * Show a widget by name
	 * @param WidgetName - Identifier for the widget (e.g., "MainMenu", "Settings")
	 * @param bSetInputModeUI - Whether to set input mode to UI only
	 * @param bShowMouseCursor - Whether to show the mouse cursor
	 * @param ZOrder - Z-order for viewport layering (higher = on top)
	 * @return The created/shown widget, or nullptr if failed
	 */
	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	UUserWidget* ShowWidget(
		const FString& WidgetName,
		bool bSetInputModeUI = true,
		bool bShowMouseCursor = true,
		int32 ZOrder = 0
	);

	/**
	 * Hide a widget by name
	 * @param WidgetName - Identifier for the widget to hide
	 * @param bDestroy - Whether to destroy the widget (true) or just remove from viewport (false)
	 */
	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	void HideWidget(const FString& WidgetName, bool bDestroy = false);

	/**
	 * Hide all currently visible widgets
	 * @param bDestroy - Whether to destroy widgets or just remove from viewport
	 */
	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	void HideAllWidgets(bool bDestroy = false);

	/**
	 * Check if a widget is currently visible
	 */
	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	bool IsWidgetVisible(const FString& WidgetName) const;

	/**
	 * Get reference to a widget (creates if not exists)
	 */
	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	UUserWidget* GetWidget(const FString& WidgetName);

	/**
	 * Set input mode to UI only with optional widget to focus
	 */
	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	void SetInputModeUIOnly(UUserWidget* WidgetToFocus = nullptr, bool bShowMouseCursor = true);

	/**
	 * Set input mode to Game only (hides cursor, focus on game)
	 */
	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	void SetInputModeGameOnly(bool bHideMouseCursor = true);

	/**
	 * Set input mode to Game and UI (cursor visible, game still receives input)
	 */
	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	void SetInputModeGameAndUI(UUserWidget* WidgetToFocus = nullptr, bool bShowMouseCursor = true);

	/**
	 * Show main menu (shortcut)
	 */
	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	void ShowMainMenu();

	/**
	 * Show settings menu (shortcut)
	 */
	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	void ShowSettings();

	/**
	 * Show pause menu (shortcut)
	 */
	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	void ShowPauseMenu();

	/**
	 * Show HUD (shortcut)
	 */
	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	void ShowHUD();

	/**
	 * Return to game (hide all UI, restore game input)
	 */
	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	void ReturnToGame();

private:
	/**
	 * Widget class registry
	 * Maps widget names to their blueprint classes
	 */
	UPROPERTY()
	TMap<FString, TSubclassOf<UUserWidget>> WidgetClasses;

	/**
	 * Active widget instances
	 * Cached widgets that have been created
	 */
	UPROPERTY()
	TMap<FString, UUserWidget*> ActiveWidgets;

	/**
	 * Currently visible widgets
	 * Tracks which widgets are on viewport
	 */
	UPROPERTY()
	TSet<FString> VisibleWidgets;

	/**
	 * Initialize widget class registry
	 * Loads all widget blueprint classes
	 */
	void InitializeWidgetRegistry();

	/**
	 * Register a widget class
	 */
	void RegisterWidgetClass(const FString& WidgetName, const TCHAR* BlueprintPath);

	/**
	 * Get the player controller for input mode changes
	 */
	APlayerController* GetPlayerController() const;

	/**
	 * Log UI manager action
	 */
	void LogUIAction(const FString& Action, const FString& WidgetName = TEXT("")) const;
};
