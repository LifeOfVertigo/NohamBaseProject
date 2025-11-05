# NEON UMG Widget Setup Guide

## Overview

This guide walks through creating UMG widgets that use NEON to display HTML/JavaScript UI within Unreal Engine 5. NEON uses CEF (Chromium Embedded Framework) to render web content as textures inside UMG widgets.

---

## Understanding NEON Widget Architecture

### Key Components

1. **UNEONWidget** (C++ Class from NEON Plugin)
   - Inherits from `UUserWidget` (standard UMG widget)
   - Manages CEF browser instance
   - Handles input forwarding (mouse, keyboard)
   - Provides UE5 ↔ JavaScript communication

2. **UImage Component** (`_BrowserImage`)
   - Required UMG component that MUST be named `_BrowserImage`
   - Displays the rendered web content as a texture
   - NEON automatically binds to this by name using `BindWidget`

3. **URLs**
   - `_DevelopmentURL`: Used during development (e.g., `http://localhost:5173` for Vite dev server)
   - `_LiveURL`: Used in packaged builds (local HTML file path)

---

## Step-by-Step: Creating a NEON Widget

### Step 1: Create the UMG Widget Blueprint

1. **Open UE5 Editor**
2. **Navigate to** `Content/UI/Widgets/`
3. **Right-click** → **User Interface** → **Widget Blueprint**
4. **Name it**: `WBP_MainMenu`
5. **Open the widget** by double-clicking

---

### Step 2: Set Parent Class to UNEONWidget

1. **In the Widget Blueprint Editor**, click **File** → **Reparent Blueprint**
2. **Select**: `UNEONWidget` (from NEON plugin)
3. **Click Reparent**

> **Why?** This gives your widget access to all NEON functionality (browser rendering, JS communication, etc.)

---

### Step 3: Add Required Image Component

**CRITICAL:** The image component MUST be named exactly `_BrowserImage`

1. **In the Designer view**, drag an **Image** component from the Palette
2. **In the Details panel**, find the **Name** field at the top
3. **Rename it to exactly**: `_BrowserImage` (case-sensitive!)
4. **Set Anchors**: Full screen (0,0 to 1,1)
5. **Set Offsets**: All to 0 (Left, Top, Right, Bottom)
6. **Check "Is Variable"** (should be automatic)

**Layout Settings:**
- **Anchors**: Min (0, 0), Max (1, 1)
- **Alignment**: 0.5, 0.5
- **Offsets**: 0, 0, 0, 0
- **Size To Content**: False

---

### Step 4: Configure NEON Properties

1. **Select the root** of the widget (the WBP_MainMenu itself in hierarchy)
2. **In Details panel**, find the **NEON** category

**Set these properties:**

| Property | Development Value | Packaged Value | Notes |
|----------|-------------------|----------------|-------|
| `_DevelopmentURL` | `http://localhost:5173/MainMenu/index.html` | - | Vite dev server URL |
| `_LiveURL` | `file:///Content/UI/NEON/dist/MainMenu/index.html` | Used in packaged builds | Local file path |
| `_OpenDevTools` | `true` (dev only) | `false` | Opens Chrome DevTools for debugging |
| `_MaxFPS` | `60` | `60` | Browser render FPS (lower = better perf) |
| `_ExternalBeginFrame` | `false` | `false` | Advanced: manual frame control |

**URL Path Formats:**

**Development (Vite dev server):**
```
http://localhost:5173/MainMenu/index.html
```

**Packaged (local file):**
```
file:///Content/UI/NEON/dist/MainMenu/index.html
```

> **Note:** The `file:///` protocol with three slashes is required for local files in CEF.

---

### Step 5: Set Up Input Mode

1. **Still in the root widget's Details panel**
2. **Find "Behavior" section**
3. **Check:**
   - ✅ Is Focusable
   - ✅ Supports Keyboard Focus

This ensures your widget can receive keyboard/mouse input.

---

### Step 6: Blueprint Event Graph Setup (Optional)

If you want to handle NEON-specific events or set up communication:

1. **Switch to Graph tab**
2. **Right-click** → Search for **"On Browser Created"**
3. **This event fires** when the CEF browser finishes initializing
4. **Use this to:**
   - Initialize JavaScript state
   - Send initial data from UE5 to JS
   - Set up communication handlers

**Example Blueprint:**
```
Event Construct
  └─> Get Game Instance
      └─> Cast To NohamGameInstance
          └─> Get NEON Bridge Subsystem
              └─> (Store reference for later use)

On Browser Created
  └─> Invoke Web (pass initial game state to JavaScript)
```

---

## JavaScript ↔ C++ Communication Setup

### From C++ to JavaScript

**In C++ (e.g., NohamNEONBridgeSubsystem):**

```cpp
// Get reference to the NEON widget
UNEONWidget* MainMenuWidget = /* your widget reference */;

// Send simple string
MainMenuWidget->InvokeWebString(TEXT("updatePlayerName"), TEXT("John"));

// Send integer
MainMenuWidget->InvokeWebInteger(TEXT("updateScore"), 1000);

// Send complex JSON data
FJsonObjectWrapper JsonData;
JsonData.JsonObject = MakeShared<FJsonObject>();
JsonData.JsonObject->SetStringField("menu", "settings");
JsonData.JsonObject->SetBoolField("visible", true);

MainMenuWidget->InvokeWeb(TEXT("navigateToMenu"), JsonData);
```

**In JavaScript (MainMenu/index.tsx):**

```javascript
// Register handlers for UE5 function calls
window.updatePlayerName = (name) => {
  console.log('[NEON] Player name updated:', name);
  // Update React state, etc.
};

window.updateScore = (score) => {
  console.log('[NEON] Score updated:', score);
};

window.navigateToMenu = (data) => {
  console.log('[NEON] Navigate to:', data);
  // data = { menu: "settings", visible: true }
};
```

---

### From JavaScript to C++

**In JavaScript:**

```javascript
// NEON provides a global `ue` object
if (window.ue && window.ue.interface) {
  // Call C++ function by name
  window.ue.interface.new_game();
  window.ue.interface.open_settings();

  // Pass data to C++
  window.ue.interface.update_setting('graphics', {
    resolution: '1920x1080',
    fullscreen: true
  });
}
```

**In C++ (NohamNEONBridgeSubsystem or custom NEON handler):**

You need to bind these functions when the widget is created. NEON uses CEF message routing to handle JS → C++ calls.

**Example binding (in widget initialization):**

```cpp
void UNohamMainMenuWidget::NativeConstruct()
{
  Super::NativeConstruct();

  // Bind JavaScript functions to C++ methods
  BindJavaScriptFunction(TEXT("new_game"), this, &UNohamMainMenuWidget::OnNewGame);
  BindJavaScriptFunction(TEXT("open_settings"), this, &UNohamMainMenuWidget::OnOpenSettings);
}

void UNohamMainMenuWidget::OnNewGame()
{
  // Get subsystem
  UNohamNEONBridgeSubsystem* Bridge = GetGameInstance()->GetSubsystem<UNohamNEONBridgeSubsystem>();
  Bridge->NewGame();
}
```

---

## Complete MainMenu Widget Checklist

Use this checklist when creating `WBP_MainMenu`:

- [ ] Widget Blueprint created in `Content/UI/Widgets/`
- [ ] Parent class set to `UNEONWidget`
- [ ] Image component added and named **exactly** `_BrowserImage`
- [ ] Image component set to full screen (anchors 0,0 to 1,1)
- [ ] Image component marked as variable
- [ ] `_DevelopmentURL` set to `http://localhost:5173/MainMenu/index.html`
- [ ] `_LiveURL` set to `file:///Content/UI/NEON/dist/MainMenu/index.html`
- [ ] `_OpenDevTools` set to `true` for debugging
- [ ] `_MaxFPS` set to `60`
- [ ] Widget is set to "Is Focusable" and "Supports Keyboard Focus"
- [ ] Blueprint graph has Event Construct handler (optional but recommended)
- [ ] Blueprint graph has On Browser Created handler (optional)

---

## Displaying the Widget in Game

### Method 1: Show on Game Start (Main Menu)

**In your GameMode Blueprint or C++:**

```cpp
void ANohamGameMode::BeginPlay()
{
  Super::BeginPlay();

  // Load the main menu widget
  TSubclassOf<UUserWidget> MainMenuWidgetClass = LoadClass<UUserWidget>(
    nullptr,
    TEXT("/Game/UI/Widgets/WBP_MainMenu.WBP_MainMenu_C")
  );

  if (MainMenuWidgetClass)
  {
    UUserWidget* MainMenuWidget = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass);
    if (MainMenuWidget)
    {
      MainMenuWidget->AddToViewport(0); // Z-order 0 (frontmost)

      // Set input mode to UI only
      APlayerController* PC = GetWorld()->GetFirstPlayerController();
      if (PC)
      {
        FInputModeUIOnly InputMode;
        InputMode.SetWidgetToFocus(MainMenuWidget->TakeWidget());
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

        PC->SetInputMode(InputMode);
        PC->bShowMouseCursor = true;
      }
    }
  }
}
```

---

### Method 2: Show/Hide Dynamically (HUD, Settings)

**Create a UI Manager Subsystem:**

```cpp
UCLASS()
class UNohamUIManagerSubsystem : public UGameInstanceSubsystem
{
  GENERATED_BODY()

private:
  UPROPERTY()
  TMap<FString, UUserWidget*> CachedWidgets;

public:
  UFUNCTION(BlueprintCallable)
  void ShowWidget(const FString& WidgetName, bool bShowMouseCursor = true);

  UFUNCTION(BlueprintCallable)
  void HideWidget(const FString& WidgetName);

  UFUNCTION(BlueprintCallable)
  void HideAllWidgets();
};
```

**Usage:**

```cpp
// Show main menu
UIManager->ShowWidget(TEXT("MainMenu"), true);

// Hide main menu, show settings
UIManager->HideWidget(TEXT("MainMenu"));
UIManager->ShowWidget(TEXT("Settings"), true);

// Back to game (hide all UI)
UIManager->HideAllWidgets();
PC->SetInputMode(FInputModeGameOnly());
PC->bShowMouseCursor = false;
```

---

## Development Workflow

### 1. Start Vite Dev Server

```bash
cd Content/UI/NEON
npm run dev
```

This starts the Vite server at `http://localhost:5173`

### 2. Launch UE5 Editor

With Vite running, the NEON widget will load your HTML from the dev server, enabling **hot reload** for instant UI updates.

### 3. Open Chrome DevTools

Set `_OpenDevTools = true` in your widget properties. When the widget loads, Chrome DevTools will open automatically for debugging JavaScript.

### 4. Test in PIE (Play In Editor)

Press **Play** in UE5. Your NEON widget should display the main menu. Any changes you make to React/JavaScript will hot-reload immediately.

---

## Production Build Workflow

### 1. Build Web Assets

```bash
cd Content/UI/NEON
npm run build
```

This creates bundled files in `Content/UI/NEON/dist/`

### 2. Update Widget to Use LiveURL

Ensure `_LiveURL` points to the built files:
```
file:///Content/UI/NEON/dist/MainMenu/index.html
```

### 3. Package the Game

NEON will automatically use `_LiveURL` in packaged builds and include the `dist/` folder in the pak file.

---

## Troubleshooting

### Widget Shows Blank/Black Screen

**Possible causes:**
1. **Wrong URL**: Check `_DevelopmentURL` matches your Vite server
2. **Vite not running**: Ensure `npm run dev` is active
3. **Image component not named correctly**: MUST be `_BrowserImage` (case-sensitive)
4. **CEF initialization failed**: Check Output Log for NEON errors

**Check Output Log for:**
```
LogNEON: Browser created successfully
LogNEON: Loaded URL: http://localhost:5173/MainMenu/index.html
```

---

### JavaScript Functions Not Calling C++

**Possible causes:**
1. **`ue.interface` not available**: NEON needs time to initialize
2. **Function not bound**: Ensure C++ function binding is set up
3. **Browser not ready**: Wait for page load before calling UE5 functions

**Solution:**
```javascript
// Wait for NEON to be ready
window.addEventListener('load', () => {
  if (window.ue && window.ue.interface) {
    console.log('[NEON] UE5 interface ready!');
    // Now safe to call UE5 functions
  } else {
    console.error('[NEON] UE5 interface not available');
  }
});
```

---

### C++ Can't Call JavaScript Functions

**Possible causes:**
1. **Function not on global window object**: Must be `window.myFunction = ...`
2. **Page not loaded yet**: Wait for `OnBrowserCreated` event
3. **Wrong function name**: Case-sensitive, must match exactly

**Solution:**
```cpp
// In widget graph or C++
void UMyWidget::NativeConstruct()
{
  Super::NativeConstruct();
}

// Implement OnBrowserCreated event
void UMyWidget::OnBrowserCreated_Implementation()
{
  // NOW safe to call InvokeWeb functions
  InvokeWebString(TEXT("initialize"), TEXT("Game Started"));
}
```

---

### Input Not Working (Can't Click Buttons)

**Possible causes:**
1. **Widget not focusable**: Check "Is Focusable" and "Supports Keyboard Focus"
2. **Input mode not set to UI**: Need `SetInputMode(FInputModeUIOnly)`
3. **Z-order issue**: Widget hidden behind another widget

**Solution:**
```cpp
// Ensure proper input mode
APlayerController* PC = GetWorld()->GetFirstPlayerController();
FInputModeUIOnly InputMode;
InputMode.SetWidgetToFocus(YourWidget->TakeWidget());
PC->SetInputMode(InputMode);
PC->bShowMouseCursor = true;
```

---

## Next Steps

Once you've created `WBP_MainMenu`:

1. **Test it** in PIE with Vite dev server running
2. **Verify DevTools** show your React app loading
3. **Test clicking buttons** - check console for events
4. **Implement C++ binding** for New Game, Settings, Quit
5. **Create WBP_Settings** using the same process
6. **Build production** and test packaged build

---

## Additional Resources

- NEON Plugin Docs: http://neon.techtile.media
- Your main menu React code: `Content/UI/NEON/MainMenu/index.tsx`
- Your NEON bridge: `Content/UI/NEON/Components/Utils/neon-bridge.js`
- Backend subsystem: `Source/Noham_Base_Proj_Cpp/Public/Subsystems/NEON/NohamNEONBridgeSubsystem.h`
