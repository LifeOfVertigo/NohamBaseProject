# Next Steps: NEON Integration Complete Setup

## ✅ What We Just Completed

### 📚 Documentation Created
1. **NEON-Widget-Setup-Guide.md** - Comprehensive 300+ line guide covering:
   - NEON widget architecture explained
   - Step-by-step UMG widget creation
   - JavaScript ↔ C++ communication patterns
   - Development & production workflows
   - Complete troubleshooting section

2. **NEON-Widget-Quick-Start.md** - Quick reference checklist for:
   - Creating widgets in UE5 Editor
   - Testing workflow
   - Common fixes
   - File path references

### 💻 C++ Code Created

**NohamUIManagerSubsystem** (`Source/Noham_Base_Proj_Cpp/Subsystems/UI/`)
- Complete UI widget lifecycle management
- Show/hide widgets by name
- Input mode switching (UI, Game, GameAndUI)
- Widget caching and lazy loading
- Shortcut methods (`ShowMainMenu()`, `ShowSettings()`, etc.)

**NohamGameMode** (Updated)
- Added `BeginPlay()` override
- Added `bShowMainMenuOnStart` property
- Automatically shows main menu when game starts

---

## 🎯 YOUR NEXT STEPS (In Order)

### Step 1: Compile C++ Code

```bash
# Close UE5 Editor if open
# In terminal/command prompt:
cd "C:\Users\tyler\OneDrive\Documentos\Unreal Projects\NohamBaseProject\Noham_Base_Proj_Cpp"

# Generate project files
"C:\Program Files\Epic Games\UE_5.5\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" -projectfiles -project="Noham_Base_Proj_Cpp.uproject" -game -rocket -progress

# OR right-click Noham_Base_Proj_Cpp.uproject → "Generate Visual Studio project files"
```

**Then open in Visual Studio:**
1. Open `Noham_Base_Proj_Cpp.sln` in Visual Studio
2. Build Solution (Ctrl+Shift+B)
3. Fix any compilation errors if they appear
4. Launch UE5 Editor from Visual Studio (F5) or open .uproject

---

### Step 2: Start Vite Dev Server

**Open a new terminal/command prompt:**

```bash
cd "C:\Users\tyler\OneDrive\Documentos\Unreal Projects\NohamBaseProject\Noham_Base_Proj_Cpp\Content\UI\NEON"
npm run dev
```

**Expected output:**
```
  VITE v5.x.x  ready in XXX ms

  ➜  Local:   http://localhost:5173/
  ➜  Network: use --host to expose
```

**Leave this running!** Do not close this terminal.

---

### Step 3: Create WBP_MainMenu Widget in UE5

**Follow the checklist from `NEON-Widget-Quick-Start.md`:**

1. **Content Browser** → Navigate to `Content/UI/Widgets/`
2. **Right-click** → **User Interface** → **Widget Blueprint**
3. **Name**: `WBP_MainMenu`
4. **Open** the widget (double-click)
5. **File** → **Reparent Blueprint** → Select `UNEONWidget` → Click Reparent
6. **Add Image component** from Palette
7. **Rename to**: `_BrowserImage` (EXACTLY - case sensitive!)
8. **Set Anchors**: Min (0,0), Max (1,1), Offsets all 0
9. **Select root widget** (WBP_MainMenu in hierarchy)
10. **Details** → NEON category:
    - `_DevelopmentURL`: `http://localhost:5173/MainMenu/index.html`
    - `_LiveURL`: `file:///Content/UI/NEON/dist/MainMenu/index.html`
    - `_OpenDevTools`: `true`
    - `_MaxFPS`: `60`
11. **Details** → Behavior:
    - ✅ Is Focusable
    - ✅ Supports Keyboard Focus
12. **Compile** and **Save**

---

### Step 4: Test in Play-In-Editor

1. **Open your main level** (or create a test level)
2. **Click Play** (Alt+P)

**Expected Results:**
- Main menu appears automatically (thanks to GameMode integration)
- Chrome DevTools opens showing your React app
- Mouse cursor visible
- Can click/hover buttons
- No console errors

**Check UE5 Output Log for:**
```
LogTemp: [UI Manager] Subsystem initialized
LogTemp: [UI Manager] Registered 5 widget classes
LogTemp: [UI Manager] ShowWidget: MainMenu
LogTemp: [UI Manager] Created widget: MainMenu
LogNEON: Browser created successfully
LogNEON: Loaded URL: http://localhost:5173/MainMenu/index.html
LogTemp: [GameMode] Main menu shown on game start
```

---

### Step 5: Hook Up JavaScript to C++ (Menu Actions)

**Your React code already calls UE5 functions** (MainMenu/index.tsx:72-102), but they need to be bound in C++.

**Two options:**

#### Option A: Use Existing NEON Bridge Subsystem (Recommended)

Update `NohamNEONBridgeSubsystem.cpp` to properly integrate with NEON's C++ API.

**You'll need to:**
1. Research NEON plugin docs (http://neon.techtile.media) for binding functions
2. Implement proper JavaScript → C++ message routing
3. Bind the functions: `new_game`, `continue_game`, `open_settings`, `quit_game`

#### Option B: Create Custom Widget Class (Alternative)

Create `UNohamMainMenuWidget` that inherits from `UNEONWidget` and binds functions directly.

**For now, test with logging:**
The existing placeholder implementation in `NohamNEONBridgeSubsystem` already logs when functions are called.

---

### Step 6: Verify Communication

**In Chrome DevTools (opens when widget loads):**

```javascript
// Test UE5 → JavaScript
// (Run this in DevTools console)
window.test_function = (data) => {
  console.log('Received from UE5:', data);
};
```

**Then in UE5 Output Log:**
You should see blueprint or C++ calling `InvokeWebString` and the JavaScript function receiving it.

---

## 🐛 Troubleshooting

### Widget Shows Blank/Black Screen

**Check:**
1. ✅ Vite dev server is running (`npm run dev`)
2. ✅ URL is correct: `http://localhost:5173/MainMenu/index.html`
3. ✅ Image component named exactly `_BrowserImage`
4. ✅ No firewall blocking localhost:5173

**Try:**
- Open `http://localhost:5173/MainMenu/index.html` in your regular browser
- Check UE5 Output Log for errors
- Enable DevTools (`_OpenDevTools = true`) to see browser console

---

### Widget Not Showing At All

**Check:**
1. ✅ GameMode is `ANohamGameMode` (or Blueprint child)
2. ✅ `bShowMainMenuOnStart = true` in GameMode
3. ✅ UI Manager subsystem initialized (check Output Log)

**Try:**
- Manually show widget using Blueprint:
  ```
  Event BeginPlay
    └─> Get Game Instance
        └─> Get Subsystem (NohamUIManagerSubsystem)
            └─> Show Main Menu
  ```

---

### Compilation Errors

**Common issues:**

**"Cannot open include file: 'Subsystems/UI/NohamUIManagerSubsystem.h'"**
- Solution: Regenerate project files, rebuild solution

**"UNohamUIManagerSubsystem is not defined"**
- Solution: Ensure Build.cs includes "UMG" module (already done)

**Linker errors (LNK2019)**
- Solution: Clean and rebuild solution

---

## 📦 When Ready for Production Build

### 1. Build Web Assets

```bash
cd "C:\Users\tyler\OneDrive\Documentos\Unreal Projects\NohamBaseProject\Noham_Base_Proj_Cpp\Content\UI\NEON"
npm run build
```

This creates `Content/UI/NEON/dist/` folder with production builds.

### 2. Verify LiveURL

Ensure widget `_LiveURL` property points to:
```
file:///Content/UI/NEON/dist/MainMenu/index.html
```

### 3. Package Game

**In UE5:**
1. **File** → **Package Project** → **Windows** (or target platform)
2. NEON will automatically use `_LiveURL` in packaged builds
3. The `dist/` folder will be included in the pak file

---

## 🎨 Creating Additional Widgets

Use the same process for:

- **WBP_Settings** → Points to `Settings/index.html`
- **WBP_PauseMenu** → Points to `PauseMenu/index.html` (needs to be created)
- **WBP_HUD** → Points to `HUD/index.html` (needs to be created)

**UI Manager already has shortcuts:**
```cpp
UIManager->ShowSettings();   // Shows WBP_Settings
UIManager->ShowPauseMenu();  // Shows WBP_PauseMenu
UIManager->ShowHUD();        // Shows WBP_HUD
UIManager->ReturnToGame();   // Hides all, restores game input
```

---

## 📝 Files Created in This Session

### Documentation
- `docs/guides/NEON-Widget-Setup-Guide.md` - Complete setup guide
- `docs/guides/NEON-Widget-Quick-Start.md` - Quick reference checklist
- `docs/guides/NEXT-STEPS-NEON-Integration.md` - This file

### C++ Headers
- `Source/Noham_Base_Proj_Cpp/Public/Subsystems/UI/NohamUIManagerSubsystem.h`

### C++ Implementation
- `Source/Noham_Base_Proj_Cpp/Private/Subsystems/UI/NohamUIManagerSubsystem.cpp`
- `Source/Noham_Base_Proj_Cpp/Private/Subsystems/Core/NohamGameMode.cpp` (updated)
- `Source/Noham_Base_Proj_Cpp/Public/Subsystems/Core/NohamGameMode.h` (updated)

### Still Need to Create in UE5 Editor
- `Content/UI/Widgets/WBP_MainMenu.uasset` ← **YOU CREATE THIS**
- `Content/UI/Widgets/WBP_Settings.uasset` ← **YOU CREATE THIS**
- `Content/UI/Widgets/WBP_PauseMenu.uasset` ← Future
- `Content/UI/Widgets/WBP_HUD.uasset` ← Future

---

## 🎯 Success Criteria

You'll know everything is working when:

1. ✅ C++ compiles without errors
2. ✅ UE5 Editor opens without crashes
3. ✅ Vite dev server runs on port 5173
4. ✅ Pressing Play shows main menu automatically
5. ✅ Chrome DevTools opens with React app loaded
6. ✅ Can click buttons with hover effects
7. ✅ Output Log shows UI Manager + NEON initialization
8. ✅ No JavaScript console errors in DevTools

---

## 🚀 Ready to Go!

**Immediate next actions:**

1. ⚡ **Compile C++ code** (regenerate project files if needed)
2. ⚡ **Start Vite** (`npm run dev` in Content/UI/NEON/)
3. ⚡ **Create WBP_MainMenu** (follow Quick Start checklist)
4. ⚡ **Test in PIE** (click Play and verify main menu appears)
5. ⚡ **Check DevTools** for React app and test button clicks

**Once that works:**

6. Hook up NEON JavaScript → C++ binding (research NEON plugin docs)
7. Create WBP_Settings widget
8. Test navigation between menus
9. Build for production and test packaged build

---

**Good luck! You're 90% there.** The hard architectural work is done - now it's just following the widget creation checklist and testing. 🎉
