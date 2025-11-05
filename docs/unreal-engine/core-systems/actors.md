# Unreal Engine 5 - Actors System (C++)

## Overview
Actors are the fundamental objects in UE5 that can be placed in the world. All gameplay objects inherit from `AActor`.

## Core Actor Classes

### AActor
Base class for all actors that can be placed in the world.

```cpp
#include "GameFramework/Actor.h"

UCLASS()
class MYGAME_API AInteractiveActor : public AActor
{
    GENERATED_BODY()

public:
    AInteractiveActor()
    {
        PrimaryActorTick.bCanEverTick = true;

        // Create root component
        RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

        // Add mesh component
        MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
        MeshComponent->SetupAttachment(RootComponent);
    }

    virtual void Tick(float DeltaTime) override
    {
        Super::Tick(DeltaTime);

        // Rotate actor
        FRotator NewRotation = GetActorRotation();
        NewRotation.Yaw += DeltaTime * RotationSpeed;
        SetActorRotation(NewRotation);
    }

    UFUNCTION(BlueprintCallable)
    void OnPlayerInteract(APlayerController* Player)
    {
        if (Player)
        {
            UE_LOG(LogTemp, Log, TEXT("Player %s interacted with %s"),
                   *Player->GetName(), *GetName());
            bIsActivated = !bIsActivated;
        }
    }

private:
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(EditAnywhere, Category = "Interaction")
    float RotationSpeed = 90.0f;

    bool bIsActivated = false;
};
```

### Key Actor Methods
- `BeginPlay()` - Called when actor starts play
- `EndPlay()` - Called when actor is destroyed
- `Tick()` - Called every frame (if enabled)
- `GetActorLocation()` / `SetActorLocation()` - Position management
- `GetActorRotation()` / `SetActorRotation()` - Rotation management

## Required Headers
```cpp
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
```

## Common Patterns

### Creating Components in Constructor
```cpp
// Always create components in constructor
MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
MeshComponent->SetupAttachment(RootComponent);
```

### Actor Spawning
```cpp
FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.0f;
FActorSpawnParameters SpawnParams;
SpawnParams.Owner = this;
SpawnParams.Instigator = Cast<APawn>(this);

GetWorld()->SpawnActor<AMyActor>(ActorClass, SpawnLocation, GetActorRotation(), SpawnParams);
```

## Best Practices
- Always call `Super::` for overridden functions
- Use `CreateDefaultSubobject` only in constructors
- Set `PrimaryActorTick.bCanEverTick = false` if Tick is not needed
- Use `UPROPERTY()` for variables that should be visible in editor or for replication