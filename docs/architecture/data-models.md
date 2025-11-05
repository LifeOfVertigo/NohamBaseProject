# Data Models

The core data models define entities shared between the UE5 C++ backend and HTML/JavaScript frontend, enabling type-safe communication and consistent data handling across the hybrid architecture.

### GameSettings

**Purpose:** Centralizes all player-configurable settings with persistence and real-time preview capabilities

**Key Attributes:**
- `GraphicsSettings`: Object - Resolution, fullscreen mode, VSync, FPS cap, quality presets
- `AudioSettings`: Object - Master/Music/SFX/Voice volume levels, audio device selection
- `InputSettings`: Object - Key bindings, mouse sensitivity, gamepad configuration, dead zones
- `GameplaySettings`: Object - Movement speed, interaction range, accessibility options
- `DeveloperSettings`: Object - Debug overlays, performance metrics, collision visualization
- `MetaSettings`: Object - Language, UI scale, first-time setup flags

#### TypeScript Interface

```typescript
interface GameSettings {
  graphics: {
    resolution: { width: number; height: number };
    fullscreen: boolean;
    vsync: boolean;
    maxFPS: number; // 0 = no cap, >0 = FPS limit
    qualityPreset: 'Low' | 'Medium' | 'High' | 'Ultra' | 'Custom';
    shadows: 'Off' | 'Low' | 'Medium' | 'High';
    effects: 'Off' | 'Low' | 'Medium' | 'High';
  };
  audio: {
    masterVolume: number; // 0.0 - 1.0
    musicVolume: number;
    sfxVolume: number;
    voiceVolume: number;
    audioDevice: string;
    muteAll: boolean;
  };
  input: {
    mouseSensitivity: number;
    gamepadSensitivity: number;
    keyBindings: Record<string, string>; // action -> key
    gamepadBindings: Record<string, string>; // action -> button
    deadZone: number; // 0.0 - 1.0
    invertMouseY: boolean;
  };
  gameplay: {
    movementSpeed: number;
    interactionRange: number;
    autoSave: boolean;
    subtitles: boolean;
    colorBlindMode: 'None' | 'Protanopia' | 'Deuteranopia' | 'Tritanopia';
  };
  developer: {
    showFPS: boolean;
    showPerformanceMetrics: boolean;
    enableCollisionVisualization: boolean;
    enableDebugOverlay: boolean;
    timeControlsEnabled: boolean;
  };
  meta: {
    language: string;
    uiScale: number; // 0.8 - 1.5
    firstTimeSetup: boolean;
    settingsVersion: string;
    lastSaveTimestamp: number;
  };
}
```

#### Relationships
- **Persisted by:** UE5 Config System (GameUserSettings.ini)
- **Updated via:** Settings UI components (shadcn sliders, checkboxes, selects)
- **Applied to:** Various UE5 subsystems (Graphics, Audio, Input, Gameplay)
- **Synchronized with:** Steam Cloud Save system

### UIState

**Purpose:** Manages transient UI state and component visibility across different game screens

**Key Attributes:**
- `currentMenu`: String - Active menu identifier for navigation
- `menuHistory`: Array - Navigation breadcrumb for back functionality  
- `modalStack`: Array - Open modals/dialogs for proper layering
- `notificationQueue`: Array - Toast notifications and alerts
- `developerMenuVisible`: Boolean - Debug overlay state
- `inputMode`: String - Current input method for UI adaptation

#### TypeScript Interface

```typescript
interface UIState {
  navigation: {
    currentMenu: 'MainMenu' | 'Settings' | 'Gameplay' | 'Pause' | 'Developer';
    menuHistory: string[];
    canGoBack: boolean;
  };
  modals: {
    activeModals: string[];
    confirmationDialog: {
      visible: boolean;
      title: string;
      message: string;
      onConfirm: string; // UE5 function name
      onCancel: string;
    };
  };
  notifications: {
    toasts: Array<{
      id: string;
      type: 'success' | 'error' | 'warning' | 'info';
      title: string;
      message: string;
      duration: number;
      timestamp: number;
    }>;
  };
  input: {
    currentInputMode: 'KeyboardMouse' | 'Gamepad' | 'SteamDeck';
    lastInputTime: number;
    inputPrompts: Record<string, string>; // action -> display icon
  };
  developer: {
    menuVisible: boolean;
    performanceOverlayVisible: boolean;
    collisionVisualizationActive: boolean;
  };
}
```

#### Relationships
- **Managed by:** NEON JavaScript state management
- **Updated via:** User interactions and game events
- **Synchronized with:** UE5 Blueprint events
- **Reset on:** Menu transitions and game state changes

### PlayerGameState

**Purpose:** Represents current game session data and player progress for UI display

**Key Attributes:**
- `playerStats`: Object - Health, score, level, experience
- `gameSession`: Object - Current level, time played, objectives
- `inventory`: Array - Player items and resources
- `achievements`: Object - Progress tracking for Steam achievements
- `debugInfo`: Object - Development metrics and system status

#### TypeScript Interface

```typescript
interface PlayerGameState {
  player: {
    health: number;
    maxHealth: number;
    score: number;
    level: number;
    experience: number;
    experienceToNext: number;
  };
  session: {
    currentLevel: string;
    timePlayed: number; // seconds
    objectives: Array<{
      id: string;
      description: string;
      completed: boolean;
      progress: number; // 0.0 - 1.0
    }>;
    gameMode: string;
  };
  inventory: {
    items: Array<{
      id: string;
      name: string;
      quantity: number;
      type: 'Weapon' | 'Tool' | 'Consumable' | 'Key';
      icon: string;
    }>;
    capacity: number;
  };
  achievements: {
    unlockedAchievements: string[];
    progressTracking: Record<string, number>;
  };
  debug: {
    fps: number;
    frameTime: number;
    memoryUsage: number;
    drawCalls: number;
    position: { x: number; y: number; z: number };
  };
}
```

#### Relationships
- **Updated by:** Game systems during gameplay
- **Displayed in:** HUD components and pause menu
- **Persisted via:** Save game system
- **Synchronized with:** Steam achievements and stats
