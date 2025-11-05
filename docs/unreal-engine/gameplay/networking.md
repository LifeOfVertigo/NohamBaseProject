# Unreal Engine 5 - Networking and Replication (C++)

## Overview
Unreal Engine provides a robust networking system for multiplayer games, including property replication, Remote Procedure Calls (RPCs), and client-server architecture.

## Basic Replication Setup

### Replicated Actor
```cpp
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"

UCLASS()
class MYGAME_API AReplicatedWeapon : public AActor
{
    GENERATED_BODY()

public:
    AReplicatedWeapon()
    {
        bReplicates = true;
        bNetUseOwnerRelevancy = true;
        NetUpdateFrequency = 10.0f;
        MinNetUpdateFrequency = 5.0f;

        WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
        RootComponent = WeaponMesh;
    }

    // Define replicated properties
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
    {
        Super::GetLifetimeReplicatedProps(OutLifetimeProps);

        // Replicate to all clients
        DOREPLIFETIME(AReplicatedWeapon, CurrentAmmo);
        DOREPLIFETIME(AReplicatedWeapon, MaxAmmo);
        
        // Replicate only to owner
        DOREPLIFETIME_CONDITION(AReplicatedWeapon, OwningPlayer, COND_OwnerOnly);
        
        // Replicate only when changed
        DOREPLIFETIME_CONDITION(AReplicatedWeapon, WeaponState, COND_InitialOnly);
    }

    // Replication callback
    UFUNCTION()
    void OnRep_CurrentAmmo()
    {
        // Update UI when ammo changes
        OnAmmoChanged.Broadcast(CurrentAmmo, MaxAmmo);
    }

private:
    UPROPERTY(ReplicatedUsing = OnRep_CurrentAmmo)
    int32 CurrentAmmo = 30;

    UPROPERTY(Replicated)
    int32 MaxAmmo = 30;

    UPROPERTY(Replicated)
    APlayerState* OwningPlayer;

    UPROPERTY(Replicated)
    EWeaponState WeaponState;

    UPROPERTY(VisibleAnywhere)
    USkeletalMeshComponent* WeaponMesh;

public:
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoChangedSignature, int32, CurrentAmmo, int32, MaxAmmo);
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnAmmoChangedSignature OnAmmoChanged;
};
```

## Remote Procedure Calls (RPCs)

### Server RPC
```cpp
// Server RPC - called on client, executed on server
UFUNCTION(Server, Reliable, WithValidation)
void ServerFire(FVector_NetQuantize MuzzleLocation, FVector_NetQuantize Direction);

void ServerFire_Implementation(FVector_NetQuantize MuzzleLocation, FVector_NetQuantize Direction)
{
    if (CurrentAmmo <= 0) return;

    // Validate fire request
    if (!CanFire()) return;

    // Perform server-side hit detection
    FHitResult Hit;
    FVector EndLocation = MuzzleLocation + Direction * WeaponRange;

    GetWorld()->LineTraceSingleByChannel(Hit, MuzzleLocation, EndLocation, ECC_Visibility);

    if (Hit.bBlockingHit)
    {
        if (AActor* HitActor = Hit.GetActor())
        {
            // Apply damage
            UGameplayStatics::ApplyPointDamage(HitActor, WeaponDamage,
                                              Direction, Hit, 
                                              GetInstigatorController(),
                                              this, DamageType);
        }
    }

    CurrentAmmo--;
    LastFireTime = GetWorld()->GetTimeSeconds();

    // Multicast visual effects
    MulticastPlayFireEffects(MuzzleLocation, Hit.ImpactPoint, Hit.bBlockingHit);
}

bool ServerFire_Validate(FVector_NetQuantize MuzzleLocation, FVector_NetQuantize Direction)
{
    // Anti-cheat: validate fire rate
    float TimeSinceLastFire = GetWorld()->GetTimeSeconds() - LastFireTime;
    return TimeSinceLastFire >= FireRate * 0.9f; // 10% tolerance
}
```

### Client RPC
```cpp
// Client RPC - called on server, executed on specific client
UFUNCTION(Client, Reliable)
void ClientShowMessage(const FString& Message);

void ClientShowMessage_Implementation(const FString& Message)
{
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, Message);
    }
}
```

### Multicast RPC
```cpp
// Multicast RPC - executed on all clients
UFUNCTION(NetMulticast, Unreliable)
void MulticastPlayFireEffects(FVector_NetQuantize MuzzleLocation,
                              FVector_NetQuantize ImpactLocation,
                              bool bHitTarget);

void MulticastPlayFireEffects_Implementation(FVector_NetQuantize MuzzleLocation,
                                            FVector_NetQuantize ImpactLocation,
                                            bool bHitTarget)
{
    // Play muzzle flash
    if (MuzzleFlashEffect)
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlashEffect, MuzzleLocation);
    }

    // Play impact effect
    if (bHitTarget && ImpactEffect)
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, ImpactLocation);
    }

    // Play sound
    if (FireSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, MuzzleLocation);
    }
}
```

## Replication Conditions

### Common Replication Conditions
```cpp
virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Always replicate
    DOREPLIFETIME(AMyActor, Health);

    // Replicate only to owner
    DOREPLIFETIME_CONDITION(AMyActor, PrivateData, COND_OwnerOnly);

    // Replicate only on initial spawn
    DOREPLIFETIME_CONDITION(AMyActor, InitialData, COND_InitialOnly);

    // Skip owner (replicate to everyone except owner)
    DOREPLIFETIME_CONDITION(AMyActor, PublicData, COND_SkipOwner);

    // Custom condition
    DOREPLIFETIME_CONDITION(AMyActor, ConditionalData, COND_Custom);
}

bool AMyActor::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
    bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

    // Custom replication condition
    if (SomeCondition)
    {
        bWroteSomething |= Channel->ReplicateSubobject(MyComponent, *Bunch, *RepFlags);
    }

    return bWroteSomething;
}
```

## Network Authority

### Authority Checks
```cpp
void AMyActor::SomeFunction()
{
    // Check if this is the server/authority
    if (HasAuthority())
    {
        // Server-side logic
        PerformAuthorativeAction();
    }
    else
    {
        // Client-side logic or request to server
        ServerPerformAction();
    }
}

void AMyActor::AuthorityOnlyFunction()
{
    // Only execute on server
    if (!HasAuthority()) return;

    // Authority logic here
}
```

### Role Checking
```cpp
void AMyActor::CheckNetworkRole()
{
    ENetRole Role = GetLocalRole();
    
    switch (Role)
    {
        case ROLE_None:
            // No networking role
            break;
        case ROLE_SimulatedProxy:
            // Simulated on client
            break;
        case ROLE_AutonomousProxy:
            // Owned by client (player character)
            break;
        case ROLE_Authority:
            // Server authority
            break;
    }
}
```

## Network Optimization

### Network Quantization
```cpp
// Use quantized types for network efficiency
UFUNCTION(Server, Reliable)
void ServerUpdateLocation(FVector_NetQuantize100 NewLocation);

UFUNCTION(NetMulticast, Unreliable)
void MulticastUpdateRotation(FRotator_NetQuantize Rotation);
```

### Conditional Replication
```cpp
class MYGAME_API AOptimizedActor : public AActor
{
    GENERATED_BODY()

public:
    virtual void PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker) override
    {
        Super::PreReplication(ChangedPropertyTracker);

        // Only replicate health if it changed significantly
        DOREPLIFETIME_ACTIVE_OVERRIDE(AOptimizedActor, Health, 
                                      FMath::Abs(Health - LastReplicatedHealth) > 5.0f);
        
        if (FMath::Abs(Health - LastReplicatedHealth) > 5.0f)
        {
            LastReplicatedHealth = Health;
        }
    }

private:
    UPROPERTY(Replicated)
    float Health;

    float LastReplicatedHealth;
};
```

## Network Troubleshooting

### Network Logging
```cpp
// Enable network logging in code
void AMyActor::EnableNetworkLogging()
{
    UE_LOG(LogNet, Warning, TEXT("Network event occurred"));
    
    // Log replication
    UE_LOG(LogNetTraffic, Log, TEXT("Replicating property: %s"), TEXT("Health"));
}
```

## Required Headers
```cpp
#include "Net/UnrealNetwork.h"
#include "Engine/ActorChannel.h"
#include "GameFramework/PlayerState.h"
#include "Net/Core/PushModel/PushModel.h"
#include "Engine/NetConnection.h"
```

## Best Practices
- Only replicate necessary data
- Use replication conditions to optimize bandwidth
- Validate all server RPCs to prevent cheating
- Use unreliable RPCs for cosmetic effects
- Implement proper authority checks
- Use quantized types for position/rotation data
- Test with simulated network conditions
- Monitor bandwidth usage in shipping builds