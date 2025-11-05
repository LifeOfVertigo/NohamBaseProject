# Unreal Engine 5 - UObject System (C++)

## Overview
UObject is the base class for all Unreal Engine objects. It provides garbage collection, reflection, serialization, and editor integration.

## Core UObject System

### UObject Base Class
```cpp
#include "UObject/Object.h"

UCLASS()
class MYGAME_API UMyGameObject : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category = "Game")
    float Health = 100.0f;

    UFUNCTION(BlueprintCallable, Category = "Game")
    void TakeDamage(float DamageAmount)
    {
        Health = FMath::Max(0.0f, Health - DamageAmount);
        if (Health <= 0.0f)
        {
            OnDeath();
        }
    }

protected:
    virtual void OnDeath()
    {
        UE_LOG(LogTemp, Warning, TEXT("Object destroyed"));
    }
};
```

## Key Macros

### UCLASS()
Marks a class as part of the UE reflection system.

```cpp
UCLASS(BlueprintType, Blueprintable)
class MYGAME_API AMyActor : public AActor
{
    GENERATED_BODY()
    // ...
};
```

### UPROPERTY()
Exposes variables to the editor and reflection system.

```cpp
// Visible in editor details panel
UPROPERTY(EditAnywhere, Category = "Weapon")
float WeaponDamage = 20.0f;

// Replicated for networking
UPROPERTY(ReplicatedUsing = OnRep_CurrentAmmo)
int32 CurrentAmmo = 30;

// Visible but not editable
UPROPERTY(VisibleAnywhere)
UStaticMeshComponent* MeshComponent;

// Blueprint assignable event
UPROPERTY(BlueprintAssignable, Category = "Events")
FOnAmmoChangedSignature OnAmmoChanged;
```

### UFUNCTION()
Exposes functions to Blueprints and RPC system.

```cpp
// Blueprint callable
UFUNCTION(BlueprintCallable, Category = "Weapon")
void FireWeapon();

// Server RPC
UFUNCTION(Server, Reliable, WithValidation)
void ServerFire(FVector_NetQuantize MuzzleLocation, FVector_NetQuantize Direction);

// Multicast RPC
UFUNCTION(NetMulticast, Unreliable)
void MulticastPlayFireEffects(FVector_NetQuantize MuzzleLocation);

// Replication callback
UFUNCTION()
void OnRep_CurrentAmmo();
```

## Common UPROPERTY Specifiers

### Editability
- `EditAnywhere` - Editable everywhere
- `EditDefaultsOnly` - Only in class defaults
- `EditInstanceOnly` - Only on instances
- `VisibleAnywhere` - Visible but not editable
- `BlueprintReadOnly` - Read-only in Blueprint
- `BlueprintReadWrite` - Read/write in Blueprint

### Categories
```cpp
UPROPERTY(EditAnywhere, Category = "Weapon|Stats")
float Damage;

UPROPERTY(EditAnywhere, Category = "Weapon|Effects") 
UParticleSystem* MuzzleFlash;
```

### Replication
```cpp
UPROPERTY(Replicated)
int32 TeamId;

UPROPERTY(ReplicatedUsing = OnRep_Health)
float Health;
```

## UFUNCTION Specifiers

### Blueprint Integration
- `BlueprintCallable` - Can be called from Blueprint
- `BlueprintImplementableEvent` - Blueprint implements this function
- `BlueprintNativeEvent` - Can be overridden in Blueprint

### Networking
- `Server, Reliable` - Reliable server RPC
- `Server, Unreliable` - Unreliable server RPC  
- `Client, Reliable` - Reliable client RPC
- `NetMulticast, Reliable` - Reliable multicast RPC

### Validation
```cpp
UFUNCTION(Server, Reliable, WithValidation)
void ServerAction(float Value);

bool ServerAction_Validate(float Value)
{
    return Value >= 0.0f && Value <= 100.0f;
}

void ServerAction_Implementation(float Value)
{
    // Server implementation
}
```

## Required Headers
```cpp
#include "UObject/Object.h"
#include "UObject/Package.h" 
#include "Engine/Engine.h"
```

## Best Practices
- Always use `GENERATED_BODY()` in UCLASS
- Organize properties with meaningful categories
- Use const-correctness with UFUNCTIONs
- Prefer `TWeakObjectPtr` for object references that might become invalid
- Use `UPROPERTY()` for garbage collection safety