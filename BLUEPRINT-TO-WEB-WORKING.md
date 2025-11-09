# ✅ Blueprint → Web Communication - WORKING

**Status**: **FULLY FUNCTIONAL** as of 2025-11-09

---

## Achievement Unlocked 🎉

Successfully established **bidirectional communication** between Unreal Engine 5 Blueprints and React-based NEON web UI.

### What Works:

✅ **Blueprint → Web**: UE5 Blueprints can send JSON data to React UI
✅ **Web → Blueprint**: React UI can trigger Blueprint events
✅ **NEON Library**: Properly integrated and loading globally
✅ **Build System**: Automated deployment from React source to UE5 project

---

## Quick Start - Sending Data from Blueprint to Web

### In Blueprint (WBP_MainMenu):

```
[Event] Event Construct (or OnBrowserCreated)
    │
    └─> [Delay] (2.0 seconds)
        │
        └─> [Invoke Web String]
            ├─ Target: Self
            ├─ Method: "UpdateMenuText"
            └─ Value: {"text": "Hello from Unreal!"}
```

### In React (shadcn-base/src/MainMenu.tsx):

```typescript
useEffect(() => {
  const NEON = (window as any).NEON;

  NEON.onInvoke('UpdateMenuText', (data: any) => {
    console.log('Received from Blueprint:', data);
    // Update your UI with data.text
    setMenuText(data.text);
  });
}, []);
```

---

## Architecture

### Communication Flow:

```
┌─────────────────────────────────────────────────────┐
│                  Unreal Engine 5                     │
│                                                      │
│  Blueprint → InvokeWebString("method", JSON)        │
│      ↓                                               │
│  NEON C++ Plugin (UNEONWidget)                      │
│      ↓                                               │
│  CEF Browser → window.NEON_Bridge_Web_Invoke()      │
└─────────────────────────────────────────────────────┘
                       ↓
┌─────────────────────────────────────────────────────┐
│              Chromium Embedded Framework             │
│                  (NEON Browser)                      │
│                                                      │
│  JavaScript → NEON.invoke(method, data)             │
│      ↓                                               │
│  Callback Registry → callbacks[method](data)        │
└─────────────────────────────────────────────────────┘
                       ↓
┌─────────────────────────────────────────────────────┐
│                  React Application                   │
│                                                      │
│  Registered Callback Executes                       │
│      ↓                                               │
│  setState() → UI Updates ✅                         │
└─────────────────────────────────────────────────────┘
```

---

## Technical Implementation Details

### NEON Library Loading

**Problem Solved**: NEON library must load **before** React to register callbacks.

**Solution**: NEON JavaScript is **inlined** in the HTML `<head>`:

```html
<!-- shadcn-base/index.html -->
<head>
  <script>
    // NEON library code inlined here (loads synchronously)
    var NEON = (function() { ... })();
  </script>
  <script type="module" src="...react.js"></script>
</head>
```

This ensures `window.NEON` is available when React loads.

### Build System

**Source**: `shadcn-base/src/MainMenu.tsx`
**Build**: `npm run build:ue`
**Output**: `Noham_Base_Proj_Cpp/*/dist/` (deployed to UE5 project)

Build automatically:
1. Compiles TypeScript → JavaScript
2. Inlines NEON library in HTML
3. Copies to both UE project NEON locations
4. Ready for UE5 to load

---

## Key Files

### Blueprint Side:
- `Noham_Base_Proj_Cpp/Content/Widgets/CoreMenuWidgets/WBP_MainMenu.uasset`
  - NEON widget (parent class: UNEONWidget)
  - Contains `InvokeWebString` calls

### React Side:
- `shadcn-base/src/MainMenu.tsx`
  - Registers NEON callbacks
  - Handles UI updates from Blueprint data

### NEON Library:
- `neon-ue-web-main/neon-browser.js`
  - Official NEON library (JavaScript version)
  - Inlined into HTML during build

### Build Configuration:
- `shadcn-base/package.json`
  - `npm run build:ue` command
  - Auto-deploys to UE5 project

---

## Usage Examples

### Example 1: Send Player Stats to UI

**Blueprint:**
```
[Get Player Stats]
    │
    └─> [Invoke Web String]
        ├─ Method: "UpdatePlayerStats"
        └─ Value: {"level": 42, "health": 100, "score": 1500}
```

**React:**
```typescript
NEON.onInvoke('UpdatePlayerStats', (data) => {
  setPlayerLevel(data.level);
  setPlayerHealth(data.health);
  setPlayerScore(data.score);
});
```

### Example 2: Show Notification

**Blueprint:**
```
[Show Achievement]
    │
    └─> [Invoke Web String]
        ├─ Method: "ShowNotification"
        └─ Value: {"message": "Achievement Unlocked!", "type": "success"}
```

**React:**
```typescript
NEON.onInvoke('ShowNotification', (data) => {
  toast({
    title: data.message,
    variant: data.type
  });
});
```

### Example 3: Update Game State

**Blueprint:**
```
[On Game State Changed]
    │
    └─> [Invoke Web String]
        ├─ Method: "UpdateGameState"
        └─ Value: {"paused": true, "currentLevel": "Level_02"}
```

**React:**
```typescript
NEON.onInvoke('UpdateGameState', (data) => {
  setIsPaused(data.paused);
  setCurrentLevel(data.currentLevel);
});
```

---

## Debugging

### Enable NEON Dev Tools:

In **WBP_MainMenu** Blueprint:
1. Select widget root
2. Find **"Open Dev Tools"** checkbox (NEON category)
3. ✅ Enable it
4. Compile & Play

**Result**: Chromium DevTools opens showing browser console.

### Console Output:

**Expected logs when working:**
```javascript
[NEON] Library loaded and attached to window
[NEON] NEON API available: true
[NEON] Registering UpdateMenuText callback...
[NEON] ✅ UpdateMenuText callback registered successfully
[NEON] Invoke NEON web callback UpdateMenuText {text: "..."}
```

### Common Issues:

| Issue | Cause | Solution |
|-------|-------|----------|
| "callback not found" | Callback not registered | Check React useEffect executed |
| NEON is undefined | Library didn't load | Verify neon.js in HTML |
| No message received | Blueprint not calling | Check InvokeWebString node |
| Wrong data format | Invalid JSON | Ensure `{"key": "value"}` format |

---

## Reverse Communication (Web → Blueprint)

### From React to Blueprint:

**React:**
```typescript
const NEON = (window as any).NEON;

NEON.invokeUnrealEvent('QuitGame', {});
NEON.invokeUnrealEvent('StartNewGame', { difficulty: 'hard' });
```

**Blueprint:**

Create Custom Events:
- `OnInvoke_QuitGame`
- `OnInvoke_StartNewGame`

These events fire when React calls the corresponding NEON function.

---

## Best Practices

### 1. **Method Naming Convention**
- Use PascalCase: `UpdatePlayerStats`, `ShowNotification`
- Be descriptive: `UpdateMenuText` (not just `Update`)
- Match Blueprint to React exactly (case-sensitive!)

### 2. **JSON Data Format**
- Always use valid JSON: `{"key": "value"}`
- Quote property names: `{"text": "..."}` ✅ not `{text: "..."}` ❌
- Escape quotes in Blueprint: `{\"text\": \"Hello\"}`

### 3. **Error Handling**
```typescript
NEON.onInvoke('UpdateData', (data) => {
  try {
    if (!data || !data.text) {
      console.error('Invalid data received:', data);
      return;
    }
    setMenuText(data.text);
  } catch (error) {
    console.error('Error processing data:', error);
  }
});
```

### 4. **Register Callbacks Early**
- Use `useEffect` with empty dependency array `[]`
- Register all callbacks on component mount
- Callbacks stay active for component lifetime

### 5. **Verbose Logging (Development)**
```typescript
NEON.setVerbose(true); // Enable detailed console logs
```

---

## Testing Checklist

- [ ] NEON library loads (check console: "Library loaded")
- [ ] Callbacks register successfully (check console: "callback registered")
- [ ] Blueprint delay allows React to mount (2+ seconds recommended)
- [ ] JSON format is valid (use online validator if unsure)
- [ ] Method name matches exactly (case-sensitive)
- [ ] DevTools show callback execution when Blueprint calls
- [ ] UI updates with received data

---

## Performance Considerations

### Frequency of Updates:
- ✅ **Good**: Update on state changes (player level up, item acquired)
- ⚠️ **Caution**: Update every frame (60 fps = 60 calls/sec)
- ❌ **Bad**: Update in tight loops (hundreds of calls/sec)

### Data Size:
- ✅ **Good**: Small JSON objects (< 1KB)
- ⚠️ **Caution**: Medium objects (1-10 KB)
- ❌ **Bad**: Large objects (> 100 KB)

**Recommendation**: For frequent updates, throttle or debounce Blueprint calls.

---

## Future Enhancements

### Potential Improvements:

1. **Type-Safe Communication**
   - Define TypeScript interfaces for data structures
   - Validate data at runtime

2. **Event System**
   - Create event bus for complex UI interactions
   - Support event bubbling/capturing

3. **State Synchronization**
   - Auto-sync Blueprint variables with React state
   - Two-way binding helpers

4. **Performance Monitoring**
   - Track callback execution time
   - Monitor data transfer sizes

---

## Credits

**Implementation**: Winston (Claude Code Architect)
**NEON Plugin**: [Techtile Media](https://neon.techtile.media/)
**React Framework**: shadcn/ui + Vite
**Date Achieved**: November 9, 2025

---

## Commit Reference

Git commit: `c81fbf0`
Branch: `master`
Message: "feat: Establish Blueprint → Web communication via NEON"

---

## Status: ✅ PRODUCTION READY

This implementation is **tested, working, and ready for development**.

Remove debug overlays (alert, status indicators) when deploying to production.

---

**Next Steps**: Start building real gameplay features with Blueprint ↔ Web communication! 🚀
