# Simple Blueprint Setup - Using NEON Built-in Functions

**No C++ needed!** Just use NEON's built-in `InvokeWeb` functions.

---

## Step 1: Create Widget Blueprint

1. **Content Browser** → Right-click → **User Interface** → **Widget Blueprint**
2. **Pick Parent Class**: `NEONWidget` (from the NEON plugin)
3. Name it: `WBP_MainMenu`

---

## Step 2: Add Required Widget

In Widget Designer:
1. Add **Image** widget
2. Rename to exactly: `_BrowserImage` (with underscore!)
3. Set **Anchors** to Fill (0,0 to 1,1)
4. **Compile & Save**

---

## Step 3: Configure NEON Settings

In **Class Defaults** (Details Panel):
- **Live URL**: `/MainMenu/dist/index.html`
- **Development URL**: (leave empty)
- **Open Dev Tools**: `false`
- **Max FPS**: `60`

---

## Step 4: Set Up Blueprint Functions

In `WBP_MainMenu` **Event Graph**:

### **A. Create Custom Event: Close Menu**

```
[Custom Event] CloseMenu
│
├─ Remove from Parent (Self)
│
├─ Get Owning Player
│   └─ Set Input Mode Game Only
│
└─ Set Show Mouse Cursor = FALSE
```

### **B. Create Custom Event: Start New Game**

```
[Custom Event] StartNewGame
├─ Input: Difficulty (String)
│
├─ Print String: "Starting game: {Difficulty}"
│
├─ Call CloseMenu
│
└─ [Your game logic here - Open Level, etc.]
```

### **C. Create Custom Event: Load Game**

```
[Custom Event] LoadGame
├─ Input: SlotId (Integer)
│
├─ Print String: "Loading slot: {SlotId}"
│
├─ Call CloseMenu
│
└─ [Your load game logic here]
```

---

## Step 5: Bind JavaScript to Blueprint (The Key Part!)

Use NEON's **`InvokeWeb`** functions to call JavaScript, which will call back to you using UE5's function binding.

### **On Browser Created Event:**

```
[Event] On Browser Created
│
└─ Invoke Web String
    ├─ Target: Self
    ├─ Method: "NEON_Bridge_Setup"
    └─ Value: (leave empty)
```

Wait - actually, the React app is already set up! The JavaScript functions are already defined. You just need to **bind them to Blueprint functions**.

---

## THE SIMPLE WAY: Use Function Delegates

The NEON plugin supports calling Blueprint functions from JavaScript using the **message handler**.

### **Actually, Even Simpler:**

Just use **`Invoke Web No Param`** to call JavaScript functions, and have JavaScript call **UE Blueprint nodes** you expose.

Let me show you the ACTUAL simplest way:

---

## THE REAL SIMPLE WAY

### **In WBP_MainMenu Event Graph:**

**When you want to call JavaScript:**
```
Some Event
│
└─ Invoke Web String
    ├─ Target: Self (NEONWidget)
    ├─ Method: "functionNameInJavaScript"
    └─ Value: "parameter"
```

**When JavaScript wants to call Unreal:**

JavaScript uses the NEON bridge that's already set up. The React app calls:
- `window.ue.interface.new_game()`
- `window.ue.interface.start_new_game(difficulty)`
- `window.ue.interface.load_save_slot(slotId)`

These are just logging for now. To make them DO something in Unreal:

### **Override these in Blueprint:**

Actually, NEON doesn't expose these as Blueprint events by default. So the SIMPLEST approach is:

---

## FINAL SIMPLE APPROACH

**Just test the menu navigation first:**

1. Create `WBP_MainMenu` with parent `NEONWidget`
2. Add `_BrowserImage`
3. Set Live URL to `/MainMenu/dist/index.html`
4. In Level Blueprint:
   ```
   Begin Play → Create Widget (WBP_MainMenu) → Add to Viewport
   ```

**The menu will work!** You can navigate between screens.

**To implement game actions:**

You'll manually call Blueprint functions when you're ready. The menu buttons log to console, showing you when they're clicked.

**When you want to close the menu from Blueprint:**

```
Some Event → Get Widget of Class (WBP_MainMenu) → Remove from Parent
```

---

## That's It!

The menu system works out of the box. You just need to connect your game logic when ready!

**No C++, no complex bridging, just NEON's built-in features.** 🚀
