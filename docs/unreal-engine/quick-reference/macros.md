# Unreal Engine 5 - Essential Macros Reference

## Core Class Macros

### UCLASS()
Marks a C++ class for UE reflection system.

```cpp
// Basic class
UCLASS()
class MYGAME_API AMyActor : public AActor
{
    GENERATED_BODY()
};

// Blueprint accessible class
UCLASS(BlueprintType, Blueprintable)
class MYGAME_API AMyCharacter : public ACharacter
{
    GENERATED_BODY()
};

// Abstract class (cannot be instantiated)
UCLASS(Abstract)
class MYGAME_API ABaseWeapon : public AActor
{
    GENERATED_BODY()
};

// Component class
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MYGAME_API UMyComponent : public UActorComponent
{
    GENERATED_BODY()
};
```

### USTRUCT()
Defines a structure for UE reflection.

```cpp
// Basic struct
USTRUCT()
struct FPlayerData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    FString PlayerName;

    UPROPERTY(EditAnywhere)
    int32 Score = 0;
};

// Blueprint accessible struct
USTRUCT(BlueprintType)
struct FWeaponStats
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Damage = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Range = 1000.0f;
};
```

### UENUM()
Defines an enumeration for UE reflection.

```cpp
// Basic enum
UENUM()
enum class EWeaponType : uint8
{
    None,
    Pistol,
    Rifle,
    Shotgun
};

// Blueprint accessible enum
UENUM(BlueprintType)
enum class EPlayerState : uint8
{
    Idle        UMETA(DisplayName = "Idle"),
    Moving      UMETA(DisplayName = "Moving"),
    Attacking   UMETA(DisplayName = "Attacking"),
    Dead        UMETA(DisplayName = "Dead")
};
```

## Property Macros

### UPROPERTY() - Editability
```cpp
// Editable anywhere (details panel and defaults)
UPROPERTY(EditAnywhere, Category = "Weapon")
float Damage = 100.0f;

// Editable only in class defaults
UPROPERTY(EditDefaultsOnly, Category = "Weapon")
TSubclassOf<AProjectile> ProjectileClass;

// Editable only on instances
UPROPERTY(EditInstanceOnly, Category = "Weapon")
FString WeaponName;

// Visible but not editable
UPROPERTY(VisibleAnywhere, Category = "Components")
UStaticMeshComponent* MeshComponent;

// Visible only in defaults
UPROPERTY(VisibleDefaultsOnly, Category = "Info")
int32 WeaponID;

// Visible only on instances
UPROPERTY(VisibleInstanceOnly, Category = "Runtime")
float CurrentAmmo;
```

### UPROPERTY() - Blueprint Access
```cpp
// Blueprint read-only
UPROPERTY(BlueprintReadOnly, Category = "Player")
float Health;

// Blueprint read/write
UPROPERTY(BlueprintReadWrite, Category = "Player")
float MaxHealth = 100.0f;

// Blueprint assignable (for delegates)
UPROPERTY(BlueprintAssignable, Category = "Events")
FOnHealthChanged OnHealthChanged;
```

### UPROPERTY() - Networking
```cpp
// Basic replication
UPROPERTY(Replicated)
float Health;

// Replication with callback
UPROPERTY(ReplicatedUsing = OnRep_Health)
float Health;

// Replication callback function
UFUNCTION()
void OnRep_Health()
{
    // Called when Health replicates
}
```

### UPROPERTY() - Metadata
```cpp
// Slider in editor
UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "100.0"))
float Percentage = 50.0f;

// Hidden in editor
UPROPERTY(meta = (EditCondition = "false"))
int32 HiddenValue;

// Conditional editing
UPROPERTY(EditAnywhere)
bool bEnableFeature = false;

UPROPERTY(EditAnywhere, meta = (EditCondition = "bEnableFeature"))
float FeatureValue = 1.0f;

// Tooltip
UPROPERTY(EditAnywhere, meta = (ToolTip = "This controls the weapon damage"))
float WeaponDamage;
```

## Function Macros

### UFUNCTION() - Blueprint Integration
```cpp
// Blueprint callable
UFUNCTION(BlueprintCallable, Category = "Weapon")
void FireWeapon();

// Blueprint implementable event (C++ declares, Blueprint implements)
UFUNCTION(BlueprintImplementableEvent, Category = "Events")
void OnWeaponFired();

// Blueprint native event (can be overridden in Blueprint)
UFUNCTION(BlueprintNativeEvent, Category = "Events")
void OnReload();
virtual void OnReload_Implementation() { /* Default implementation */ }

// Blueprint pure (no execution pins)
UFUNCTION(BlueprintPure, Category = "Weapon")
float GetDamage() const { return Damage; }
```

### UFUNCTION() - Networking RPCs
```cpp
// Server RPC - reliable
UFUNCTION(Server, Reliable, WithValidation)
void ServerFireWeapon(FVector Location, FVector Direction);

void ServerFireWeapon_Implementation(FVector Location, FVector Direction)
{
    // Server implementation
}

bool ServerFireWeapon_Validate(FVector Location, FVector Direction)
{
    // Validation logic
    return true;
}

// Client RPC
UFUNCTION(Client, Reliable)
void ClientPlayEffect();

void ClientPlayEffect_Implementation()
{
    // Client implementation
}

// Multicast RPC
UFUNCTION(NetMulticast, Unreliable)
void MulticastPlaySound();

void MulticastPlaySound_Implementation()
{
    // Implementation for all clients
}
```

### UFUNCTION() - Replication
```cpp
// Replication callback
UFUNCTION()
void OnRep_Health();

// Custom replication condition
UFUNCTION()
bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags);
```

## Interface Macros

### UINTERFACE()
```cpp
UINTERFACE(MinimalAPI, Blueprintable)
class UInteractable : public UInterface
{
    GENERATED_BODY()
};

class IInteractable
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
    void Interact(AActor* InteractingActor);
    virtual void Interact_Implementation(AActor* InteractingActor) = 0;
};
```

## Delegate Macros

### Delegate Declarations
```cpp
// Single-cast delegate
DECLARE_DELEGATE_OneParam(FOnHealthChanged, float);

// Multi-cast delegate
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerDied, APlayerCharacter*);

// Dynamic delegate (Blueprint compatible)
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnItemUsed, int32, ItemID);

// Dynamic multicast delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnScoreChanged, int32, NewScore, int32, OldScore);
```

## Logging Macros

### UE_LOG()
```cpp
// Basic logging
UE_LOG(LogTemp, Warning, TEXT("Hello World"));

// Formatted logging
UE_LOG(LogTemp, Log, TEXT("Player health: %f"), Health);
UE_LOG(LogTemp, Error, TEXT("Player %s died"), *PlayerName);

// Conditional logging
UE_CLOG(Health <= 0, LogTemp, Fatal, TEXT("Player health is zero!"));

// Custom log category
DECLARE_LOG_CATEGORY_EXTERN(LogMyGame, Log, All);

// In .cpp file
DEFINE_LOG_CATEGORY(LogMyGame);

// Usage
UE_LOG(LogMyGame, Warning, TEXT("Custom log message"));
```

## Module Macros

### IMPLEMENT_MODULE()
```cpp
// Basic module implementation
IMPLEMENT_MODULE(FDefaultModuleImpl, MyGame);

// Custom module implementation
IMPLEMENT_MODULE(FMyGameModule, MyGame);
```

## Replication Macros

### GetLifetimeReplicatedProps()
```cpp
void AMyActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Basic replication
    DOREPLIFETIME(AMyActor, Health);

    // Conditional replication
    DOREPLIFETIME_CONDITION(AMyActor, OwnerOnlyData, COND_OwnerOnly);
    DOREPLIFETIME_CONDITION(AMyActor, InitialData, COND_InitialOnly);
    DOREPLIFETIME_CONDITION(AMyActor, SkipOwnerData, COND_SkipOwner);
}
```

## Common Macro Patterns

### Required Boilerplate
```cpp
// Every UCLASS needs this
UCLASS()
class MYGAME_API AMyClass : public AParentClass
{
    GENERATED_BODY()
    
    // Constructor
    AMyClass();
    
    // Virtual functions
    virtual void BeginPlay() override;
};
```

### Editor-Only Code
```cpp
#if WITH_EDITOR
    // Editor-only code
    UPROPERTY(EditAnywhere, Category = "Debug")
    bool bDebugMode = false;
#endif
```

These macros are essential for integrating C++ classes with Unreal Engine's reflection system, Blueprint system, and networking architecture.