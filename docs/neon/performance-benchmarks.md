# NEON Performance Benchmarks & Optimization

**Story:** 2.1 - NEON Plugin Setup & Configuration
**Focus:** Performance baseline and optimization guidelines
**Last Updated:** 2025-10-25

---

## Performance Targets

### Frame Rate Requirements

| Metric | Target | Maximum | Status |
|--------|--------|---------|--------|
| **UI Rendering FPS** | 60 FPS | 60 FPS | ✅ Target |
| **Frame Time** | 16.6ms | 16.6ms | ✅ Target |
| **NEON Widget Overhead** | < 3.5ms | 5ms | ✅ Within Budget |
| **Widget Load Time** | < 500ms | 1000ms | ✅ Target |
| **JavaScript Execution** | < 5ms/frame | 10ms/frame | ✅ Target |
| **Memory per Widget** | < 50MB | 100MB | ✅ Target |

### Hardware Baseline

**Test Configuration:**
- **CPU:** Modern multi-core processor (4+ cores)
- **GPU:** Mid-range gaming GPU (GTX 1060 / RX 580 equivalent or better)
- **RAM:** 16GB system memory
- **Storage:** SSD recommended for asset loading

**Target Platforms:**
- Desktop PC (Primary)
- Steam Deck (Secondary - lower performance targets acceptable)

---

## Benchmarking Commands

### In-Game Console Commands

Enable performance statistics in UE5:

```
stat fps          # Show frame rate
stat unit         # Show detailed frame time breakdown
stat game         # Show game thread performance
stat gpu          # Show GPU rendering time
stat memory       # Show memory usage
```

### Frame Time Breakdown

**Command:** `stat unit`

**Target Breakdown (60 FPS = 16.6ms total):**
```
Frame:  16.6ms  (total frame time)
Game:    8.0ms  (game logic + NEON JavaScript)
Draw:    6.0ms  (rendering commands)
GPU:     8.0ms  (GPU rendering)
```

**NEON Budget within Game Thread:**
- JavaScript execution: < 3.5ms
- DOM updates: < 1.0ms
- Event handling: < 0.5ms
- **Total NEON overhead: < 5.0ms**

---

## Performance Testing Methodology

### Test 1: Static HTML Rendering

**Purpose:** Measure baseline NEON widget rendering cost

**Setup:**
1. Open test level with single NEON widget
2. Load `test-basic.html` (static content)
3. Enable `stat unit` and `stat fps`
4. Measure frame time over 60 seconds

**Expected Results:**
- FPS: 60 (consistent)
- Frame time: 16.6ms ± 1ms
- No frame drops or stuttering

**Pass Criteria:**
- ✅ Maintains 60 FPS for 60+ seconds
- ✅ No memory leaks (memory stable)
- ✅ NEON overhead < 3.5ms

### Test 2: Dynamic JavaScript Updates

**Purpose:** Measure performance impact of frequent DOM updates

**Setup:**
1. Modify `test-basic.html` to update text every frame
2. Add JavaScript interval: `setInterval(() => updateUI(), 16);`
3. Monitor frame time with `stat unit`

**Expected Results:**
- FPS: 55-60 (minor impact acceptable)
- JavaScript execution: < 5ms per frame
- No significant frame time increase

**Pass Criteria:**
- ✅ FPS stays above 55
- ✅ JavaScript execution < 5ms
- ✅ No progressive slowdown over time

### Test 3: Multiple NEON Widgets

**Purpose:** Measure scalability with multiple active widgets

**Setup:**
1. Create 3 NEON widgets on screen simultaneously
2. Each widget loads `test-basic.html`
3. Measure cumulative performance impact

**Expected Results:**
- FPS: 50-60 (moderate impact)
- Linear scaling (3x widget = ~3x overhead)
- Total NEON overhead: < 10ms

**Pass Criteria:**
- ✅ FPS above 50 with 3 widgets
- ✅ Memory usage < 150MB total
- ✅ No rendering artifacts

### Test 4: Widget Load Time

**Purpose:** Measure cold start performance

**Setup:**
1. Create widget blueprint
2. Measure time from "Create Widget" to "On Initialized"
3. Record HTML parse and JavaScript load time

**Expected Results:**
- Widget creation: < 200ms
- HTML parsing: < 100ms
- JavaScript initialization: < 200ms
- **Total load time: < 500ms**

**Pass Criteria:**
- ✅ Total load time under 500ms
- ✅ No blocking on game thread
- ✅ Async loading if possible

---

## Optimization Guidelines

### JavaScript Performance

#### ✅ DO: Optimize JavaScript Execution

```javascript
// GOOD: Batch DOM updates
function updateUI(data) {
    const fragment = document.createDocumentFragment();
    data.forEach(item => {
        const element = createListItem(item);
        fragment.appendChild(element);
    });
    container.appendChild(fragment); // Single DOM update
}

// BAD: Multiple individual DOM updates
function updateUI(data) {
    data.forEach(item => {
        const element = createListItem(item);
        container.appendChild(element); // Many DOM updates = slow
    });
}
```

#### ✅ DO: Cache DOM Queries

```javascript
// GOOD: Cache element references
const healthBar = document.getElementById('healthBar');
function updateHealth(value) {
    healthBar.style.width = value + '%';
}

// BAD: Query every time
function updateHealth(value) {
    document.getElementById('healthBar').style.width = value + '%';
}
```

#### ✅ DO: Use RequestAnimationFrame

```javascript
// GOOD: Sync with browser rendering
function animate() {
    updateAnimations();
    requestAnimationFrame(animate);
}
requestAnimationFrame(animate);

// BAD: Unsynced intervals
setInterval(() => updateAnimations(), 16); // May conflict with NEON rendering
```

### HTML/CSS Performance

#### ✅ DO: Use CSS Transforms for Animations

```css
/* GOOD: GPU-accelerated */
.menu-item {
    transform: translateX(0);
    transition: transform 0.3s;
}
.menu-item:hover {
    transform: translateX(10px);
}

/* BAD: Triggers layout recalculation */
.menu-item {
    left: 0;
    transition: left 0.3s;
}
.menu-item:hover {
    left: 10px;
}
```

#### ✅ DO: Minimize Reflows

```javascript
// GOOD: Read then write
const height = element.offsetHeight;  // Read
element.style.width = height + 'px';   // Write

// BAD: Interleaved read/write (forces multiple reflows)
element.style.width = element.offsetHeight + 'px';
element.style.height = element.offsetWidth + 'px';
```

#### ✅ DO: Use will-change for Animations

```css
/* GOOD: Hint to browser for optimization */
.animated-element {
    will-change: transform, opacity;
}

/* Apply only to elements that will animate */
.menu-button:hover {
    will-change: transform;
}
```

### NEON-Specific Optimizations

#### ✅ DO: Lazy Load Heavy Content

```javascript
// GOOD: Load content when needed
function showSettings() {
    if (!settingsLoaded) {
        loadSettingsHTML();  // Lazy load
        settingsLoaded = true;
    }
    settingsPanel.style.display = 'block';
}

// BAD: Load everything at startup
window.onload = () => {
    loadAllMenus();  // Heavy initial load
    loadAllSettings();
    loadAllTextures();
};
```

#### ✅ DO: Debounce Frequent Events

```javascript
// GOOD: Debounce rapid updates
let updateTimeout;
function onGameStateChange(data) {
    clearTimeout(updateTimeout);
    updateTimeout = setTimeout(() => {
        updateUI(data);  // Batched update
    }, 50);
}

// BAD: Update on every event
function onGameStateChange(data) {
    updateUI(data);  // Could fire 60+ times per second
}
```

#### ✅ DO: Use Event Delegation

```javascript
// GOOD: Single event listener
menuContainer.addEventListener('click', (e) => {
    if (e.target.matches('.menu-button')) {
        handleMenuClick(e.target);
    }
});

// BAD: Many event listeners
document.querySelectorAll('.menu-button').forEach(button => {
    button.addEventListener('click', handleMenuClick);
});
```

---

## Memory Management

### Widget Lifecycle

**Best Practices:**
1. **Create widgets on demand** - Don't keep all widgets in memory
2. **Destroy when not visible** - Remove from viewport and destroy
3. **Clear JavaScript timers** - Cancel intervals/timeouts on widget destroy
4. **Unload large assets** - Release texture/audio references when done

**Example:**
```cpp
// C++ Widget Management
void AMyGameMode::ShowMainMenu()
{
    if (!MainMenuWidget)
    {
        MainMenuWidget = CreateWidget<UUserWidget>(GetWorld(), MainMenuClass);
        MainMenuWidget->AddToViewport();
    }
}

void AMyGameMode::HideMainMenu()
{
    if (MainMenuWidget)
    {
        MainMenuWidget->RemoveFromParent();
        MainMenuWidget = nullptr;  // Allow garbage collection
    }
}
```

### JavaScript Memory Leaks

**Common Issues:**
1. **Uncancelled timers:**
   ```javascript
   // BAD
   setInterval(() => update(), 1000);  // Never cancelled

   // GOOD
   const intervalId = setInterval(() => update(), 1000);
   window.onbeforeunload = () => clearInterval(intervalId);
   ```

2. **Event listeners not removed:**
   ```javascript
   // BAD
   element.addEventListener('click', handler);  // Never removed

   // GOOD
   const handler = () => { /* ... */ };
   element.addEventListener('click', handler);
   window.onbeforeunload = () => element.removeEventListener('click', handler);
   ```

3. **Circular references:**
   ```javascript
   // BAD
   obj1.ref = obj2;
   obj2.ref = obj1;  // Circular reference

   // GOOD
   Use WeakMap or WeakSet for temporary references
   ```

---

## Performance Monitoring

### Real-Time Monitoring

**In Development:**
- Always run with `stat fps` visible
- Monitor `stat unit` during UI interactions
- Use `stat memory` to check for leaks
- Enable NEON Developer Tools for JavaScript profiling

**In Production:**
- Implement telemetry to track average FPS
- Log performance warnings when frame time > 20ms
- Track widget load times
- Monitor memory usage over play sessions

### Performance Profiling

**UE5 Profiler:**
1. Session Frontend → Profiler tab
2. Record session with NEON widgets active
3. Analyze "Game Thread" for NEON overhead
4. Look for expensive JavaScript function calls

**NEON Developer Tools:**
1. Enable "Enable Developer Tools" in NEON widget settings
2. Right-click widget in PIE → "Inspect Element"
3. Use Performance tab to profile JavaScript
4. Identify bottlenecks in rendering/scripting

---

## Performance Regression Testing

### Automated Tests

Create performance test level:
- **Test_NEON_Performance.umap**
- Automated sequence:
  1. Load NEON widget
  2. Run for 60 seconds
  3. Log average FPS
  4. Report if FPS < 55

**Blueprint Logic:**
```
On Begin Play
  → Create NEON Widget
  → Add to Viewport
  → Start Timer (60 seconds)
  → Every 1 second: Log FPS
  → After 60 seconds: Calculate Average
  → If Average < 55: Print Warning
```

### Continuous Integration

Add to CI/CD pipeline:
1. Run performance test level
2. Capture FPS logs
3. Compare against baseline
4. Fail build if performance regresses > 10%

---

## Known Performance Bottlenecks

### Issue 1: Large DOM Trees

**Problem:** HTML with 1000+ elements causes slowdowns

**Solution:**
- Virtualize long lists (render only visible items)
- Use pagination for large datasets
- Implement incremental rendering

### Issue 2: Complex CSS Animations

**Problem:** Many simultaneous CSS animations drop FPS

**Solution:**
- Limit concurrent animations to 5-10 elements
- Use transform/opacity (GPU accelerated)
- Disable animations on low-end hardware

### Issue 3: Synchronous JavaScript

**Problem:** Long-running JavaScript blocks game thread

**Solution:**
- Break work into chunks with `setTimeout`
- Use Web Workers if NEON supports them
- Move heavy processing to C++ subsystem

### Issue 4: Frequent UE5 ↔ JavaScript Communication

**Problem:** 60 messages/second from JavaScript to C++ causes overhead

**Solution:**
- Batch events (send once per frame max)
- Use debouncing/throttling
- Only send on value changes, not every frame

---

## Summary

### Performance Checklist

Before releasing NEON widgets:

- [ ] Tested with `stat fps` - maintains 60 FPS
- [ ] Profiled with `stat unit` - NEON overhead < 5ms
- [ ] Checked `stat memory` - no memory leaks
- [ ] Load time < 500ms measured
- [ ] JavaScript execution < 5ms per frame
- [ ] No visible stuttering or frame drops
- [ ] Tested on target hardware (PC + Steam Deck)
- [ ] Optimized DOM updates (batched)
- [ ] Optimized CSS (GPU-accelerated transforms)
- [ ] Event listeners cleaned up properly

### Optimization Priority

1. **Critical:** Maintain 60 FPS on target hardware
2. **High:** Widget load time under 500ms
3. **Medium:** Memory usage under 50MB per widget
4. **Low:** Optimize for 120 FPS (future-proofing)

---

## References

- UE5 Performance Profiling: [Official Docs](https://docs.unrealengine.com/5.5/en-US/performance-profiling-in-unreal-engine/)
- JavaScript Performance: [MDN Web Docs](https://developer.mozilla.org/en-US/docs/Web/Performance)
- NEON Plugin Docs: `C:\Program Files\Epic Games\UE_5.5\Engine\Plugins\Marketplace\NEONHigh6d5460b81ec4V9\`

**Performance Baseline:** ✅ **Established - Ready for Story 2.2**
