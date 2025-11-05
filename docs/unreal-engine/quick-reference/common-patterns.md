# Unreal Engine 5 - Common C++ Patterns

## Actor Creation and Setup

### Basic Actor Template
```cpp
UCLASS()
class MYGAME_API AMyActor : public AActor
{
    GENERATED_BODY()

public:
    AMyActor()
    {
        PrimaryActorTick.bCanEverTick = true;
        
        // Create components
        RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
        MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
        MeshComponent->SetupAttachment(RootComponent);
    }

protected:
    virtual void BeginPlay() override
    {
        Super::BeginPlay();
        // Initialization code
    }

    virtual void Tick(float DeltaTime) override
    {
        Super::Tick(DeltaTime);
        // Per-frame logic
    }

private:
    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* MeshComponent;
};
```

### Component Creation Pattern
```cpp
// In constructor only
MyComponent = CreateDefaultSubobject<UMyComponent>(TEXT("MyComponent"));
MyComponent->SetupAttachment(RootComponent);

// At runtime
UMyComponent* RuntimeComponent = CreateComponentByClass(UMyComponent::StaticClass());
RuntimeComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
RuntimeComponent->RegisterComponent();
```

## Common UE5 Patterns

### Safe Object Access
```cpp
// Check validity before use
if (IsValid(MyActor))
{
    MyActor->DoSomething();
}

// Weak pointer pattern
if (WeakActorPtr.IsValid())
{
    AActor* Actor = WeakActorPtr.Get();
    Actor->DoSomething();
}
```

### Finding Actors
```cpp
// Find actor by class
AActor* FoundActor = GetWorld()->GetFirstPlayerController()->GetPawn();

// Find all actors of a class
TArray<AActor*> FoundActors;
UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMyActor::StaticClass(), FoundActors);

// Find actor with tag
AActor* TaggedActor = UGameplayStatics::GetActorOfClass(GetWorld(), AMyActor::StaticClass());
```

### Spawning Actors
```cpp
// Basic spawning
FVector SpawnLocation = GetActorLocation();
FRotator SpawnRotation = GetActorRotation();
AActor* SpawnedActor = GetWorld()->SpawnActor<AMyActor>(SpawnLocation, SpawnRotation);

// Spawning with parameters
FActorSpawnParameters SpawnParams;
SpawnParams.Owner = this;
SpawnParams.Instigator = GetInstigator();
SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

AMyActor* SpawnedActor = GetWorld()->SpawnActor<AMyActor>(
    AMyActor::StaticClass(),
    SpawnLocation,
    SpawnRotation,
    SpawnParams
);
```

## Timer Patterns

### Basic Timer Usage
```cpp
// Set timer
GetWorldTimerManager().SetTimer(
    TimerHandle,
    this,
    &AMyActor::TimerFunction,
    1.0f,  // Delay in seconds
    true   // Loop
);

// Clear timer
GetWorldTimerManager().ClearTimer(TimerHandle);

// Lambda timer
GetWorldTimerManager().SetTimer(
    TimerHandle,
    [this]()
    {
        // Timer code here
        UE_LOG(LogTemp, Log, TEXT("Timer fired"));
    },
    2.0f,
    false
);
```

### Delegate Timer
```cpp
FTimerDelegate TimerDelegate;
TimerDelegate.BindUFunction(this, FName("TimerFunction"));
GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 1.0f, true);
```

## Collision and Physics

### Line Trace
```cpp
FHitResult HitResult;
FVector Start = GetActorLocation();
FVector End = Start + GetActorForwardVector() * 1000.0f;

bool bHit = GetWorld()->LineTraceSingleByChannel(
    HitResult,
    Start,
    End,
    ECC_Visibility
);

if (bHit)
{
    AActor* HitActor = HitResult.GetActor();
    FVector HitLocation = HitResult.Location;
}
```

### Sphere Overlap
```cpp
TArray<FOverlapResult> OverlapResults;
FCollisionShape SphereShape = FCollisionShape::MakeSphere(500.0f);

bool bHasOverlap = GetWorld()->OverlapMultiByChannel(
    OverlapResults,
    GetActorLocation(),
    FQuat::Identity,
    ECC_Pawn,
    SphereShape
);

for (const FOverlapResult& Result : OverlapResults)
{
    if (AActor* OverlappedActor = Result.GetActor())
    {
        // Process overlapped actor
    }
}
```

## Animation Patterns

### Playing Animation
```cpp
// Play animation montage
if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
{
    if (MyAnimMontage)
    {
        AnimInstance->Montage_Play(MyAnimMontage);
    }
}

// Stop animation
if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
{
    AnimInstance->Montage_Stop(0.2f, MyAnimMontage);
}
```

### Animation Callbacks
```cpp
// Bind to montage events
if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
{
    FOnMontageEnded MontageEndedDelegate;
    MontageEndedDelegate.BindUFunction(this, FName("OnMontageEnded"));
    AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, MyAnimMontage);
}

UFUNCTION()
void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    // Animation finished
}
```

## UI Patterns

### Creating Widgets
```cpp
// Create widget
if (MyWidgetClass)
{
    UMyWidget* Widget = CreateWidget<UMyWidget>(GetWorld(), MyWidgetClass);
    Widget->AddToViewport();
}

// Remove widget
if (MyWidget)
{
    MyWidget->RemoveFromParent();
    MyWidget = nullptr;
}
```

## Math Utilities

### Common Math Operations
```cpp
// Interpolation
FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, InterpSpeed);
FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, InterpSpeed);

// Distance calculation
float Distance = FVector::Dist(ActorA->GetActorLocation(), ActorB->GetActorLocation());

// Normalize vector
FVector Direction = (Target - Start).GetSafeNormal();

// Clamp values
float ClampedValue = FMath::Clamp(Value, MinValue, MaxValue);
```

## Event Patterns

### Delegate Declaration and Usage
```cpp
// Declare delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, NewHealth);

// In class
UPROPERTY(BlueprintAssignable)
FOnHealthChanged OnHealthChanged;

// Broadcast event
OnHealthChanged.Broadcast(CurrentHealth);

// Bind to event
MyActor->OnHealthChanged.AddDynamic(this, &AMyClass::OnActorHealthChanged);
```

### Interface Pattern
```cpp
// Interface definition
UINTERFACE(MinimalAPI)
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
    virtual void Interact_Implementation(AActor* InteractingActor) {}
};

// Implementation
class MYGAME_API AMyActor : public AActor, public IInteractable
{
    GENERATED_BODY()

public:
    virtual void Interact_Implementation(AActor* InteractingActor) override
    {
        // Interaction logic
    }
};

// Usage
if (IInteractable* InteractableActor = Cast<IInteractable>(SomeActor))
{
    InteractableActor->Execute_Interact(SomeActor, this);
}
```

## Memory Management

### Object References
```cpp
// Strong reference (prevents garbage collection)
UPROPERTY()
AActor* StrongReference;

// Weak reference (allows garbage collection)
UPROPERTY()
TWeakObjectPtr<AActor> WeakReference;

// Checking weak reference
if (WeakReference.IsValid())
{
    AActor* Actor = WeakReference.Get();
}
```

These patterns form the foundation of most UE5 C++ gameplay programming. Use them as starting points for your implementations.