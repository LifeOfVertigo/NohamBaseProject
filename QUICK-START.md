# Quick Start - Menu Integration

## Current Status

✅ **C++ Code Written** - `MainMenuWidget.h` and `MainMenuWidget.cpp` are ready
✅ **React Menus Built** - All menus deployed to `Noham_Base_Proj_Cpp/NEON/MainMenu/dist/`
⏳ **Needs Compile** - Run the build script below

---

## Step 1: Compile C++ Code

**Double-click:** `BuildProject.bat` (in this folder)

This will compile the MainMenuWidget class.

**Alternative:** Right-click `Noham_Base_Proj_Cpp.uproject` → "Generate Visual Studio project files" → Open `.sln` → Build Solution

---

## Step 2: Create Widget Blueprint

Once compiled:

1. Open Unreal Engine
2. **Content Browser** → Right-click → **User Interface** → **Widget Blueprint**
3. **Pick Parent Class**: `MainMenuWidget`
4. Name it: `WBP_MainMenu`
5. Open it

---

## Step 3: Add Required Widget

In Widget Designer:
1. Add **Image** widget
2. Rename to: `_BrowserImage`
3. Set **Anchors** to Fill (0,0 to 1,1)
4. **Compile & Save**

---

## Step 4: Configure NEON Settings

In **Class Defaults** (Details Panel):
- **Live URL**: `/MainMenu/dist/index.html`
- **Open Dev Tools**: `false`
- **Max FPS**: `60`

---

## Step 5: Add Blueprint Events

In **Event Graph**, add these 3 events:

### **Event: Close Menu**
```
Right-click → Add Event → Event Close Menu

Actions:
1. Remove from Parent (Self)
2. Get Owning Player → Set Input Mode Game Only
3. Set Show Mouse Cursor = FALSE
```

### **Event: Start New Game**
```
Right-click → Add Event → Event Start New Game

Actions:
1. Print String: "Starting game: {Difficulty}"
2. Remove from Parent (Self)
3. Set Input Mode Game Only
4. Set Show Mouse Cursor = FALSE
```

### **Event: Load Game**
```
Right-click → Add Event → Event Load Game

Actions:
1. Print String: "Loading slot: {Slot Id}"
2. Remove from Parent (Self)
3. Set Input Mode Game Only
4. Set Show Mouse Cursor = FALSE
```

---

## Step 6: Add Polling Logic (IMPORTANT!)

Since JavaScript can't directly call Unreal, we poll for messages:

### **In WBP_MainMenu Event Graph:**

```
Event Tick
│
└─ Invoke Web String
    ├─ Target: Self
    ├─ Method: "getLastMenuMessage"
    └─ Return Value → MessageFromJS
        │
        └─ Is Not Empty?
            │
            └─ Handle JavaScript Message
                └─ Message: MessageFromJS
```

**Simplified:**
1. Add **Event Tick**
2. Call **Invoke Web String** with method `"getLastMenuMessage"`
3. If result is not empty, call **Handle JavaScript Message** with the result

---

## Step 7: Test

### **In Level Blueprint:**

```
Event Begin Play
│
├─ Create Widget: WBP_MainMenu → MenuRef
├─ Add to Viewport (MenuRef)
├─ Set Input Mode UI Only
└─ Set Show Mouse Cursor = TRUE
```

**Play in Editor:**
- Menu appears
- Click: New Game → Normal → Start Game
- Console should show: "Starting game: normal"
- Menu closes, mouse disappears

---

## Done!

You now have a fully working React menu system integrated with Unreal Engine!

**See `FINAL-MENU-SETUP-GUIDE.md` for more details.**
