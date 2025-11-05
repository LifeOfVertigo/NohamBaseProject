# Frontend Architecture

The frontend consists of **HTML/CSS/JavaScript components** rendered within **NEON widgets**, leveraging **shadcn/ui components** and **Framer Motion animations** for professional game UI development.

### Component Architecture

#### Component Organization

```
Content/UI/HTML/
├── Components/
│   ├── Core/
│   │   ├── GameButton.js          # shadcn Button wrapper with game styling
│   │   ├── GameSlider.js          # shadcn Slider with real-time UE5 sync
│   │   ├── GameSelect.js          # shadcn Select with validation
│   │   ├── GameInput.js           # shadcn Input with key binding support
│   │   └── GameCard.js            # shadcn Card with consistent layout
│   ├── Layout/
│   │   ├── MenuContainer.js       # Centered menu layout pattern
│   │   ├── SettingsTab.js         # shadcn Tabs wrapper
│   │   ├── Modal.js               # shadcn Dialog for confirmations
│   │   └── Notification.js        # shadcn Toast wrapper
│   ├── HUD/
│   │   ├── HealthBar.js           # Animated progress bar
│   │   ├── ScoreDisplay.js        # Count-up animations
│   │   ├── ObjectiveTracker.js    # Dynamic task list
│   │   └── MiniMap.js             # Spatial awareness component
│   └── Developer/
│       ├── PerformanceOverlay.js  # FPS/memory display
│       ├── DebugConsole.js        # Command input interface
│       └── TimeControls.js        # Time scale manipulation
├── Pages/
│   ├── MainMenu/
│   ├── Settings/
│   ├── HUD/
│   └── Developer/
├── Styles/
│   ├── globals.css                # shadcn global styles + game theme
│   ├── components.css             # Game-specific component overrides
│   └── animations.css             # Framer Motion configuration
└── Utils/
    ├── neon-bridge.js             # NEON communication utilities
    ├── validation.js              # Settings validation helpers
    └── animations.js              # Framer Motion presets
```

### State Management Architecture

```javascript
// game-state.js - Centralized state management
class GameStateManager {
  constructor() {
    this.state = {
      // UI State
      currentMenu: 'MainMenu',
      menuHistory: [],
      inputMode: 'KeyboardMouse',
      
      // Settings State (synced with UE5)
      settings: {
        graphics: null,
        audio: null,
        input: null,
        gameplay: null,
        developer: null
      },
      
      // Game State (received from UE5)
      player: {
        health: 100,
        maxHealth: 100,
        score: 0,
        level: 1
      },
      
      // Developer State
      debug: {
        showOverlay: false,
        performanceMetrics: {
          fps: 0,
          frameTime: 0,
          memoryUsage: 0
        }
      }
    };
    
    this.listeners = new Map();
    this.initializeNEONListeners();
  }
  
  initializeNEONListeners() {
    // Settings updates from UE5
    NEON.onInvoke('settingsChanged', (newSettings) => {
      this.updateState('settings', newSettings);
    });
    
    // Game state updates from UE5
    NEON.onInvoke('playerStateChanged', (playerState) => {
      this.updateState('player', playerState);
    });
    
    // HUD updates (batched for performance)
    NEON.onInvoke('hudUpdate', (hudData) => {
      this.updateState('player', { ...this.state.player, ...hudData });
    });
    
    // Input mode changes
    NEON.onInvoke('inputModeChanged', (inputData) => {
      this.updateState('inputMode', inputData.mode);
      this.dispatchInputModeChange(inputData.mode);
    });
    
    // Debug info updates
    NEON.onInvoke('debugInfoUpdate', (debugData) => {
      this.updateState('debug.performanceMetrics', debugData);
    });
  }
}

// Global state instance
export const gameState = new GameStateManager();
```

### Routing Architecture

```javascript
// router.js - Simple hash-based routing for NEON
class NEONRouter {
  constructor() {
    this.routes = new Map();
    this.currentRoute = null;
    this.initializeRouting();
  }
  
  navigate(path) {
    const previousRoute = this.currentRoute;
    
    // Update menu history for back navigation
    if (previousRoute && previousRoute !== path) {
      gameState.updateState('menuHistory', [
        ...gameState.getState('menuHistory'),
        previousRoute
      ]);
    }
    
    // Update current menu state
    gameState.updateState('currentMenu', path);
    
    // Handle route transition
    this.handleRouteChange(previousRoute, path);
    this.currentRoute = path;
  }
  
  handleRouteChange(from, to) {
    // Hide previous component
    if (from && this.routes.has(from)) {
      const fromComponent = this.routes.get(from);
      fromComponent.hide();
    }
    
    // Show new component
    if (this.routes.has(to)) {
      const toComponent = this.routes.get(to);
      toComponent.show();
    }
    
    // Notify UE5 of menu transition
    NEON.invokeUnreal('menuTransition', { from, to });
  }
}

export const router = new NEONRouter();
```

### Frontend Services Layer

```javascript
// neon-api-client.js - Centralized NEON communication
class NEONApiClient {
  constructor() {
    this.pendingRequests = new Map();
    this.requestId = 0;
  }
  
  async invoke(functionName, data = {}) {
    const id = ++this.requestId;
    
    try {
      const result = await NEON.invokeUnrealFunction(functionName, {
        requestId: id,
        ...data
      });
      
      return {
        success: true,
        data: result,
        error: null
      };
    } catch (error) {
      console.error(`NEON API Error [${functionName}]:`, error);
      return {
        success: false,
        data: null,
        error: error.message || 'Unknown error'
      };
    }
  }
  
  send(eventName, data = {}) {
    try {
      NEON.invokeUnreal(eventName, data);
      return true;
    } catch (error) {
      console.error(`NEON Event Error [${eventName}]:`, error);
      return false;
    }
  }
}

export const neonApi = new NEONApiClient();
```
