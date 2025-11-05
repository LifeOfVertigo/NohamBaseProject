# Unreal Engine 5 - Containers and Data Types (C++)

## Overview
Unreal Engine provides its own container types that integrate with the UObject system, garbage collection, and reflection.

## Core Data Types

### Basic Types
```cpp
// Unreal Engine specific types
int32 MyInteger = 100;        // 32-bit signed integer
float MyFloat = 3.14f;        // 32-bit floating point
FString MyString = TEXT("Hello"); // Unreal's string type
bool bMyBoolean = true;       // Boolean (note the 'b' prefix convention)

// Use TEXT() macro for string literals
FString PlayerName = TEXT("Player1");
FString LogMessage = FString::Printf(TEXT("Player health: %d"), Health);
```

## Container Types

### TArray (Dynamic Array)
```cpp
#include "Containers/Array.h"

// Declaration and initialization
TArray<FString> PlayerNames;
TArray<int32> Scores = {100, 200, 300};

// Adding elements
PlayerNames.Add(TEXT("Player1"));
PlayerNames.Emplace(TEXT("Player2"));
PlayerNames.Append({TEXT("Player3"), TEXT("Player4")});

// Accessing elements
FString FirstPlayer = PlayerNames[0];
FString* PlayerPtr = PlayerNames.FindByPredicate([](const FString& Name) {
    return Name.Contains(TEXT("Player1"));
});

// Iterating
for (const FString& Name : PlayerNames)
{
    UE_LOG(LogTemp, Log, TEXT("Player: %s"), *Name);
}

// Removing elements
PlayerNames.Remove(TEXT("Player2"));
PlayerNames.RemoveAt(0);
PlayerNames.Empty(); // Clear all elements
```

### TMap (Hash Map/Dictionary)
```cpp
#include "Containers/Map.h"

// Declaration and initialization
TMap<int32, FVector> PlayerPositions;
TMap<FString, int32> PlayerScores;

// Adding elements
PlayerPositions.Add(1, FVector(100.0f, 200.0f, 0.0f));
PlayerPositions.Emplace(2, FVector(-50.0f, 150.0f, 0.0f));

PlayerScores[TEXT("Alice")] = 1500;
PlayerScores[TEXT("Bob")] = 1200;

// Accessing elements
if (FVector* Position = PlayerPositions.Find(1))
{
    UE_LOG(LogTemp, Log, TEXT("Player 1 position: %s"), *Position->ToString());
}

// Iterating
for (const TPair<int32, FVector>& Pair : PlayerPositions)
{
    UE_LOG(LogTemp, Log, TEXT("Player %d at position: %s"),
           Pair.Key, *Pair.Value.ToString());
}

// Removing elements
PlayerPositions.Remove(1);
PlayerScores.Empty();
```

### TSet (Hash Set)
```cpp
#include "Containers/Set.h"

// Declaration and initialization
TSet<FString> UniqueNames;
TSet<int32> UniqueIDs = {1, 2, 3, 4, 5};

// Adding elements
UniqueNames.Add(TEXT("Player1"));
UniqueNames.Emplace(TEXT("Player2"));

// Checking existence
if (UniqueNames.Contains(TEXT("Player1")))
{
    UE_LOG(LogTemp, Log, TEXT("Player1 exists"));
}

// Iterating
for (const FString& Name : UniqueNames)
{
    UE_LOG(LogTemp, Log, TEXT("Unique name: %s"), *Name);
}
```

## String Types

### FString
```cpp
// Basic string operations
FString PlayerName = TEXT("Player");
FString FullName = PlayerName + TEXT("1");

// String formatting
FString Formatted = FString::Printf(TEXT("Health: %d/%d"), CurrentHealth, MaxHealth);

// String comparison
if (PlayerName.Equals(TEXT("Player"), ESearchCase::IgnoreCase))
{
    // Case-insensitive comparison
}

// String searching
if (PlayerName.Contains(TEXT("Play")))
{
    // Contains substring
}

// String conversion
int32 Number = FCString::Atoi(*FString(TEXT("123")));
float FloatVal = FCString::Atof(*FString(TEXT("3.14")));
```

### FName
```cpp
// Lightweight string identifier (case-insensitive, immutable)
FName SocketName = TEXT("WeaponSocket");
FName BoneName = FName(TEXT("head"));

// Comparison (fast)
if (SocketName == FName(TEXT("WeaponSocket")))
{
    // Names are equal
}
```

### FText
```cpp
// Localization-aware text
FText DisplayText = LOCTEXT("WelcomeMessage", "Welcome to the game!");
FText FormattedText = FText::Format(
    LOCTEXT("ScoreFormat", "Score: {0}"),
    FText::AsNumber(PlayerScore)
);
```

## Math Types

### Vectors
```cpp
// 3D Vector
FVector Location(100.0f, 200.0f, 50.0f);
FVector Direction = (Target - Location).GetSafeNormal();
float Distance = FVector::Dist(Location, Target);

// 2D Vector
FVector2D ScreenPosition(800.0f, 600.0f);
```

### Rotations
```cpp
// Rotator (Pitch, Yaw, Roll in degrees)
FRotator Rotation(0.0f, 90.0f, 0.0f);
FVector ForwardVector = Rotation.Vector();

// Quaternion
FQuat QuatRotation = FQuat(Rotation);
```

### Transforms
```cpp
FTransform ActorTransform;
ActorTransform.SetLocation(FVector(100.0f, 0.0f, 0.0f));
ActorTransform.SetRotation(FQuat(FRotator(0.0f, 45.0f, 0.0f)));
ActorTransform.SetScale3D(FVector(2.0f, 2.0f, 2.0f));
```

## Smart Pointers

### TWeakObjectPtr
```cpp
// Weak reference to UObject (won't prevent garbage collection)
UPROPERTY()
TWeakObjectPtr<AActor> WeakActorRef;

// Check if still valid
if (WeakActorRef.IsValid())
{
    AActor* Actor = WeakActorRef.Get();
    // Use Actor
}
```

### TSharedPtr / TWeakPtr
```cpp
// For non-UObject types
TSharedPtr<FMyData> SharedData = MakeShareable(new FMyData());
TWeakPtr<FMyData> WeakData = SharedData;

// Check weak pointer
if (TSharedPtr<FMyData> PinnedData = WeakData.Pin())
{
    // Use PinnedData
}
```

## Required Headers
```cpp
#include "CoreMinimal.h"
#include "Containers/Array.h"
#include "Containers/Map.h"
#include "Containers/Set.h"
#include "Math/Vector.h"
#include "Math/Rotator.h" 
#include "Math/Transform.h"
#include "HAL/Platform.h"
```

## Best Practices
- Use Unreal's types instead of STL for UE integration
- Prefer `TArray` over `std::vector`
- Use `TEXT()` macro for string literals
- Use `FName` for identifiers that are compared frequently
- Use `FText` for user-visible strings
- Initialize containers with expected capacity when known
- Use range-based for loops for iteration