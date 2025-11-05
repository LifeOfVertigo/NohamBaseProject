# Simple Unreal Engine Menu Integration (Blueprint Only)

## The Simplest Solution

Since NEON's `InvokeUnreal` isn't directly accessible from Blueprint, we'll use the **InvokeWeb functions** instead, which work in reverse: Unreal calls JavaScript functions.

---

## Solution: Let Unreal Handle Game Start

Instead of JavaScript calling back to close the menu, **have Unreal listen for when to start the game**.

### **Step 1: Create WBP_MainMenu**

1. Create Widget Blueprint
2. **Reparent to**: `NEONWidget`
3. Add `_BrowserImage` (Image widget)
4. Set **Live URL**: `/MainMenu/dist/index.html`

### **Step 2: In Level Blueprint or Game Mode**

**Show Menu:**
```
[Event] Begin Play
│
├─ Create Widget
│   └─ Class: WBP_MainMenu
│   └─ Return → MainMenuRef
│
├─ Add to Viewport
│   └─ Target: MainMenuRef
│
├─ Set Input Mode UI Only
│   └─ Player Controller: Get Player Controller
│
└─ Set Show Mouse Cursor = TRUE
```

**Check for Game Start (Polling Method):**
```
[Event] Event Tick
│
├─ Is Valid? (MainMenuRef)
│   │
│   └─ [Yes]
│       │
│       └─ Invoke Web No Param
│           ├─ Target: MainMenuRef
│           └─ Method: "checkGameStart"
│
└─ [No] → Do nothing
```

Wait, this is getting complicated. Let me provide you the **absolute easiest solution**:

---

## **EASIEST SOLUTION: Use Unreal's Built-in Close Function**

Just add a **simple custom event** that you call manually or from a timer.

### In WBP_MainMenu Event Graph:

**Custom Event: Start New Game**
```
[Custom Event] StartNewGame (Input: String "Difficulty")
│
├─ Print String: "Starting game with {Difficulty}"
│
├─ Remove from Parent (Self)
│
├─ Get Owning Player
│   └─ Set Input Mode Game Only
│
└─ Set Show Mouse Cursor = FALSE
```

**Then in your Level Blueprint:**

```
[Event] Begin Play
│
├─ Create Widget: WBP_MainMenu → MainMenuRef
│
├─ Add to Viewport
│
├─ Set Input Mode UI Only
│
├─ Set Show Mouse Cursor = TRUE
│
└─ Delay (5 seconds) ← Just for testing
    │
    └─ Start New Game (Target: MainMenuRef, Difficulty: "Normal")
```

This tests that the close functionality works.

---

## **BETTER: Use a Blueprint Function Library (Recommended)**

I'll create a simple C++ function library that exposes the menu closing to both Blueprint AND JavaScript.

Let me create that for you...

