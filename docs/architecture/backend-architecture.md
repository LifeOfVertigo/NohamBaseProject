# Backend Architecture

The backend consists of **UE5 C++ Subsystems** and **Blueprint integration layers** that handle core game logic, settings management, and communication with NEON frontend components.

### Service Architecture

#### UE5 Subsystem Organization

```cpp
// Core backend subsystems following UE5 patterns
Source/NohamBaseProject/
├── Subsystems/
│   ├── Settings/
│   │   ├── NohamGameUserSettings.h/.cpp          # Extended UGameUserSettings
│   │   ├── SettingsSubsystem.h/.cpp              # Settings management
│   │   └── SettingsValidationLibrary.h/.cpp      # Validation utilities
│   ├── Input/
│   │   ├── InputManagerSubsystem.h/.cpp          # Multi-platform input detection
│   │   ├── InputMappingComponent.h/.cpp          # Dynamic key rebinding
│   │   └── InputPromptLibrary.h/.cpp             # UI prompt generation
│   ├── Platform/
│   │   ├── ISteamPlatformInterface.h             # Abstract Steam interface
│   │   ├── SteamPlatformFactory.h/.cpp           # Plugin instantiation
│   │   └── MockSteamPlatform.h/.cpp              # Development mock
│   ├── Developer/
│   │   ├── DeveloperToolsSubsystem.h/.cpp        # Debug utilities
│   │   ├── PerformanceMonitor.h/.cpp             # Runtime metrics
│   │   └── ConsoleCommandLibrary.h/.cpp          # Command execution
│   ├── NEON/
│   │   ├── NEONBridgeSubsystem.h/.cpp            # UE5 ↔ NEON communication
│   │   ├── NEONWidgetManager.h/.cpp              # Widget lifecycle management
│   │   └── NEONEventDispatcher.h/.cpp            # Event batching and routing
│   └── Core/
│       ├── GameModeSubsystem.h/.cpp              # Game state management
│       ├── AchievementSubsystem.h/.cpp           # Achievement logic
│       └── SaveGameSubsystem.h/.cpp              # Save/load operations
└── Components/
    ├── Characters/
    │   ├── NohamCharacterController.h/.cpp       # FPS movement
    │   ├── InteractionComponent.h/.cpp           # Object interaction
    │   └── HealthComponent.h/.cpp                # Health/damage system
    └── UI/
        ├── NEONWidget.h/.cpp                     # Base NEON widget class
        └── HUDUpdateComponent.h/.cpp             # Real-time HUD data
```

### Authentication and Authorization

#### Steam Platform Integration

```cpp
// ISteamPlatformInterface.h - Abstract interface for modular Steam integration
class NOHAMBASEPROJECT_API ISteamPlatformInterface
{
public:
    virtual ~ISteamPlatformInterface() = default;

    // Authentication
    virtual bool IsUserLoggedIn() const = 0;
    virtual FString GetUserID() const = 0;
    virtual FString GetUserName() const = 0;
    
    // Achievements
    virtual bool UnlockAchievement(const FString& AchievementID) = 0;
    virtual bool IsAchievementUnlocked(const FString& AchievementID) const = 0;
    virtual TArray<FString> GetUnlockedAchievements() const = 0;
    
    // Cloud Storage
    virtual bool SaveToCloud(const FString& FileName, const TArray<uint8>& Data) = 0;
    virtual bool LoadFromCloud(const FString& FileName, TArray<uint8>& OutData) = 0;
    virtual bool DeleteFromCloud(const FString& FileName) = 0;
    
    // Input Detection
    virtual bool GetConnectedControllers(TArray<FString>& OutControllers) const = 0;
    virtual bool IsSteamDeck() const = 0;
    virtual bool IsSteamDeckDocked() const = 0;
    
    // Platform Features
    virtual bool OpenSteamOverlay(const FString& Page = TEXT("")) = 0;
    virtual FString GetUserLanguage() const = 0;
    virtual bool IsOnline() const = 0;
};
```
