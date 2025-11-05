# Core Workflows

Key system workflows illustrating component interactions, NEON communication patterns, and Steam platform integration through the modular interface.

### Settings Update Workflow

```mermaid
sequenceDiagram
    participant UI as NEON Settings UI
    participant Bridge as NEONBridgeSubsystem
    participant Settings as SettingsSubsystem
    participant Steam as ISteamPlatformInterface
    participant Engine as UE5 Engine

    UI->>Bridge: NEON.invokeUnreal('updateGraphicsSettings', data)
    Bridge->>Settings: UpdateGraphicsSettings(FGraphicsSettings)
    
    Settings->>Settings: ValidateSettings(newSettings)
    alt Settings Valid
        Settings->>Engine: ApplyGraphicsSettings(resolution, vsync, maxFPS)
        Engine-->>Settings: Success
        Settings->>Steam: SaveToCloud('settings.json', data)
        Steam-->>Settings: Cloud sync result
        Settings->>Bridge: SendSettingsUpdateSuccess()
        Bridge->>UI: NEON.invoke('settingsChanged', updatedSettings)
        UI->>UI: updateSettingsUI(updatedSettings)
    else Settings Invalid
        Settings->>Bridge: SendValidationError(field, error)
        Bridge->>UI: NEON.invoke('settingValidationError', {field, error})
        UI->>UI: showErrorToast(error)
        UI->>UI: revertToLastValidSettings()
    end
```

### Game Launch Workflow

```mermaid
sequenceDiagram
    participant UI as NEON Main Menu
    participant Bridge as NEONBridgeSubsystem
    participant Game as GameModeSubsystem
    participant Input as InputManagerSubsystem
    participant Steam as ISteamPlatformInterface

    UI->>Bridge: NEON.invokeUnreal('startNewGame', {})
    Bridge->>Steam: GetUserID(userID)
    Steam-->>Bridge: Steam user authentication
    Bridge->>Input: DetectInputMode()
    Input-->>Bridge: CurrentInputMode (Keyboard/Gamepad/SteamDeck)
    Bridge->>Game: InitializeGameSession(userID, inputMode)
    
    Game->>Game: LoadGameLevel()
    Game->>Bridge: SendGameStateUpdate(playerState)
    Bridge->>UI: NEON.invoke('menuTransition', {from: 'MainMenu', to: 'Gameplay'})
    UI->>UI: fadeOutMainMenu()
    
    Game->>Bridge: SendHUDInitialization(initialHUDData)
    Bridge->>UI: NEON.invoke('hudUpdate', hudData)
    UI->>UI: initializeGameplayHUD(hudData)
```

### Real-time HUD Update Workflow

```mermaid
sequenceDiagram
    participant Game as Gameplay Systems
    participant Bridge as NEONBridgeSubsystem
    participant UI as NEON HUD Component
    participant Anim as Framer Motion

    loop Every 16ms (60fps)
        Game->>Bridge: QueueHUDUpdate(health, score, ammo)
        Bridge->>Bridge: BatchUpdates(16ms timer)
    end
    
    Bridge->>UI: NEON.invoke('hudUpdate', batchedData)
    
    par Health Bar Update
        UI->>Anim: animateHealthBar(newHealth, maxHealth)
        Anim->>UI: Smooth health bar transition
    and Score Update
        UI->>Anim: animateScoreCounter(newScore)
        Anim->>UI: Count-up animation
    and Ammo Update
        UI->>UI: updateAmmoDisplay(newAmmo)
    end
```

### Input Mode Detection Workflow

```mermaid
sequenceDiagram
    participant Hardware as Input Hardware
    participant Input as InputManagerSubsystem
    participant Steam as ISteamPlatformInterface
    participant Bridge as NEONBridgeSubsystem
    participant UI as All NEON Components

    Hardware->>Input: InputDeviceChange(Gamepad Connected)
    Input->>Steam: GetConnectedControllers()
    Steam-->>Input: Controller list + Steam Deck detection
    
    Input->>Input: DetermineInputMode(controllers, steamDeck)
    Input->>Input: GenerateInputPrompts(currentMode)
    
    Input->>Bridge: NotifyInputModeChange(mode, prompts)
    Bridge->>UI: NEON.invoke('inputModeChanged', {mode, prompts})
    
    par Main Menu
        UI->>UI: updateButtonPrompts(gamepadIcons)
    and Settings Menu
        UI->>UI: updateNavigationHints(dpadNavigation)
    and HUD
        UI->>UI: updateInteractionPrompts(gamepadButtons)
    end
```

### Steam Achievement Unlock Workflow

```mermaid
sequenceDiagram
    participant Game as Gameplay Event
    participant Achievement as AchievementSubsystem
    participant Steam as ISteamPlatformInterface

    Game->>Achievement: TriggerAchievement('FIRST_KILL')
    Achievement->>Achievement: ValidateAchievementCriteria()
    
    alt Achievement Unlocked
        Achievement->>Steam: UnlockAchievement('FIRST_KILL')
        Steam-->>Achievement: Success
        Note over Steam: Steam handles achievement notification display
        Steam->>Steam: ShowSteamAchievementNotification()
    else Achievement Failed
        Steam-->>Achievement: Error (already unlocked/network)
        Note over Achievement: Log error, no UI notification needed
    end
```

### Developer Tools Workflow

```mermaid
sequenceDiagram
    participant UI as NEON Developer Menu
    participant Bridge as NEONBridgeSubsystem
    participant DevTools as DeveloperToolsSubsystem
    participant Engine as UE5 Debug Systems

    UI->>Bridge: NEON.invokeUnreal('toggleDebugOverlay', {})
    Bridge->>DevTools: ToggleDebugOverlay()
    DevTools->>Engine: EnableCollisionVisualization()
    Engine-->>DevTools: Collision display activated
    
    loop Performance Monitoring
        DevTools->>Engine: GetPerformanceMetrics()
        Engine-->>DevTools: FPS, memory, draw calls
        DevTools->>Bridge: SendDebugUpdate(metrics)
        Bridge->>UI: NEON.invoke('debugInfoUpdate', metrics)
        UI->>UI: updatePerformanceDisplay(metrics)
    end
    
    UI->>Bridge: NEON.invokeUnrealFunction('executeConsoleCommand', {command: 'stat fps'})
    Bridge->>DevTools: ExecuteConsoleCommand('stat fps')
    DevTools->>Engine: RunConsoleCommand('stat fps')
    Engine-->>DevTools: Command output
    DevTools-->>Bridge: Return command result
    Bridge-->>UI: Return result to JavaScript
    UI->>UI: displayConsoleOutput(result)
```

### Error Handling Workflow

```mermaid
sequenceDiagram
    participant UI as NEON Component
    participant Bridge as NEONBridgeSubsystem
    participant Subsystem as Any UE5 Subsystem
    participant Steam as ISteamPlatformInterface

    UI->>Bridge: NEON.invokeUnreal('updateSettings', invalidData)
    Bridge->>Subsystem: ProcessSettingsUpdate(invalidData)
    Subsystem->>Subsystem: ValidateData(invalidData)
    
    alt Validation Error
        Subsystem->>Bridge: SendValidationError(field, errorMessage)
        Bridge->>UI: NEON.invoke('settingValidationError', {field, error})
        UI->>UI: showFieldError(field, error)
        UI->>UI: highlightInvalidField(field)
    else Steam Communication Error
        Subsystem->>Steam: SaveToCloud(data)
        Steam-->>Subsystem: Network error
        Subsystem->>Bridge: SendCloudSyncError(errorCode)
        Bridge->>UI: NEON.invoke('showNotification', {
            type: 'warning',
            message: 'Settings saved locally, cloud sync failed'
        })
        UI->>UI: showRetryOption()
    end
```
