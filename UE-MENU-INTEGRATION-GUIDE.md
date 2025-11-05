# Unreal Engine Menu Integration Guide

## What's Been Set Up

The React menu system now calls `close_menu_widget()` when:
- **Start Game** button is clicked (from New Game menu)
- **Load Selected Save** button is clicked (from Load Game menu)
- **Resume** button is clicked (from Pause menu)

This allows the menu to hand control back to the player character.

---

## Step-by-Step: Hook Up in Unreal Engine Blueprint

### 1. Create a Menu Widget Blueprint

If you don't already have one:
1. **Content Browser** → Right-click → **User Interface** → **Widget Blueprint**
2. Name it: `WBP_MainMenu` (or similar)
3. Inside the widget:
   - Add a **NEON Widget** component
   - Set its **Live URL** to `/MainMenu/dist/index.html`

### 2. Create Blueprint Functions to Handle Menu Closure

In your **Player Controller** or **Game Mode** blueprint:

#### **A. Function: Show Main Menu**

```
Event: ShowMainMenu
│
├─ Create Widget (Class: WBP_MainMenu)
│   └─ Return Value → [Store in variable: MainMenuWidget]
│
├─ Add to Viewport (Target: MainMenuWidget)
│
├─ Set Input Mode UI Only
│   ├─ Player Controller: Self
│   └─ Widget to Focus: MainMenuWidget
│
└─ Set Show Mouse Cursor = True
```

#### **B. Function: Close Main Menu**

```
Event/Function: CloseMainMenu
│
├─ Is Valid? (MainMenuWidget)
│   │
│   └─ [Yes]
│       │
│       ├─ Remove from Parent (Target: MainMenuWidget)
│       │
│       ├─ Set Input Mode Game Only
│       │   └─ Player Controller: Self
│       │
│       └─ Set Show Mouse Cursor = False
│
└─ [No] → Do nothing
```

### 3. Bind the NEON Bridge Function

In your **WBP_MainMenu** widget or **Player Controller**:

#### **Event Graph:**

```
Event Construct (or Event Begin Play)
│
└─ NEON Widget → Invoke Unreal
    │
    ├─ Method: "close_menu_widget"
    │
    └─ Bind to Event: CloseMainMenu
```

**Or in C++ (UNEONWidget.cpp):**

```cpp
// In your widget's NativeConstruct or similar initialization
void UYourMenuWidget::BindNEONFunctions()
{
    // Get reference to player controller or game mode
    APlayerController* PC = GetOwningPlayer();

    if (PC)
    {
        // Bind close_menu_widget to a C++ function
        FString Script = TEXT("window.ue.interface.close_menu_widget = function() { ue.interface.invoke_unreal('CloseMenu'); };");
        NEONWidget->ExecuteJavaScript(Script);
    }
}

// Handle the invoke from JavaScript
void UYourMenuWidget::InvokeUnreal(const FString& Data)
{
    if (Data == "CloseMenu")
    {
        CloseMenu();
    }
}

void UYourMenuWidget::CloseMenu()
{
    // Remove widget from viewport
    RemoveFromParent();

    // Restore game input
    APlayerController* PC = GetOwningPlayer();
    if (PC)
    {
        FInputModeGameOnly InputMode;
        PC->SetInputMode(InputMode);
        PC->bShowMouseCursor = false;
    }
}
```

---

## Blueprint Example (Visual)

### **Player Controller Blueprint:**

**Event: Begin Play**
```
Begin Play
│
└─ Show Main Menu (call custom function)
```

**Custom Event: Close Main Menu (Callable from NEON)**
```
[Custom Event] CloseMainMenu
│
├─ Is Valid? (MainMenuWidget)
│   └─ Remove from Parent
│
├─ Set Input Mode Game Only
│
└─ Set Show Mouse Cursor = False
```

**Custom Event: On Start New Game**
```
[Custom Event] OnStartNewGame (String: Difficulty)
│
├─ Close Main Menu
│
├─ Print String: "Starting game with difficulty: {Difficulty}"
│
└─ Open Level (Level Name: "YourGameLevel")
```

---

## NEON Bridge Functions Available

The React menu system calls these functions when needed:

| Function | When Called | Purpose |
|----------|-------------|---------|
| `close_menu_widget()` | Start Game, Load Game, Resume | Closes menu and returns control to player |
| `new_game()` | Main Menu → New Game | Navigate to New Game screen |
| `continue_game()` | Main Menu → Load Game | Navigate to Load Game screen |
| `open_settings()` | Main Menu → Settings | Navigate to Settings screen |
| `quit_game()` | Main Menu → Quit | Exit the game |
| `start_new_game(difficulty)` | New Game → Start | Start new game with selected difficulty |
| `load_save_slot(slotId)` | Load Game → Load | Load specific save slot |
| `back_to_main_menu()` | Any menu → Back | Return to main menu |
| `apply_settings(settings)` | Settings → Apply | Apply graphics/audio settings |

---

## Quick Start Checklist

- [ ] Create `WBP_MainMenu` widget blueprint
- [ ] Add NEON Widget component with Live URL: `/MainMenu/dist/index.html`
- [ ] Create `CloseMainMenu` function in Player Controller
- [ ] Bind `close_menu_widget` NEON function to `CloseMainMenu` event
- [ ] Test: Click "New Game" → Select difficulty → Click "Start Game"
- [ ] Verify: Menu closes, mouse cursor disappears, game input is restored

---

## Testing the Integration

1. **Play in Editor (PIE)**
2. Menu should appear with mouse cursor
3. Click: **New Game** → **Normal** → **Start Game**
4. Expected behavior:
   - Console log: `[NEON] Starting new game with difficulty: normal`
   - Menu closes
   - Mouse cursor disappears
   - Player has full control

---

## Next Steps

Once this works, you can extend it:
- Add actual level loading in `start_new_game`
- Hook up save/load system to `load_save_slot`
- Implement settings in `apply_settings`
- Add pause menu trigger (ESC key → show pause menu)

The foundation is ready for your game! 🚀
