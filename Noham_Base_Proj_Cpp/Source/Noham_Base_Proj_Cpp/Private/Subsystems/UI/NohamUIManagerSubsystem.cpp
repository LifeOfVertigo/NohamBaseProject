// Copyright Noham Studios. All Rights Reserved.

#include "Subsystems/UI/NohamUIManagerSubsystem.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

void UNohamUIManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Initialize widget class registry
	InitializeWidgetRegistry();

	UE_LOG(LogTemp, Log, TEXT("[UI Manager] Subsystem initialized"));
}

void UNohamUIManagerSubsystem::Deinitialize()
{
	// Clean up all widgets
	HideAllWidgets(true);

	WidgetClasses.Empty();
	ActiveWidgets.Empty();
	VisibleWidgets.Empty();

	UE_LOG(LogTemp, Log, TEXT("[UI Manager] Subsystem deinitialized"));

	Super::Deinitialize();
}

void UNohamUIManagerSubsystem::InitializeWidgetRegistry()
{
	// Register all widget blueprint classes
	// Format: RegisterWidgetClass(Name, Blueprint Path)

	RegisterWidgetClass(TEXT("MainMenu"), TEXT("/Game/Widgets/CoreMenuWidgets/WBP_MainMenu.WBP_MainMenu_C"));
	RegisterWidgetClass(TEXT("Settings"), TEXT("/Game/Widgets/CoreMenuWidgets/WBP_Settings.WBP_Settings_C"));
	RegisterWidgetClass(TEXT("PauseMenu"), TEXT("/Game/Widgets/CoreMenuWidgets/WBP_PauseMenu.WBP_PauseMenu_C"));
	RegisterWidgetClass(TEXT("HUD"), TEXT("/Game/Widgets/WBP_HUD.WBP_HUD_C"));
	RegisterWidgetClass(TEXT("Developer"), TEXT("/Game/Widgets/WBP_Developer.WBP_Developer_C"));

	UE_LOG(LogTemp, Log, TEXT("[UI Manager] Registered %d widget classes"), WidgetClasses.Num());
}

void UNohamUIManagerSubsystem::RegisterWidgetClass(const FString& WidgetName, const TCHAR* BlueprintPath)
{
	TSubclassOf<UUserWidget> WidgetClass = LoadClass<UUserWidget>(nullptr, BlueprintPath);

	if (WidgetClass)
	{
		WidgetClasses.Add(WidgetName, WidgetClass);
		UE_LOG(LogTemp, Log, TEXT("[UI Manager] Registered widget: %s"), *WidgetName);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[UI Manager] Failed to load widget class: %s from %s"), *WidgetName, BlueprintPath);
	}
}

UUserWidget* UNohamUIManagerSubsystem::ShowWidget(
	const FString& WidgetName,
	bool bSetInputModeUI,
	bool bShowMouseCursor,
	int32 ZOrder
)
{
	LogUIAction(TEXT("ShowWidget"), WidgetName);

	// Get or create the widget
	UUserWidget* Widget = GetWidget(WidgetName);
	if (!Widget)
	{
		UE_LOG(LogTemp, Error, TEXT("[UI Manager] Failed to get widget: %s"), *WidgetName);
		return nullptr;
	}

	// Add to viewport if not already visible
	if (!VisibleWidgets.Contains(WidgetName))
	{
		Widget->AddToViewport(ZOrder);
		VisibleWidgets.Add(WidgetName);
	}

	// Set input mode if requested
	if (bSetInputModeUI)
	{
		SetInputModeUIOnly(Widget, bShowMouseCursor);
	}

	return Widget;
}

void UNohamUIManagerSubsystem::HideWidget(const FString& WidgetName, bool bDestroy)
{
	LogUIAction(TEXT("HideWidget"), WidgetName);

	if (!VisibleWidgets.Contains(WidgetName))
	{
		return; // Widget not visible
	}

	UUserWidget* Widget = ActiveWidgets.FindRef(WidgetName);
	if (Widget)
	{
		Widget->RemoveFromParent();
		VisibleWidgets.Remove(WidgetName);

		if (bDestroy)
		{
			Widget->ConditionalBeginDestroy();
			ActiveWidgets.Remove(WidgetName);
		}
	}
}

void UNohamUIManagerSubsystem::HideAllWidgets(bool bDestroy)
{
	LogUIAction(TEXT("HideAllWidgets"));

	// Copy the set to avoid modification during iteration
	TSet<FString> WidgetsToHide = VisibleWidgets;

	for (const FString& WidgetName : WidgetsToHide)
	{
		HideWidget(WidgetName, bDestroy);
	}
}

bool UNohamUIManagerSubsystem::IsWidgetVisible(const FString& WidgetName) const
{
	return VisibleWidgets.Contains(WidgetName);
}

UUserWidget* UNohamUIManagerSubsystem::GetWidget(const FString& WidgetName)
{
	// Check if widget already exists
	if (UUserWidget** ExistingWidget = ActiveWidgets.Find(WidgetName))
	{
		return *ExistingWidget;
	}

	// Check if widget class is registered
	TSubclassOf<UUserWidget>* WidgetClass = WidgetClasses.Find(WidgetName);
	if (!WidgetClass || !(*WidgetClass))
	{
		UE_LOG(LogTemp, Error, TEXT("[UI Manager] Widget class not found: %s"), *WidgetName);
		return nullptr;
	}

	// Create new widget
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("[UI Manager] Invalid world context"));
		return nullptr;
	}

	UUserWidget* NewWidget = CreateWidget<UUserWidget>(World, *WidgetClass);
	if (NewWidget)
	{
		ActiveWidgets.Add(WidgetName, NewWidget);
		UE_LOG(LogTemp, Log, TEXT("[UI Manager] Created widget: %s"), *WidgetName);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[UI Manager] Failed to create widget: %s"), *WidgetName);
	}

	return NewWidget;
}

void UNohamUIManagerSubsystem::SetInputModeUIOnly(UUserWidget* WidgetToFocus, bool bShowMouseCursor)
{
	APlayerController* PC = GetPlayerController();
	if (!PC)
	{
		return;
	}

	FInputModeUIOnly InputMode;
	if (WidgetToFocus)
	{
		InputMode.SetWidgetToFocus(WidgetToFocus->TakeWidget());
	}
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PC->SetInputMode(InputMode);
	PC->bShowMouseCursor = bShowMouseCursor;

	LogUIAction(TEXT("SetInputMode"), TEXT("UI Only"));
}

void UNohamUIManagerSubsystem::SetInputModeGameOnly(bool bHideMouseCursor)
{
	APlayerController* PC = GetPlayerController();
	if (!PC)
	{
		return;
	}

	FInputModeGameOnly InputMode;
	PC->SetInputMode(InputMode);
	PC->bShowMouseCursor = !bHideMouseCursor;

	LogUIAction(TEXT("SetInputMode"), TEXT("Game Only"));
}

void UNohamUIManagerSubsystem::SetInputModeGameAndUI(UUserWidget* WidgetToFocus, bool bShowMouseCursor)
{
	APlayerController* PC = GetPlayerController();
	if (!PC)
	{
		return;
	}

	FInputModeGameAndUI InputMode;
	if (WidgetToFocus)
	{
		InputMode.SetWidgetToFocus(WidgetToFocus->TakeWidget());
	}
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PC->SetInputMode(InputMode);
	PC->bShowMouseCursor = bShowMouseCursor;

	LogUIAction(TEXT("SetInputMode"), TEXT("Game and UI"));
}

void UNohamUIManagerSubsystem::ShowMainMenu()
{
	ShowWidget(TEXT("MainMenu"), true, true, 0);
}

void UNohamUIManagerSubsystem::ShowSettings()
{
	ShowWidget(TEXT("Settings"), true, true, 0);
}

void UNohamUIManagerSubsystem::ShowPauseMenu()
{
	ShowWidget(TEXT("PauseMenu"), true, true, 10);
}

void UNohamUIManagerSubsystem::ShowHUD()
{
	ShowWidget(TEXT("HUD"), false, false, -10);
}

void UNohamUIManagerSubsystem::ReturnToGame()
{
	HideAllWidgets(false);
	SetInputModeGameOnly(true);
}

APlayerController* UNohamUIManagerSubsystem::GetPlayerController() const
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return nullptr;
	}

	return World->GetFirstPlayerController();
}

void UNohamUIManagerSubsystem::LogUIAction(const FString& Action, const FString& WidgetName) const
{
	if (WidgetName.IsEmpty())
	{
		UE_LOG(LogTemp, Log, TEXT("[UI Manager] %s"), *Action);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[UI Manager] %s: %s"), *Action, *WidgetName);
	}
}
