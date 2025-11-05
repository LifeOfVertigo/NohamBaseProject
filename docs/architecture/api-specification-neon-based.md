# API Specification (NEON-Based)

The API specification defines the **UE5 Blueprint/C++ to NEON JavaScript** communication layer, enabling type-safe bidirectional data flow between native game systems and HTML UI components.

### NEON Communication Pattern

**Communication Pattern:** Direct function calls via NEON's JavaScript-to-UE5 bridge with event-driven updates from UE5 to JavaScript.

#### UE5 → JavaScript (Game State Updates)

**Event-based data push from UE5 to HTML UI:**

```javascript
// NEON Event System - UE5 pushes data to JavaScript
window.UE5Events = {
  // Settings Management
  onSettingsChanged: (settings: GameSettings) => void,
  onSettingValidationError: (field: string, error: string) => void,
  
  // Game State Updates  
  onPlayerStateChanged: (playerState: PlayerGameState) => void,
  onUIStateChanged: (uiState: UIState) => void,
  onMenuTransition: (fromMenu: string, toMenu: string) => void,
  
  // Real-time Updates (HUD)
  onHealthChanged: (health: number, maxHealth: number) => void,
  onScoreChanged: (score: number) => void,
  onFPSUpdated: (fps: number, frameTime: number) => void,
  
  // Input System
  onInputModeChanged: (mode: 'KeyboardMouse' | 'Gamepad' | 'SteamDeck') => void,
  onInputPromptsUpdated: (prompts: Record<string, string>) => void,
  
  // Notifications
  onShowNotification: (notification: UIState['notifications']['toasts'][0]) => void,
  
  // Developer Tools
  onDebugInfoUpdated: (debugInfo: PlayerGameState['debug']) => void,
  onPerformanceMetricsUpdated: (metrics: object) => void
};
```

#### JavaScript → UE5 (User Actions)

**Function calls from HTML UI to UE5 systems:**

```javascript
// NEON Function Bridge - JavaScript calls UE5 functions
window.UE5Functions = {
  // Settings Management
  updateGraphicsSettings: (graphics: GameSettings['graphics']) => Promise<boolean>,
  updateAudioSettings: (audio: GameSettings['audio']) => Promise<boolean>,
  updateInputSettings: (input: GameSettings['input']) => Promise<boolean>,
  updateGameplaySettings: (gameplay: GameSettings['gameplay']) => Promise<boolean>,
  resetSettingsToDefault: (category?: string) => Promise<boolean>,
  
  // Menu Navigation
  navigateToMenu: (menuId: string) => Promise<void>,
  exitGame: () => Promise<void>,
  startNewGame: () => Promise<void>,
  loadGame: (saveSlot?: number) => Promise<boolean>,
  saveGame: (saveSlot?: number) => Promise<boolean>,
  
  // Gameplay Commands
  pauseGame: () => Promise<void>,
  resumeGame: () => Promise<void>,
  restartLevel: () => Promise<void>,
  returnToMainMenu: () => Promise<void>,
  
  // Input Actions
  rebindKey: (action: string, newKey: string) => Promise<boolean>,
  testInputBinding: (action: string) => Promise<void>,
  resetInputToDefaults: () => Promise<void>,
  
  // Developer Tools
  toggleDebugOverlay: () => Promise<void>,
  togglePerformanceMetrics: () => Promise<void>,
  toggleCollisionVisualization: () => Promise<void>,
  setTimeScale: (scale: number) => Promise<void>,
  executeConsoleCommand: (command: string) => Promise<string>,
  
  // System Queries
  getSupportedResolutions: () => Promise<Array<{width: number, height: number}>>,
  getAvailableAudioDevices: () => Promise<string[]>,
  getCurrentSettings: () => Promise<GameSettings>,
  getSystemInfo: () => Promise<object>,
  
  // Steam Integration
  openSteamOverlay: (page?: string) => Promise<void>,
  syncSettingsToCloud: () => Promise<boolean>,
  checkAchievements: () => Promise<string[]>
};
```

#### NEON Integration Examples

**Settings UI Component Integration:**

```javascript
// Settings updates from UE5
NEON.onInvoke('settingsChanged', (data) => {
  updateSettingsUI(data);
});

NEON.onInvoke('settingValidationError', (data) => {
  showSettingsError(data.field, data.error);
});

// Game State Updates
NEON.onInvoke('playerStateChanged', (data) => {
  updateHUD(data);
});

NEON.onInvoke('menuTransition', (data) => {
  handleMenuNavigation(data.fromMenu, data.toMenu);
});

// Real-time HUD Updates (batched for performance)
NEON.onInvoke('hudUpdate', (data) => {
  if (data.health !== undefined) updateHealthBar(data.health, data.maxHealth);
  if (data.score !== undefined) updateScore(data.score);
  if (data.fps !== undefined) updateFPSDisplay(data.fps);
});

// Input System Changes
NEON.onInvoke('inputModeChanged', (data) => {
  updateInputPrompts(data.mode);
});

// Notifications
NEON.onInvoke('showNotification', (data) => {
  showToast(data.type, data.title, data.message, data.duration);
});

// Developer Tools
NEON.onInvoke('debugInfoUpdate', (data) => {
  updateDebugOverlay(data);
});
```

**JavaScript to UE5 Communication:**

```javascript
// ⚠️ CRITICAL: Use correct NEON API based on communication type

// EVENTS (One-Way: JS → UE5) - Use NEON.invokeUnrealEvent()
// For actions that don't need return values

// Graphics Settings Update (Event)
const updateGraphicsSettings = (newSettings) => {
  NEON.invokeUnrealEvent('UpdateGraphicsSettings', {
    resolution: newSettings.resolution,
    fullscreen: newSettings.fullscreen,
    vsync: newSettings.vsync,
    maxFPS: newSettings.maxFPS, // 0 = no cap
    qualityPreset: newSettings.qualityPreset,
    shadows: newSettings.shadows,
    effects: newSettings.effects
  });
};

// Menu Navigation (Event)
const navigateToMenu = (menuId) => {
  NEON.invokeUnrealEvent('NavigateToMenu', { menuId: menuId });
};

const startNewGame = () => {
  NEON.invokeUnrealEvent('StartNewGame', {});
};

// FUNCTIONS (Two-Way: JS ↔ UE5) - Use await NEON.invokeUnrealFunction()
// For queries that return data - note array [] parameter format

// System Queries with Return Values (Function)
const getSupportedResolutions = async () => {
  const result = await NEON.invokeUnrealFunction('GetSupportedResolutions', []);
  return result.resolutions;
};

const getCurrentSettings = async () => {
  const result = await NEON.invokeUnrealFunction('GetCurrentSettings', []);
  return result;
};

// Validation with return (Function)
const validateSettings = async (category, settings) => {
  const result = await NEON.invokeUnrealFunction('ValidateSettings', [category, settings]);
  return result.isValid;
};
```

### UE5 Blueprint Integration

**⚠️ CRITICAL NAMING RULES:**
- JavaScript events → UE5 Custom Events with `OnInvoke_` prefix
- JavaScript functions → UE5 Functions with `Invoke_` prefix
- Names are case-sensitive and must match exactly!

**Required Custom Events in UE5 Widget Blueprint:**

```
// Format: OnInvoke_[JavaScriptEventName]
// JavaScript: NEON.invokeUnrealEvent('UpdateGraphicsSettings', {...})
// UE5: Custom Event named "OnInvoke_UpdateGraphicsSettings"

OnInvoke_UpdateGraphicsSettings (JSONObject: settings)
OnInvoke_UpdateAudioSettings (JSONObject: settings)
OnInvoke_UpdateInputSettings (JSONObject: settings)
OnInvoke_NavigateToMenu (JSONObject: data)
OnInvoke_StartNewGame (JSONObject: data)
OnInvoke_ExitGame (JSONObject: data)
OnInvoke_PauseGame (JSONObject: data)
OnInvoke_ResumeGame (JSONObject: data)
OnInvoke_ToggleDebugOverlay (JSONObject: data)
OnInvoke_TogglePerformanceMetrics (JSONObject: data)
OnInvoke_SetTimeScale (JSONObject: data)
OnInvoke_ApplySettings (JSONObject: data)
OnInvoke_BackToMainMenu (JSONObject: data)
OnInvoke_LoadGame (JSONObject: data)
OnInvoke_QuitGame (JSONObject: data)
OnInvoke_OpenSettings (JSONObject: data)
OnInvoke_LogMessage (JSONObject: data)
```

**Required Custom Functions in UE5 Widget Blueprint:**

```
// Format: Invoke_[JavaScriptFunctionName]
// JavaScript: await NEON.invokeUnrealFunction('GetSupportedResolutions', [])
// UE5: Function named "Invoke_GetSupportedResolutions" with Return Node

Invoke_GetSupportedResolutions () → JSONWrapper
Invoke_GetAvailableAudioDevices () → JSONWrapper
Invoke_GetCurrentSettings () → JSONWrapper
Invoke_ValidateSettings (category: String, settings: JSONWrapper) → JSONWrapper
Invoke_ExecuteConsoleCommand (command: String) → JSONWrapper
```

**Blueprint Implementation Notes:**
1. Custom Events receive data from JavaScript but don't return values
2. Functions MUST include Return Node to send data back to JavaScript
3. Use JSONObject for event parameters (incoming only)
4. Use JSONWrapper for function return values (outgoing data)
5. All parameters are passed within the JSON object/array

### NEON Widget Configuration

```
Development URL: /NEON/index.html (local file)
Live URL: /NEON/index.html (packaged)
Max FPS: 60 (for smooth UI animations)
Frame Budget: 3.5ms (gives NEON processing time each tick)
```
