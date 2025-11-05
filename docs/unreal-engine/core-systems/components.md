# Unreal Engine 5 - Component System (C++)

## Overview
Components are modular pieces of functionality that can be attached to Actors. They provide reusable functionality like rendering, physics, audio, etc.

## Core Component Types

### UActorComponent
Base class for all components.

### USceneComponent  
Components that have a transform (position, rotation, scale).

### UPrimitiveComponent
Components that have collision and can be rendered.

## Common Components

### UStaticMeshComponent
For rendering static meshes.

```cpp
UPROPERTY(VisibleAnywhere)
UStaticMeshComponent* MeshComponent;

// In constructor:
MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
MeshComponent->SetupAttachment(RootComponent);
```

### USkeletalMeshComponent  
For rendering animated meshes.

```cpp
UPROPERTY(VisibleAnywhere)
USkeletalMeshComponent* WeaponMesh;

// In constructor:
WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
RootComponent = WeaponMesh;
```

## Custom Component Example

```cpp
#include "Components/ActorComponent.h"

UCLASS()
class MYGAME_API UPhysicsInteractionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPhysicsInteractionComponent()
    {
        PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
    }

    UFUNCTION(BlueprintCallable)
    void GrabObject(UPrimitiveComponent* Component, FName BoneName, FVector GrabLocation)
    {
        if (!Component || !PhysicsHandle) return;

        // Configure physics handle
        PhysicsHandle->SetInterpolationSpeed(50.0f);
        PhysicsHandle->SetLinearDamping(200.0f);
        PhysicsHandle->SetAngularDamping(500.0f);

        // Grab the component
        PhysicsHandle->GrabComponentAtLocationWithRotation(
            Component, BoneName, GrabLocation, FRotator::ZeroRotator
        );

        bIsGrabbing = true;
    }

    UFUNCTION(BlueprintCallable)
    void ReleaseObject()
    {
        if (PhysicsHandle && bIsGrabbing)
        {
            PhysicsHandle->ReleaseComponent();
            bIsGrabbing = false;
        }
    }

private:
    UPROPERTY()
    UPhysicsHandleComponent* PhysicsHandle;

    bool bIsGrabbing = false;
};
```

## Component Lifecycle
- Constructor: Create subcomponents
- `BeginPlay()`: Initialize component
- `TickComponent()`: Per-frame updates (if enabled)
- `EndPlay()`: Cleanup

## Required Headers
```cpp
#include "Components/ActorComponent.h"
#include "Components/SceneComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
```

## Best Practices
- Use components to separate concerns
- Prefer composition over inheritance
- Create subcomponents only in constructor
- Use `SetupAttachment()` to establish component hierarchy
- Enable ticking only when necessary