# Unreal Engine 5 - Enhanced Input System (C++)

## Overview
Enhanced Input provides a more flexible and powerful input handling system compared to the legacy input system.

## Basic Setup

### Player Character Input Setup
```cpp
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

UCLASS()
class MYGAME_API AMyPlayerCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override
    {
        Super::SetupPlayerInputComponent(PlayerInputComponent);

        if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
        {
            // Bind movement actions
            EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyPlayerCharacter::Move);
            EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyPlayerCharacter::Look);
            EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
            EnhancedInput->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AMyPlayerCharacter::Sprint);
        }
    }

protected:
    virtual void BeginPlay() override
    {
        Super::BeginPlay();

        // Add input mapping context
        if (APlayerController* PC = Cast<APlayerController>(GetController()))
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
                ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
            {
                Subsystem->AddMappingContext(InputMappingContext, 0);
            }
        }
    }

    // Input action handlers
    void Move(const FInputActionValue& Value)
    {
        FVector2D MovementVector = Value.Get<FVector2D>();

        if (Controller)
        {
            const FRotator Rotation = Controller->GetControlRotation();
            const FRotator YawRotation(0, Rotation.Yaw, 0);

            const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
            const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

            AddMovementInput(ForwardDirection, MovementVector.Y);
            AddMovementInput(RightDirection, MovementVector.X);
        }
    }

    void Look(const FInputActionValue& Value)
    {
        FVector2D LookAxisVector = Value.Get<FVector2D>();

        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y);
    }

    void Sprint(const FInputActionValue& Value)
    {
        bool bIsSprinting = Value.Get<bool>();
        GetCharacterMovement()->MaxWalkSpeed = bIsSprinting ? SprintSpeed : WalkSpeed;
    }

    // Input assets (set in Blueprint)
    UPROPERTY(EditAnywhere, Category = "Input")
    UInputMappingContext* InputMappingContext;

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* LookAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* JumpAction;

    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* SprintAction;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float WalkSpeed = 600.0f;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float SprintSpeed = 1000.0f;
};
```

## Input Action Value Types

### Getting Different Value Types
```cpp
void HandleInput(const FInputActionValue& Value)
{
    // Boolean input (buttons)
    bool bPressed = Value.Get<bool>();

    // Float input (triggers, single axis)
    float AxisValue = Value.Get<float>();

    // 2D Vector input (mouse, gamepad sticks)
    FVector2D Vector2D = Value.Get<FVector2D>();

    // 3D Vector input (motion controllers)
    FVector Vector3D = Value.Get<FVector>();
}
```

## Trigger Events

### Common Trigger Events
```cpp
// Event fires when input starts
EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &AMyCharacter::Jump);

// Event fires while input is active
EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);

// Event fires when input is completed
EnhancedInput->BindAction(InteractAction, ETriggerEvent::Completed, this, &AMyCharacter::Interact);

// Event fires when input is canceled
EnhancedInput->BindAction(AimAction, ETriggerEvent::Canceled, this, &AMyCharacter::StopAiming);

// Event fires while input is ongoing
EnhancedInput->BindAction(FireAction, ETriggerEvent::Ongoing, this, &AMyCharacter::FireWeapon);
```

## Dynamic Input Context Management

### Adding/Removing Contexts
```cpp
void AMyPlayerCharacter::SwitchToVehicleInput()
{
    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            // Remove character input context
            Subsystem->RemoveMappingContext(CharacterInputContext);
            
            // Add vehicle input context with higher priority
            Subsystem->AddMappingContext(VehicleInputContext, 1);
        }
    }
}

void AMyPlayerCharacter::SwitchToCharacterInput()
{
    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            Subsystem->RemoveMappingContext(VehicleInputContext);
            Subsystem->AddMappingContext(CharacterInputContext, 0);
        }
    }
}
```

## Legacy Input System (Still Supported)

### Basic Legacy Input
```cpp
virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Action bindings (binary input)
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMyCharacter::StartFiring);
    PlayerInputComponent->BindAction("Fire", IE_Released, this, &AMyCharacter::StopFiring);

    // Axis bindings (analog input)
    PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);
    PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

void AMyCharacter::MoveForward(float Value)
{
    if (Value != 0.0f)
    {
        AddMovementInput(GetActorForwardVector(), Value);
    }
}

void AMyCharacter::MoveRight(float Value)
{
    if (Value != 0.0f)
    {
        AddMovementInput(GetActorRightVector(), Value);
    }
}
```

## Required Headers
```cpp
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
```

## Best Practices
- Use Enhanced Input for new projects
- Create separate input contexts for different gameplay states
- Use appropriate trigger events for different actions
- Bind input in `SetupPlayerInputComponent()`
- Add mapping contexts in `BeginPlay()`
- Remove mapping contexts when switching states
- Use descriptive names for input actions and contexts