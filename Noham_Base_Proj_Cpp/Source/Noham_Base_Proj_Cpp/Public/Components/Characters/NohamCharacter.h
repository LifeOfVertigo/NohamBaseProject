// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "NohamCharacter.generated.h"

class UInputComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class UNohamSettingsSubsystem;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogNohamCharacter, Log, All);

/**
 * Base character class for Noham project
 * Implements first-person character with Enhanced Input system
 * Features smooth movement, configurable sensitivity, and camera collision
 *
 * References:
 * - docs/unreal-engine/core-systems/actors.md - Actor patterns
 * - docs/unreal-engine/gameplay/input.md - Enhanced Input system
 */
UCLASS(config=Game)
class NOHAM_BASE_PROJ_CPP_API ANohamCharacter : public ACharacter
{
	GENERATED_BODY()

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Crouch Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	/** Aim Down Sights Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AimAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

public:
	ANohamCharacter();

protected:
	// AActor interface
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	// End of AActor interface

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for crouch input */
	void StartCrouch(const FInputActionValue& Value);
	void StopCrouch(const FInputActionValue& Value);

	/** Called for sprint input */
	void StartSprint(const FInputActionValue& Value);
	void StopSprint(const FInputActionValue& Value);

	/** Called for aim down sights input */
	void StartAim(const FInputActionValue& Value);
	void StopAim(const FInputActionValue& Value);

	/** Called for interact input */
	void Interact(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	// Movement configuration
	/** Base movement speed multiplier */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "0.1", ClampMax = "5.0"))
	float MovementSpeedMultiplier = 1.0f;

	/** Movement acceleration rate (units per second squared) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "100.0", ClampMax = "10000.0"))
	float MovementAcceleration = 2048.0f;

	/** Movement deceleration rate (units per second squared) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "100.0", ClampMax = "10000.0"))
	float MovementDeceleration = 2048.0f;

	// Crouch configuration
	/** Crouch transition speed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Crouch", meta = (ClampMin = "1.0", ClampMax = "20.0"))
	float CrouchTransitionSpeed = 10.0f;

	/** Crouched movement speed multiplier */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Crouch", meta = (ClampMin = "0.1", ClampMax = "1.0"))
	float CrouchSpeedMultiplier = 0.5f;

	/** Crouched capsule half height */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Crouch", meta = (ClampMin = "30.0", ClampMax = "90.0"))
	float CrouchedCapsuleHalfHeight = 48.0f;

	// Sprint configuration
	/** Sprint speed multiplier */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Sprint", meta = (ClampMin = "1.0", ClampMax = "3.0"))
	float SprintSpeedMultiplier = 1.5f;

	/** Can sprint while aiming */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Sprint")
	bool bCanSprintWhileAiming = false;

	/** Sprint stamina framework enabled (for future implementation) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Sprint")
	bool bUseStaminaSystem = false;

	// Aim Down Sights configuration
	/** ADS FOV multiplier (reduces FOV for zoom effect) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|ADS", meta = (ClampMin = "0.5", ClampMax = "1.0"))
	float ADSFOVMultiplier = 0.75f;

	/** ADS movement speed multiplier */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|ADS", meta = (ClampMin = "0.1", ClampMax = "1.0"))
	float ADSSpeedMultiplier = 0.6f;

	/** ADS transition speed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|ADS", meta = (ClampMin = "1.0", ClampMax = "30.0"))
	float ADSTransitionSpeed = 15.0f;

	// Camera configuration
	/** Enable camera collision detection and correction */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	bool bEnableCameraCollision = true;

	/** Camera collision probe radius */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (ClampMin = "1.0", ClampMax = "50.0"))
	float CameraCollisionProbeRadius = 12.0f;

	/** Camera smooth interpolation speed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (ClampMin = "1.0", ClampMax = "30.0"))
	float CameraInterpolationSpeed = 10.0f;

	// Look input smoothing
	/** Enable input smoothing for mouse look - DISABLED for responsive survival game feel */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	bool bEnableLookSmoothing = false;

	/** Look input smoothing speed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (ClampMin = "1.0", ClampMax = "30.0"))
	float LookSmoothingSpeed = 15.0f;

	// Interaction configuration
	/** Maximum interaction distance */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (ClampMin = "50.0", ClampMax = "1000.0"))
	float InteractionDistance = 300.0f;

	/** Interaction raycast radius (sphere trace) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction", meta = (ClampMin = "1.0", ClampMax = "50.0"))
	float InteractionRadius = 15.0f;

	/** Enable debug visualization for interaction traces */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Debug")
	bool bDebugInteraction = false;

private:
	// Cached settings subsystem reference
	UPROPERTY()
	UNohamSettingsSubsystem* SettingsSubsystem = nullptr;

	// Camera collision state
	FVector DesiredCameraLocation;
	FVector CurrentCameraLocation;
	bool bCameraCollisionActive = false;

	// Input smoothing state
	FVector2D CurrentLookInput;
	FVector2D SmoothedLookInput;

	// Movement state flags
	bool bWantsToSprint = false;
	bool bIsAiming = false;

	// Crouch state
	float StandingCapsuleHalfHeight = 96.0f;
	float CurrentCapsuleHalfHeight = 96.0f;
	float TargetCapsuleHalfHeight = 96.0f;

	// ADS state
	float DefaultFOV = 90.0f;
	float CurrentFOV = 90.0f;
	float TargetFOV = 90.0f;

	// Interaction state
	UPROPERTY()
	AActor* CurrentInteractable = nullptr;
	UPROPERTY()
	AActor* LastInteractable = nullptr;

	// Camera collision detection
	void UpdateCameraCollision(float DeltaTime);
	bool CheckCameraCollision(const FVector& DesiredLocation, FVector& OutSafeLocation);

	// Input processing helpers
	float GetMouseSensitivityX() const;
	float GetMouseSensitivityY() const;
	bool ShouldInvertMouseY() const;

	// Movement state update helpers
	void UpdateCrouchState(float DeltaTime);
	void UpdateAimState(float DeltaTime);
	void UpdateMovementSpeed();
	bool CanSprint() const;

	// Interaction helpers
	void UpdateInteraction();
	AActor* TraceForInteractable();
	void NotifyInteractableFocusChanged();

public:
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	/** Set movement speed multiplier (called by settings system) */
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetMovementSpeedMultiplier(float NewMultiplier);

	/** Get current movement speed multiplier */
	UFUNCTION(BlueprintPure, Category = "Movement")
	float GetMovementSpeedMultiplier() const { return MovementSpeedMultiplier; }

	/** Check if character is sprinting */
	UFUNCTION(BlueprintPure, Category = "Movement")
	bool IsSprinting() const { return bWantsToSprint && CanSprint(); }

	/** Check if character is aiming down sights */
	UFUNCTION(BlueprintPure, Category = "Movement")
	bool IsAiming() const { return bIsAiming; }

	/** Check if character is crouched */
	UFUNCTION(BlueprintPure, Category = "Movement")
	bool IsCrouched() const { return bIsCrouched; }

	/** Get current FOV (for animation or UI feedback) */
	UFUNCTION(BlueprintPure, Category = "Camera")
	float GetCurrentFOV() const { return CurrentFOV; }

	/** Get currently focused interactable actor (for UI prompts) */
	UFUNCTION(BlueprintPure, Category = "Interaction")
	AActor* GetCurrentInteractable() const { return CurrentInteractable; }

	/** Check if there's an interactable object in focus */
	UFUNCTION(BlueprintPure, Category = "Interaction")
	bool HasInteractableInFocus() const { return CurrentInteractable != nullptr; }
};
