# Unreal Engine 5 - Module System (C++)

## Overview
Unreal Engine uses a modular architecture where code is organized into modules. Each module is a separate DLL/shared library that can be loaded and unloaded at runtime.

## Basic Module Implementation

### Simple Module (Default)
```cpp
// Private/GameplayModule.cpp
#include "Modules/ModuleManager.h"

// NOTE: This macro handles the default implementation for a module setup
IMPLEMENT_MODULE(FDefaultModuleImpl, Gameplay);
```

### Custom Module Class
```cpp
// Public/GameplayModule.h
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FGameplayModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
```

```cpp
// Private/GameplayModule.cpp
#include "GameplayModule.h"

#define LOCTEXT_NAMESPACE "FGameplayModule"

void FGameplayModule::StartupModule()
{
    // This code will execute after your module is loaded into memory
    UE_LOG(LogTemp, Log, TEXT("Gameplay module started"));
}

void FGameplayModule::ShutdownModule()
{
    // This function may be called during shutdown to clean up your module
    UE_LOG(LogTemp, Log, TEXT("Gameplay module shutdown"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGameplayModule, Gameplay)
```

## Module Build Configuration

### .Build.cs File Structure
```csharp
// Source/MyGame/MyGame.Build.cs
using UnrealBuildTool;

public class MyGame : ModuleRules
{
    public MyGame(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject", 
            "Engine",
            "InputCore"
        });

        PrivateDependencyModuleNames.AddRange(new string[] {
            "Slate",
            "SlateCore"
        });
    }
}
```

### Vehicle Module Example
```csharp
// MyVehicleModule.Build.cs
using UnrealBuildTool;

public class MyVehicleModule : ModuleRules
{
    public MyVehicleModule(ReadOnlyTargetRules Target) : base(Target)
    {
        Type = ModuleType.Shared;

        PublicDependencyModuleNames.AddRange(new string[] { 
            "Core", 
            "CoreUObject", 
            "Engine", 
            "InputCore" 
        });

        // Add dependencies for Chaos Vehicle System
        PrivateDependencyModuleNames.AddRange(new string[] { 
            "ChaosVehicles" 
        });

        // Add include paths for public headers
        PublicIncludePaths.AddRange(new string[] { 
            "MyVehicleModule/Public" 
        });
        
        PrivateIncludePaths.AddRange(new string[] { 
            "MyVehicleModule/Private" 
        });
    }
}
```

## Module Directory Structure
```
Source/
├── MyGame/
│   ├── Public/
│   │   ├── MyGame.h
│   │   └── MyGameCharacter.h
│   ├── Private/
│   │   ├── MyGame.cpp
│   │   └── MyGameCharacter.cpp
│   └── MyGame.Build.cs
└── MyVehicleModule/
    ├── Public/
    │   └── MyVehicleMovementComponent.h
    ├── Private/
    │   └── MyVehicleMovementComponent.cpp
    └── MyVehicleModule.Build.cs
```

## Common Module Dependencies

### Core Modules
```csharp
PublicDependencyModuleNames.AddRange(new string[] {
    "Core",           // Basic types, containers, math
    "CoreUObject",    // UObject system
    "Engine",         // Main engine functionality
    "InputCore",      // Input handling
});
```

### Additional Modules
```csharp
PrivateDependencyModuleNames.AddRange(new string[] {
    "Slate",          // UI framework
    "SlateCore",      // UI core
    "UnrealEd",       // Editor functionality
    "ToolMenus",      // Editor menus
    "EditorStyle",    // Editor styling
    "EditorWidgets",  // Editor widgets
    "PropertyEditor", // Property customization
    "Sequencer",      // Animation sequencer
    "MovieScene",     // Movie scenes
    "LevelSequence",  // Level sequences
    "ChaosVehicles",  // Vehicle physics
    "AIModule",       // AI functionality
    "GameplayAbilities", // Gameplay ability system
    "NavigationSystem",   // Pathfinding
    "UMG",               // UI widgets
    "HTTP",              // HTTP requests
    "Json",              // JSON parsing
    "WebBrowser",        // Web browser widget
});
```

## Module Loading
```cpp
// Check if module is loaded
if (FModuleManager::Get().IsModuleLoaded("MyModule"))
{
    // Module is loaded
}

// Load module dynamically
FModuleManager::LoadModuleChecked<IMyModuleInterface>("MyModule");

// Get module interface
IMyModuleInterface& MyModule = FModuleManager::GetModuleChecked<IMyModuleInterface>("MyModule");
```

## Best Practices
- Keep public dependencies minimal
- Use private dependencies for implementation details
- Organize related functionality into logical modules
- Use forward declarations in headers when possible
- Include paths should be relative to module root
- Always implement proper startup/shutdown logic