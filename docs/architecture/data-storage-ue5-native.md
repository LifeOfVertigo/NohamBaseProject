# Data Storage (UE5 Native)

All data persistence uses **UE5 native systems** - no external databases. Data is stored using UE5 config files, data tables, save games, and asset references for maximum integration with engine workflows.

### Settings Persistence (UE5 Config System)

**GameUserSettings.ini** - Standard UE5 user preferences storage
```ini
[/Script/Engine.GameUserSettings]
ResolutionSizeX=1920
ResolutionSizeY=1080
FullscreenMode=1
bVSyncEnabled=true
FrameRateLimit=0
GraphicsQuality=3
ShadowQuality=2
EffectsQuality=2

[/Script/NohamBaseProject.AudioSettings]
MasterVolume=1.0
MusicVolume=0.8
SFXVolume=0.9
VoiceVolume=1.0
AudioDevice="Default"

[/Script/NohamBaseProject.InputSettings]  
MouseSensitivity=1.0
GamepadSensitivity=1.0
InvertMouseY=false

[/Script/NohamBaseProject.GameplaySettings]
MovementSpeed=1.0
InteractionRange=200.0
AutoSave=true
ColorBlindMode=0

[/Script/NohamBaseProject.DeveloperSettings]
ShowFPS=false
ShowPerformanceMetrics=false
DebugOverlayEnabled=false
```

### Game Configuration (Data Tables)

**DT_InputMappings** - Action mappings for different input modes
```cpp
USTRUCT(BlueprintType)
struct FInputMappingData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString ActionName;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FKey KeyboardKey;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FKey GamepadButton;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FString UIPromptText;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftObjectPtr<UTexture2D> KeyboardIcon;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSoftObjectPtr<UTexture2D> GamepadIcon;
};
```

### Runtime Game State (Save Game System)

**UNohamSaveGame** - Player progress and session data
```cpp
UCLASS(BlueprintType)
class NOHAMBASEPROJECT_API UNohamSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere, Category = "Player")
    float Health = 100.0f;
    
    UPROPERTY(VisibleAnywhere, Category = "Player")
    int32 Score = 0;
    
    UPROPERTY(VisibleAnywhere, Category = "Player")
    int32 Level = 1;
    
    UPROPERTY(VisibleAnywhere, Category = "Player")
    FVector LastPosition = FVector::ZeroVector;
    
    UPROPERTY(VisibleAnywhere, Category = "Session")
    FString CurrentLevelName;
    
    UPROPERTY(VisibleAnywhere, Category = "Session")
    float TimePlayed = 0.0f;
    
    UPROPERTY(VisibleAnywhere, Category = "Inventory")
    TArray<FInventoryItem> Inventory;
    
    UPROPERTY(VisibleAnywhere, Category = "Achievements")
    TArray<FString> UnlockedAchievements;
    
    UPROPERTY(VisibleAnywhere, Category = "Meta")
    FDateTime SaveTimestamp;
    
    UPROPERTY(VisibleAnywhere, Category = "Meta")
    FString SaveGameVersion = "1.0";
};
```

### Steam Cloud Integration

**Settings Synchronization** - Config files synced via Steam Cloud
```cpp
class USteamCloudManager
{
public:
    // Upload current settings to Steam Cloud
    bool SyncSettingsToCloud()
    {
        if (SteamPlatform.IsValid())
        {
            FString SettingsJSON = SerializeSettingsToJSON();
            return SteamPlatform->SaveToCloud("NohamSettings.json", 
                                            StringToBytes(SettingsJSON));
        }
        return false;
    }
    
    // Download settings from Steam Cloud
    bool LoadSettingsFromCloud()
    {
        if (SteamPlatform.IsValid())
        {
            TArray<uint8> CloudData;
            if (SteamPlatform->LoadFromCloud("NohamSettings.json", CloudData))
            {
                FString SettingsJSON = BytesToString(CloudData);
                return DeserializeSettingsFromJSON(SettingsJSON);
            }
        }
        return false;
    }
};
```
