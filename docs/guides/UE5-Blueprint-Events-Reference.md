# UE5 Blueprint Events Reference for NEON Menus

**Quick reference for creating Blueprint events to handle menu button presses**

---

## 🎯 Current Menu Requirements

Based on the fixed TypeScript menu implementations, you need to create the following Custom Events in your UE5 Widget Blueprints.

---

## 📋 Required Custom Events (MainMenu)

Create these in your **WBP_MainMenu** Blueprint:

### 1. OnInvoke_NewGame
**JavaScript Call:**
```javascript
NEON.invokeUnrealEvent('NewGame', {});
```

**Blueprint Setup:**
1. Right-click → **Add Custom Event**
2. Name: `OnInvoke_NewGame`
3. No parameters needed (empty object)
4. Connect to your new game logic

**Example Logic:**
```
[OnInvoke_NewGame]
  └─> [Remove from Parent]
      └─> [Open Level: "NewGameMenu"]
```

---

### 2. OnInvoke_LoadGame
**JavaScript Call:**
```javascript
NEON.invokeUnrealEvent('LoadGame', {});
```

**Blueprint Setup:**
1. Add Custom Event: `OnInvoke_LoadGame`
2. No parameters
3. Connect to load game menu logic

**Example Logic:**
```
[OnInvoke_LoadGame]
  └─> [Remove from Parent]
      └─> [Open Load Game Widget]
```

---

### 3. OnInvoke_OpenSettings
**JavaScript Call:**
```javascript
NEON.invokeUnrealEvent('OpenSettings', {});
```

**Blueprint Setup:**
1. Add Custom Event: `OnInvoke_OpenSettings`
2. No parameters
3. Connect to settings menu logic

**Example Logic:**
```
[OnInvoke_OpenSettings]
  └─> [Remove from Parent]
      └─> [Create Widget: WBP_Settings]
          └─> [Add to Viewport]
```

---

### 4. OnInvoke_QuitGame
**JavaScript Call:**
```javascript
NEON.invokeUnrealEvent('QuitGame', {});
```

**Blueprint Setup:**
1. Add Custom Event: `OnInvoke_QuitGame`
2. No parameters
3. Connect to quit game logic

**Example Logic:**
```
[OnInvoke_QuitGame]
  └─> [Print String: "Quitting Game"]
      └─> [Quit Game]
```

---

### 5. OnInvoke_LogMessage
**JavaScript Call:**
```javascript
NEON.invokeUnrealEvent('LogMessage', { message: 'Escape pressed on main menu' });
```

**Blueprint Setup:**
1. Add Custom Event: `OnInvoke_LogMessage`
2. Add parameter: `message` (String)
3. Connect to logging logic

**Example Logic:**
```
[OnInvoke_LogMessage]
  ├─ message (String)
  └─> [Print String: message]
```

---

## 📋 Required Custom Events (NewGame Menu)

Create these in your **WBP_NewGame** Blueprint:

### 6. OnInvoke_StartNewGame
**JavaScript Call:**
```javascript
NEON.invokeUnrealEvent('StartNewGame', { difficulty: selectedDifficulty });
```

**Blueprint Setup:**
1. Add Custom Event: `OnInvoke_StartNewGame`
2. Add parameter: `difficulty` (String)
3. Connect to game start logic

**Example Logic:**
```
[OnInvoke_StartNewGame]
  ├─ difficulty (String)
  └─> [Set Game Difficulty: difficulty]
      └─> [Open Level: "MainGameLevel"]
```

---

### 7. OnInvoke_BackToMainMenu (NewGame)
**JavaScript Call:**
```javascript
NEON.invokeUnrealEvent('BackToMainMenu', {});
```

**Blueprint Setup:**
1. Add Custom Event: `OnInvoke_BackToMainMenu`
2. No parameters
3. Connect to back navigation logic

**Example Logic:**
```
[OnInvoke_BackToMainMenu]
  └─> [Remove from Parent]
      └─> [Create Widget: WBP_MainMenu]
          └─> [Add to Viewport]
```

---

## 📋 Required Custom Events (Settings Menu)

Create these in your **WBP_Settings** Blueprint:

### 8. OnInvoke_ApplySettings
**JavaScript Call:**
```javascript
NEON.invokeUnrealEvent('ApplySettings', {
  graphics: { resolution, quality, vsync, brightness },
  audio: { master, music, sfx }
});
```

**Blueprint Setup:**
1. Add Custom Event: `OnInvoke_ApplySettings`
2. Add parameter: `graphics` (JSONObject)
3. Add parameter: `audio` (JSONObject)
4. Connect to settings application logic

**Example Logic:**
```
[OnInvoke_ApplySettings]
  ├─ graphics (JSONObject)
  ├─ audio (JSONObject)
  │
  ├─> [Parse JSON: graphics]
  │     └─> [Apply Graphics Settings]
  │
  └─> [Parse JSON: audio]
        └─> [Apply Audio Settings]
            └─> [Save Config]
                └─> [Print String: "Settings Applied"]
```

**Parsing JSON in Blueprint:**
```
[Parse JSON: graphics]
  └─> [Get Field: "resolution"]
  └─> [Get Field: "quality"]
  └─> [Get Field: "vsync"]
  └─> [Get Field: "brightness"]
```

---

### 9. OnInvoke_BackToMainMenu (Settings)
**JavaScript Call:**
```javascript
NEON.invokeUnrealEvent('BackToMainMenu', {});
```

**Blueprint Setup:**
- Same as #7 above (can be in multiple widgets)

---

## 🔧 Testing Your Events

### Step 1: Add Debug Print Strings
For each event, temporarily add a Print String node:

```
[OnInvoke_NewGame]
  └─> [Print String: "NewGame event received!"]
      └─> [Your Logic]
```

### Step 2: Test in PIE
1. Start Vite dev server: `npm run dev`
2. Play in Editor (Alt+P)
3. Click menu buttons
4. Check **Output Log** for:
   - JavaScript console logs: `[NEON] Called UE5 event: NewGame`
   - Blueprint print strings: "NewGame event received!"

### Step 3: Common Issues

| Problem | Solution |
|---------|----------|
| Event not firing | Check spelling - must be exactly `OnInvoke_NewGame` |
| Parameters undefined | Verify parameter types (String vs JSONObject) |
| JavaScript error "NEON not defined" | Ensure widget parent is `UNEONWidget` |
| No logs in Output Log | Check Vite dev server is running |

---

## 📊 Complete Event Checklist

Use this checklist when setting up your Blueprint:

**MainMenu (WBP_MainMenu):**
- [ ] OnInvoke_NewGame
- [ ] OnInvoke_LoadGame
- [ ] OnInvoke_OpenSettings
- [ ] OnInvoke_QuitGame
- [ ] OnInvoke_LogMessage

**NewGame Menu (WBP_NewGame):**
- [ ] OnInvoke_StartNewGame (with `difficulty` parameter)
- [ ] OnInvoke_BackToMainMenu

**Settings Menu (WBP_Settings):**
- [ ] OnInvoke_ApplySettings (with `graphics` and `audio` JSONObject parameters)
- [ ] OnInvoke_BackToMainMenu

---

## 💡 Pro Tips

1. **Start Simple:** Create events with just Print String nodes first to verify connectivity
2. **Consistent Naming:** Use PascalCase in JavaScript (`NewGame`) to match UE5 conventions
3. **Debug Early:** Add console logs on both JavaScript and Blueprint sides
4. **Test Incrementally:** Test each button individually before moving to the next
5. **JSON Parsing:** Use UE5's JSON parsing nodes for complex parameters

---

## 📚 Related Documentation

- [NEON Communication Guide](NEON-Communication-Guide.md) - Complete API reference
- [API Specification](../architecture/api-specification-neon-based.md) - Full system architecture
- [NEON Widget Quick Start](NEON-Widget-Quick-Start.md) - Widget setup checklist

---

## ✅ Success Criteria

You know it's working when:

1. ✅ Click "New Game" → Output Log shows event received
2. ✅ Click "Settings" → Settings widget opens
3. ✅ Click "Quit Game" → Game exits gracefully
4. ✅ No JavaScript errors in browser DevTools
5. ✅ All menu navigation flows smoothly

---

**Last Updated:** After fixing all menu TypeScript implementations
**Status:** ✅ Ready for Blueprint Implementation
