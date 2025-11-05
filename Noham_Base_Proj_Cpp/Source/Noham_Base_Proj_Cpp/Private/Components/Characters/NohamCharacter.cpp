// Copyright Epic Games, Inc. All Rights Reserved.

#include "Components/Characters/NohamCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Subsystems/Settings/NohamSettingsSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Components/Interaction/NohamInteractable.h"

DEFINE_LOG_CATEGORY(LogNohamCharacter);

ANohamCharacter::ANohamCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Enable ticking for camera collision and input smoothing
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Configure character movement
	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	if (MovementComp)
	{
		MovementComp->MaxWalkSpeed = 600.0f; // Base walk speed
		MovementComp->MaxAcceleration = MovementAcceleration;
		MovementComp->BrakingDecelerationWalking = MovementDeceleration;
		MovementComp->AirControl = 0.2f; // Limited air control for FPS feel
		MovementComp->bCanWalkOffLedges = true;
		MovementComp->bCanWalkOffLedgesWhenCrouching = true;

		// Configure crouch settings
		MovementComp->GetNavAgentPropertiesRef().bCanCrouch = true;
		MovementComp->SetCrouchedHalfHeight(CrouchedCapsuleHalfHeight);
	}

	// Store default capsule height
	StandingCapsuleHalfHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	CurrentCapsuleHalfHeight = StandingCapsuleHalfHeight;
	TargetCapsuleHalfHeight = StandingCapsuleHalfHeight;
}

void ANohamCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Cache settings subsystem reference
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		SettingsSubsystem = GameInstance->GetSubsystem<UNohamSettingsSubsystem>();
		if (!SettingsSubsystem)
		{
			UE_LOG(LogNohamCharacter, Warning, TEXT("Failed to get NohamSettingsSubsystem in BeginPlay"));
		}
	}

	// Initialize camera location tracking
	if (FirstPersonCameraComponent)
	{
		CurrentCameraLocation = FirstPersonCameraComponent->GetRelativeLocation();
		DesiredCameraLocation = CurrentCameraLocation;
	}

	// Initialize input smoothing
	CurrentLookInput = FVector2D::ZeroVector;
	SmoothedLookInput = FVector2D::ZeroVector;

	// Initialize FOV
	if (FirstPersonCameraComponent)
	{
		DefaultFOV = FirstPersonCameraComponent->FieldOfView;
		CurrentFOV = DefaultFOV;
		TargetFOV = DefaultFOV;
	}
}

void ANohamCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Update camera collision
	if (bEnableCameraCollision)
	{
		UpdateCameraCollision(DeltaTime);
	}

	// Update crouch state (smooth transitions)
	UpdateCrouchState(DeltaTime);

	// Update ADS state (smooth FOV transitions)
	UpdateAimState(DeltaTime);

	// Update movement speed based on current state
	UpdateMovementSpeed();

	// Update interaction detection
	UpdateInteraction();
}

void ANohamCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ANohamCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ANohamCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ANohamCharacter::Look);

		// Crouching
		if (CrouchAction)
		{
			EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ANohamCharacter::StartCrouch);
			EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ANohamCharacter::StopCrouch);
		}

		// Sprinting
		if (SprintAction)
		{
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ANohamCharacter::StartSprint);
			EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ANohamCharacter::StopSprint);
		}

		// Aiming
		if (AimAction)
		{
			EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &ANohamCharacter::StartAim);
			EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &ANohamCharacter::StopAim);
		}

		// Interacting
		if (InteractAction)
		{
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ANohamCharacter::Interact);
		}
	}
	else
	{
		UE_LOG(LogNohamCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ANohamCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// Apply movement speed multiplier
		UCharacterMovementComponent* MovementComp = GetCharacterMovement();
		if (MovementComp)
		{
			// Update max walk speed based on multiplier
			MovementComp->MaxWalkSpeed = 600.0f * MovementSpeedMultiplier;
		}

		// Add movement input (Enhanced Input handles normalization)
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ANohamCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// Get sensitivity from settings
		float SensitivityX = GetMouseSensitivityX();
		float SensitivityY = GetMouseSensitivityY();
		bool bInvertY = ShouldInvertMouseY();

		// Apply ADS sensitivity multiplier if aiming
		if (bIsAiming && SettingsSubsystem)
		{
			float ADSMultiplier = SettingsSubsystem->GetInputSettings().ADSSensitivityMultiplier;
			SensitivityX *= ADSMultiplier;
			SensitivityY *= ADSMultiplier;
		}

		// Store current input for smoothing
		CurrentLookInput = LookAxisVector;

		// Apply sensitivity scaling
		// Note: Y-axis is inverted by default for standard FPS controls (moving mouse up looks down)
		// The bInvertY setting reverses this behavior if the user wants "flight sim" style controls
		FVector2D ScaledInput = FVector2D(
			LookAxisVector.X * SensitivityX,
			LookAxisVector.Y * SensitivityY * (bInvertY ? 1.0f : -1.0f)
		);

		// Apply smoothing if enabled
		if (bEnableLookSmoothing)
		{
			float DeltaTime = GetWorld()->GetDeltaSeconds();
			SmoothedLookInput = FMath::Vector2DInterpTo(SmoothedLookInput, ScaledInput, DeltaTime, LookSmoothingSpeed);
			ScaledInput = SmoothedLookInput;
		}

		// Add yaw and pitch input to controller
		AddControllerYawInput(ScaledInput.X);
		AddControllerPitchInput(ScaledInput.Y);
	}
}

// Camera collision detection and correction
void ANohamCharacter::UpdateCameraCollision(float DeltaTime)
{
	if (!FirstPersonCameraComponent)
	{
		return;
	}

	// Get desired camera location (relative to capsule)
	DesiredCameraLocation = FVector(-10.f, 0.f, 60.f); // Default camera offset

	// Check for collision and get safe location
	FVector SafeLocation;
	if (CheckCameraCollision(DesiredCameraLocation, SafeLocation))
	{
		bCameraCollisionActive = true;
		CurrentCameraLocation = SafeLocation;
	}
	else
	{
		bCameraCollisionActive = false;
		CurrentCameraLocation = DesiredCameraLocation;
	}

	// Smoothly interpolate camera to safe location
	FVector NewLocation = FMath::VInterpTo(
		FirstPersonCameraComponent->GetRelativeLocation(),
		CurrentCameraLocation,
		DeltaTime,
		CameraInterpolationSpeed
	);

	FirstPersonCameraComponent->SetRelativeLocation(NewLocation);
}

bool ANohamCharacter::CheckCameraCollision(const FVector& DesiredLocation, FVector& OutSafeLocation)
{
	if (!GetWorld())
	{
		OutSafeLocation = DesiredLocation;
		return false;
	}

	// Get camera world location
	FVector CapsuleLocation = GetCapsuleComponent()->GetComponentLocation();
	FVector DesiredWorldLocation = CapsuleLocation + DesiredLocation;

	// Trace from capsule center to desired camera location
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = false;

	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		CapsuleLocation,
		DesiredWorldLocation,
		FQuat::Identity,
		ECC_Camera,
		FCollisionShape::MakeSphere(CameraCollisionProbeRadius),
		QueryParams
	);

	if (bHit)
	{
		// Move camera to hit location (slightly pulled back)
		FVector HitLocation = HitResult.Location;
		FVector Direction = (DesiredWorldLocation - CapsuleLocation).GetSafeNormal();
		FVector SafeWorldLocation = HitLocation - (Direction * CameraCollisionProbeRadius);
		OutSafeLocation = SafeWorldLocation - CapsuleLocation;
		return true;
	}

	OutSafeLocation = DesiredLocation;
	return false;
}

// Settings integration helpers
float ANohamCharacter::GetMouseSensitivityX() const
{
	if (SettingsSubsystem)
	{
		return SettingsSubsystem->GetInputSettings().MouseSensitivityX;
	}
	return 1.0f; // Default sensitivity
}

float ANohamCharacter::GetMouseSensitivityY() const
{
	if (SettingsSubsystem)
	{
		return SettingsSubsystem->GetInputSettings().MouseSensitivityY;
	}
	return 1.0f; // Default sensitivity
}

bool ANohamCharacter::ShouldInvertMouseY() const
{
	if (SettingsSubsystem)
	{
		return SettingsSubsystem->GetInputSettings().bInvertMouseY;
	}
	return false; // Default not inverted
}

void ANohamCharacter::SetMovementSpeedMultiplier(float NewMultiplier)
{
	MovementSpeedMultiplier = FMath::Clamp(NewMultiplier, 0.1f, 5.0f);

	// Apply immediately to movement component
	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	if (MovementComp)
	{
		MovementComp->MaxWalkSpeed = 600.0f * MovementSpeedMultiplier;
	}

	UE_LOG(LogNohamCharacter, Log, TEXT("Movement speed multiplier set to %.2f"), MovementSpeedMultiplier);
}

// Crouch input handlers
void ANohamCharacter::StartCrouch(const FInputActionValue& Value)
{
	Crouch();
	TargetCapsuleHalfHeight = CrouchedCapsuleHalfHeight;
	UE_LOG(LogNohamCharacter, Verbose, TEXT("Start crouch"));
}

void ANohamCharacter::StopCrouch(const FInputActionValue& Value)
{
	UnCrouch();
	TargetCapsuleHalfHeight = StandingCapsuleHalfHeight;
	UE_LOG(LogNohamCharacter, Verbose, TEXT("Stop crouch"));
}

// Sprint input handlers
void ANohamCharacter::StartSprint(const FInputActionValue& Value)
{
	bWantsToSprint = true;
	UE_LOG(LogNohamCharacter, Verbose, TEXT("Start sprint"));
}

void ANohamCharacter::StopSprint(const FInputActionValue& Value)
{
	bWantsToSprint = false;
	UE_LOG(LogNohamCharacter, Verbose, TEXT("Stop sprint"));
}

// Aim input handlers
void ANohamCharacter::StartAim(const FInputActionValue& Value)
{
	bIsAiming = true;
	TargetFOV = DefaultFOV * ADSFOVMultiplier;
	UE_LOG(LogNohamCharacter, Verbose, TEXT("Start aiming"));
}

void ANohamCharacter::StopAim(const FInputActionValue& Value)
{
	bIsAiming = false;
	TargetFOV = DefaultFOV;
	UE_LOG(LogNohamCharacter, Verbose, TEXT("Stop aiming"));
}

// Crouch state update
void ANohamCharacter::UpdateCrouchState(float DeltaTime)
{
	// Smoothly interpolate capsule height
	CurrentCapsuleHalfHeight = FMath::FInterpTo(
		CurrentCapsuleHalfHeight,
		TargetCapsuleHalfHeight,
		DeltaTime,
		CrouchTransitionSpeed
	);

	// Note: UE5's Crouch()/UnCrouch() handles the actual capsule resizing
	// This is mainly for tracking state and potential animation
}

// ADS state update
void ANohamCharacter::UpdateAimState(float DeltaTime)
{
	if (!FirstPersonCameraComponent)
	{
		return;
	}

	// Smoothly interpolate FOV
	CurrentFOV = FMath::FInterpTo(
		CurrentFOV,
		TargetFOV,
		DeltaTime,
		ADSTransitionSpeed
	);

	// Apply FOV to camera
	FirstPersonCameraComponent->SetFieldOfView(CurrentFOV);

	// Get ADS sensitivity multiplier from settings if available
	if (bIsAiming && SettingsSubsystem)
	{
		// The Look() function will use ADSSensitivityMultiplier from settings
		// when bIsAiming is true (implementation in Look function if needed)
	}
}

// Movement speed update based on current state
void ANohamCharacter::UpdateMovementSpeed()
{
	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	if (!MovementComp)
	{
		return;
	}

	// Calculate final speed multiplier based on state
	float FinalMultiplier = MovementSpeedMultiplier;

	// Apply crouch speed reduction
	if (bIsCrouched)
	{
		FinalMultiplier *= CrouchSpeedMultiplier;
	}

	// Apply ADS speed reduction
	if (bIsAiming)
	{
		FinalMultiplier *= ADSSpeedMultiplier;
	}

	// Apply sprint speed boost (if sprinting and conditions allow)
	if (IsSprinting())
	{
		FinalMultiplier *= SprintSpeedMultiplier;
	}

	// Apply to movement component
	MovementComp->MaxWalkSpeed = 600.0f * FinalMultiplier;
}

// Check if character can sprint
bool ANohamCharacter::CanSprint() const
{
	// Can't sprint while crouched
	if (bIsCrouched)
	{
		return false;
	}

	// Can't sprint while aiming (unless specifically allowed)
	if (bIsAiming && !bCanSprintWhileAiming)
	{
		return false;
	}

	// Can't sprint in the air
	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	if (MovementComp && MovementComp->IsFalling())
	{
		return false;
	}

	// Future: Add stamina check here if bUseStaminaSystem is true
	// if (bUseStaminaSystem && CurrentStamina <= 0.0f)
	// {
	//     return false;
	// }

	return true;
}

// Interaction input handler
void ANohamCharacter::Interact(const FInputActionValue& Value)
{
	if (!CurrentInteractable)
	{
		UE_LOG(LogNohamCharacter, Verbose, TEXT("Interact pressed but no interactable in focus"));
		return;
	}

	// Check if the actor implements the interactable interface
	if (CurrentInteractable->Implements<UNohamInteractable>())
	{
		// Call the OnInteract function
		bool bSuccess = INohamInteractable::Execute_OnInteract(CurrentInteractable, this);
		if (bSuccess)
		{
			UE_LOG(LogNohamCharacter, Log, TEXT("Successfully interacted with %s"), *CurrentInteractable->GetName());
		}
		else
		{
			UE_LOG(LogNohamCharacter, Warning, TEXT("Interaction with %s failed"), *CurrentInteractable->GetName());
		}
	}
}

// Update interaction state
void ANohamCharacter::UpdateInteraction()
{
	// Trace for interactable object
	AActor* NewInteractable = TraceForInteractable();

	// Update current interactable
	LastInteractable = CurrentInteractable;
	CurrentInteractable = NewInteractable;

	// Notify focus changes
	NotifyInteractableFocusChanged();
}

// Trace for interactable objects
AActor* ANohamCharacter::TraceForInteractable()
{
	if (!FirstPersonCameraComponent)
	{
		return nullptr;
	}

	// Get camera location and forward vector
	FVector Start = FirstPersonCameraComponent->GetComponentLocation();
	FVector ForwardVector = FirstPersonCameraComponent->GetForwardVector();
	FVector End = Start + (ForwardVector * InteractionDistance);

	// Perform sphere trace
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = false;

	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		Start,
		End,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeSphere(InteractionRadius),
		QueryParams
	);

	// Debug visualization
	if (bDebugInteraction)
	{
		FColor DebugColor = bHit ? FColor::Green : FColor::Red;
		DrawDebugLine(GetWorld(), Start, End, DebugColor, false, -1.0f, 0, 2.0f);
		if (bHit)
		{
			DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, InteractionRadius, 12, FColor::Green, false, -1.0f);
		}
	}

	// Check if hit actor implements interactable interface
	if (bHit && HitResult.GetActor())
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor->Implements<UNohamInteractable>())
		{
			// Get interaction data to check if we can interact
			FNohamInteractionData InteractionData = INohamInteractable::Execute_GetInteractionData(HitActor, this);
			if (InteractionData.bCanInteract)
			{
				return HitActor;
			}
		}
	}

	return nullptr;
}

// Notify focus changes
void ANohamCharacter::NotifyInteractableFocusChanged()
{
	// Lost focus on previous interactable
	if (LastInteractable != nullptr && LastInteractable != CurrentInteractable)
	{
		if (LastInteractable->Implements<UNohamInteractable>())
		{
			INohamInteractable::Execute_OnInteractionFocusLost(LastInteractable, this);
			UE_LOG(LogNohamCharacter, Verbose, TEXT("Lost focus on %s"), *LastInteractable->GetName());
		}
	}

	// Gained focus on new interactable
	if (CurrentInteractable != nullptr && CurrentInteractable != LastInteractable)
	{
		if (CurrentInteractable->Implements<UNohamInteractable>())
		{
			INohamInteractable::Execute_OnInteractionFocusGained(CurrentInteractable, this);
			UE_LOG(LogNohamCharacter, Verbose, TEXT("Gained focus on %s"), *CurrentInteractable->GetName());
		}
	}
}
