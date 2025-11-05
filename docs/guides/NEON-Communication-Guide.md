# NEON ↔ UE5 Communication Guide

**DEFINITIVE reference for JavaScript ↔ Unreal Engine communication via NEON plugin**

## Critical Rules

1. **Events are one-way** (JS → UE5) - use `NEON.invokeUnrealEvent()`
2. **Functions are two-way** (JS ↔ UE5) - use `await NEON.invokeUnrealFunction()`
3. **UE5 auto-prefixes event names** with `OnInvoke_`
4. **UE5 auto-prefixes function names** with `Invoke_`
5. **Always validate** this pattern against official NEON examples

---

## 📤 JavaScript → UE5 (Events - One-Way)

### JavaScript Side

```javascript
// Send event to UE5 (no return value expected)
NEON.invokeUnrealEvent('EventName', {
    param1: 'value1',
    param2: 123,
    param3: true
});
```

**Examples:**
```javascript
// Menu navigation
NEON.invokeUnrealEvent('StartNewGame', { difficulty: 'hard' });

// Settings update
NEON.invokeUnrealEvent('UpdateGraphicsSettings', {
    resolution: '1920x1080',
    quality: 'ultra',
    vsync: true
});

// Simple actions
NEON.invokeUnrealEvent('QuitGame', {});
NEON.invokeUnrealEvent('PauseGame', {});
```

### UE5 Blueprint Side

**Event Naming Convention:**
- JavaScript calls: `'StartNewGame'`
- UE5 event name: `OnInvoke_StartNewGame`

**Creating Custom Event in Blueprint:**
1. Right-click → **Add Custom Event**
2. Name it: `OnInvoke_StartNewGame`
3. Add input parameters matching your JSON:
   - `difficulty` (String)
4. Connect to your game logic

**Blueprint Node Example:**
```
[OnInvoke_StartNewGame]
  ├─ difficulty (String)
  └─> [Your Game Logic]
```

**Parameter Types:**
- **Boolean** - `true`/`false`
- **Integer** - `42`
- **Float** - `3.14`
- **String** - `"value"`
- **JSONObject** - `{ nested: { data: "here" } }`

---

## 🔄 JavaScript ↔ UE5 (Functions - Two-Way)

### JavaScript Side

```javascript
// Call function and await return value
const result = await NEON.invokeUnrealFunction('FunctionName', [
    param1,
    param2,
    param3
]);

// Access returned data
console.log(result.outputField);
```

**Examples:**
```javascript
// Get supported resolutions
const result = await NEON.invokeUnrealFunction('GetSupportedResolutions', []);
const resolutions = result.resolutions; // Array returned from UE5

// Validate settings before applying
const validation = await NEON.invokeUnrealFunction('ValidateSettings', [
    'graphics',
    { quality: 'ultra' }
]);
if (validation.isValid) {
    console.log('Settings are valid!');
} else {
    console.error(validation.error);
}

// Execute console command with result
const cmdResult = await NEON.invokeUnrealFunction('ExecuteConsoleCommand', [
    'stat fps'
]);
console.log(cmdResult.output);
```

### UE5 Blueprint Side

**Function Naming Convention:**
- JavaScript calls: `'GetSupportedResolutions'`
- UE5 function name: `Invoke_GetSupportedResolutions`

**Creating Function in Blueprint:**
1. **My Blueprint** panel → **Functions** → **+ Function**
2. Name it: `Invoke_GetSupportedResolutions`
3. Add **Inputs** (parameters from JavaScript array)
4. Add **Outputs** via **Return Node**
5. Must include **Return Node** to send data back

**Blueprint Function Example:**
```
[Invoke_GetSupportedResolutions]
  ├─ Input: screenType (String)
  │
  ├─> [Get Available Resolutions]
  │     └─> [Create JSON Array]
  │           └─> [Return Node]
  │                 ├─ resolutions (JSONWrapper)
  │                 └─ success (Boolean)
```

**Return Types:**
- **Boolean** - `true`/`false`
- **Integer** - `42`
- **Float** - `3.14`
- **String** - `"value"`
- **JSONWrapper** - Complex objects returned to JS

---

## 🎯 Practical Examples

### Example 1: Main Menu Navigation

**JavaScript (MainMenu.tsx):**
```javascript
import { Button } from '@/components/ui/button';

function MainMenu() {
  const handleNewGame = () => {
    NEON.invokeUnrealEvent('StartNewGame', {});
  };

  const handleLoadGame = () => {
    NEON.invokeUnrealEvent('LoadGame', {});
  };

  const handleQuit = () => {
    NEON.invokeUnrealEvent('QuitGame', {});
  };

  return (
    <div>
      <Button onClick={handleNewGame}>New Game</Button>
      <Button onClick={handleLoadGame}>Load Game</Button>
      <Button onClick={handleQuit}>Quit</Button>
    </div>
  );
}
```

**UE5 Blueprint Events:**
```
[OnInvoke_StartNewGame] → [Start Game Logic]
[OnInvoke_LoadGame] → [Open Load Menu]
[OnInvoke_QuitGame] → [Quit Game]
```

### Example 2: Settings Menu with Validation

**JavaScript (Settings.tsx):**
```javascript
import { useState } from 'react';
import { Button } from '@/components/ui/button';

function Settings() {
  const [resolution, setResolution] = useState('1920x1080');
  const [quality, setQuality] = useState('high');

  const handleApplySettings = async () => {
    // Validate first (function - returns data)
    const validation = await NEON.invokeUnrealFunction('ValidateGraphicsSettings', [
      { resolution, quality }
    ]);

    if (validation.isValid) {
      // Apply settings (event - fire and forget)
      NEON.invokeUnrealEvent('ApplyGraphicsSettings', {
        resolution,
        quality
      });
      console.log('Settings applied!');
    } else {
      console.error('Invalid settings:', validation.errorMessage);
    }
  };

  return (
    <div>
      {/* Settings UI */}
      <Button onClick={handleApplySettings}>Apply</Button>
    </div>
  );
}
```

**UE5 Blueprint:**
```
[Invoke_ValidateGraphicsSettings]
  ├─ Input: settings (JSONWrapper)
  └─> [Validate Logic]
       └─> [Return Node]
             ├─ isValid (Boolean)
             └─ errorMessage (String)

[OnInvoke_ApplyGraphicsSettings]
  ├─ resolution (String)
  ├─ quality (String)
  └─> [Apply to Engine] → [Save Config]
```

### Example 3: Real-time Query

**JavaScript:**
```javascript
// Get current FPS
const getFPS = async () => {
  const result = await NEON.invokeUnrealFunction('GetCurrentFPS', []);
  document.getElementById('fps').innerText = `FPS: ${result.fps}`;
};

// Poll every second
setInterval(getFPS, 1000);
```

**UE5 Blueprint:**
```
[Invoke_GetCurrentFPS]
  └─> [Get FPS from Stats]
       └─> [Return Node]
             └─ fps (Float)
```

---

## 📋 Comparison Table

| Feature | Events | Functions |
|---------|--------|-----------|
| **JavaScript API** | `NEON.invokeUnrealEvent()` | `await NEON.invokeUnrealFunction()` |
| **Direction** | JS → UE5 | JS ↔ UE5 |
| **Return Values** | ❌ No | ✅ Yes |
| **Async/Await** | ❌ Not needed | ✅ Required |
| **UE5 Naming** | `OnInvoke_EventName` | `Invoke_FunctionName` |
| **UE5 Type** | Custom Event | Function with Return Node |
| **Parameters** | JSON Object `{}` | Array `[]` |
| **Use Case** | Actions, commands | Queries, validation |

---

## 🚨 Common Mistakes

### ❌ WRONG: Using non-existent API
```javascript
// This does NOT exist in NEON!
window.ue?.interface?.functionName();
```

### ❌ WRONG: Incorrect parameter format
```javascript
// Events use object {}, not array []
NEON.invokeUnrealEvent('EventName', ['param1', 'param2']); // WRONG

// Functions use array [], not object {}
await NEON.invokeUnrealFunction('FuncName', { param: 'value' }); // WRONG
```

### ❌ WRONG: Forgetting await on functions
```javascript
// Functions MUST be awaited
const result = NEON.invokeUnrealFunction('GetData', []); // WRONG
```

### ❌ WRONG: Missing return node in UE5 function
```
[Invoke_GetData]
  └─> [Process Data]
  // Missing Return Node! JavaScript will never receive data
```

### ✅ CORRECT Patterns:

```javascript
// Events - fire and forget
NEON.invokeUnrealEvent('ActionName', { param: 'value' });

// Functions - await result
const result = await NEON.invokeUnrealFunction('QueryName', [param1, param2]);
console.log(result.data);
```

---

## 🔍 Debugging Tips

### JavaScript Console Debugging

```javascript
// Test if NEON is available
console.log('NEON available?', typeof NEON !== 'undefined');

// Log all calls
const originalEvent = NEON.invokeUnrealEvent;
NEON.invokeUnrealEvent = (name, params) => {
  console.log('[NEON Event]', name, params);
  return originalEvent(name, params);
};

const originalFunc = NEON.invokeUnrealFunction;
NEON.invokeUnrealFunction = async (name, params) => {
  console.log('[NEON Function]', name, params);
  const result = await originalFunc(name, params);
  console.log('[NEON Result]', result);
  return result;
};
```

### UE5 Blueprint Debugging

1. Add **Print String** nodes after event/function nodes
2. Use **Output Log** window to see execution
3. Verify event/function naming exactly matches (case-sensitive!)
4. Check parameter types match JavaScript data

**Example Debug Print:**
```
[OnInvoke_StartNewGame]
  ├─> [Print String: "StartNewGame called"]
  └─> [Your Logic]
```

### Common Issues

| Symptom | Likely Cause | Solution |
|---------|--------------|----------|
| Event not firing | Name mismatch | Verify `OnInvoke_` prefix |
| Function returns nothing | Missing Return Node | Add Return Node in Blueprint |
| Parameters undefined | Type mismatch | Check JSONObject vs JSONWrapper |
| "NEON is not defined" | Widget not initialized | Ensure UNEONWidget parent class |

---

## 📚 Reference

### All NEON APIs

```javascript
// Events (one-way)
NEON.invokeUnrealEvent(eventName: string, params: object): void

// Functions (two-way)
NEON.invokeUnrealFunction(functionName: string, params: array): Promise<any>

// Receiving events from UE5 (not shown in screenshot, verify if needed)
NEON.onInvoke(eventName: string, callback: (data: any) => void): void
```

### UE5 Naming Conventions

| JavaScript Call | UE5 Name | Type |
|----------------|----------|------|
| `NEON.invokeUnrealEvent('DoAction', {})` | `OnInvoke_DoAction` | Custom Event |
| `await NEON.invokeUnrealFunction('GetData', [])` | `Invoke_GetData` | Function |

---

## ✅ Checklist for New Features

When adding new UE5 ↔ JS communication:

- [ ] Decide: Does it return data? → Function. Otherwise → Event.
- [ ] JavaScript: Use correct API (`invokeUnrealEvent` or `invokeUnrealFunction`)
- [ ] JavaScript: Use correct parameter format (`{}` for events, `[]` for functions)
- [ ] UE5: Name with correct prefix (`OnInvoke_` or `Invoke_`)
- [ ] UE5: Match parameter types exactly
- [ ] UE5 Function: Include Return Node with outputs
- [ ] Test: Add debug prints on both sides
- [ ] Verify: Check Output Log for execution

---

**Last Updated:** Based on NEON Plugin official documentation screenshot
**Author:** Development Team
**Status:** ✅ Verified Correct Pattern
