# External APIs

Based on the PRD requirements for Steam platform integration and indie game development needs, the following external services are required:

### Steam Platform Integration (Modular)

- **Purpose:** Platform integration for Steam PC deployment, user authentication, achievements, and Steam Deck compatibility
- **Documentation:** TBD - Will depend on selected FAB store Steam plugin
- **Base URL(s):** Plugin-dependent (typically native Steam client integration)
- **Authentication:** Plugin-dependent (typically Steam API Key + Steam App ID)
- **Rate Limits:** Plugin-dependent Steam API limits

**Required Platform Features (Plugin-Agnostic):**
- User identification and authentication
- Achievement unlocking and progress tracking
- Steam Cloud save synchronization
- Controller detection and Steam Deck input support
- Localization and language detection
- User profile information access
- Steam overlay integration

**Integration Notes:** 

**MODULAR DESIGN:** Steam integration implemented through a **SteamPlatformInterface** abstraction layer that can be swapped with any FAB store Steam plugin without affecting core game systems.

**Architecture Pattern:**
```cpp
// Abstract interface - game systems depend on this, not specific plugin
class ISteamPlatformInterface 
{
public:
    virtual bool GetUserID(FString& OutUserID) = 0;
    virtual bool UnlockAchievement(const FString& AchievementID) = 0;
    virtual bool SaveToCloud(const FString& FileName, const TArray<uint8>& Data) = 0;
    virtual bool GetInputMode(ESteamInputMode& OutMode) = 0;
    virtual bool GetUserLanguage(FString& OutLanguage) = 0;
    virtual bool OpenSteamOverlay(const FString& Page) = 0;
    // ... other required platform functions
};

// Concrete implementation using selected FAB plugin
class FSteamPluginImplementation : public ISteamPlatformInterface
{
    // Implementation delegates to chosen FAB store plugin
};
```

**Plugin Replacement Strategy:** When FAB store plugin is selected, only the concrete implementation class needs to be replaced. All game systems, NEON bridge communication, and UI components remain unchanged.

### NEON Web Library

- **Purpose:** Bridge communication between UE5 engine and HTML/JavaScript UI components
- **Documentation:** https://github.com/micon4sure/neon-web
- **Base URL(s):** N/A (local plugin integration)
- **Authentication:** N/A (local communication)
- **Rate Limits:** Frame budget limited (3.5ms processing time per tick)

**Key Endpoints Used:**
- `NEON.invokeUnreal(eventName, data)` - Send UI events to UE5 Blueprint system
- `NEON.invokeUnrealFunction(functionName, data)` - Call UE5 functions with return values
- `NEON.onInvoke(eventName, callback)` - Receive game state updates from UE5
- `GetFPS()` - Monitor NEON rendering performance
- `SetMaxFPS(fps)` - Configure NEON frame rate limits

**Integration Notes:** NEON plugin handles all communication between UE5 and HTML UI. Requires proper NEON widget setup in UMG with configured Development/Live URLs and frame budget allocation.

### shadcn/ui Component Library

- **Purpose:** Professional UI component system for rapid HTML interface development
- **Documentation:** https://ui.shadcn.com/docs
- **Base URL(s):** N/A (bundled components via npm/Vite)
- **Authentication:** N/A (open source library)
- **Rate Limits:** N/A (local component library)

**Key Components Used:**
- **Form Controls:** `Button`, `Input`, `Slider`, `Select`, `Switch`, `Checkbox` - Settings interface building blocks
- **Layout:** `Card`, `Tabs`, `Dialog`, `Sheet`, `Separator` - Menu structure and organization
- **Feedback:** `Toast`, `Alert`, `Progress`, `Badge` - User notifications and status display
- **Navigation:** `Command`, `Menubar`, `Breadcrumb` - Menu navigation patterns
- **Data Display:** `Table`, `Avatar`, `Calendar` - Information presentation

**Integration Notes:** shadcn components bundled via Vite and served through NEON's local file system. Requires Tailwind CSS configuration and proper theming setup for game-appropriate styling.

### Lucide Icons

- **Purpose:** Comprehensive SVG icon library for UI elements and input prompts
- **Documentation:** https://lucide.dev/icons
- **Base URL(s):** N/A (bundled via npm)
- **Authentication:** N/A (open source library)
- **Rate Limits:** N/A (local asset library)

**Key Icons Used:**
- **Input Icons:** `Mouse`, `Keyboard`, `Gamepad2`, `Move` - Input method indicators
- **Settings Icons:** `Volume2`, `Monitor`, `Settings`, `Palette` - Settings category icons
- **Game Icons:** `Play`, `Pause`, `SkipForward`, `RotateCcw` - Game control indicators
- **System Icons:** `AlertTriangle`, `CheckCircle`, `Info`, `X` - Status and notification icons

**Integration Notes:** Icons imported as React components or SVG assets, optimized for game UI contexts with appropriate sizing and theming support.
