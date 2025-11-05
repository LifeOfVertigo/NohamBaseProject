# NEON Widget Quick Start Checklist

**Use this checklist while creating widgets in the UE5 Editor.**

---

## 📋 Creating WBP_MainMenu Widget

### ✅ Part 1: Create Blueprint (In UE5 Editor)

1. Navigate to `Content/UI/Widgets/`
2. Right-click → **User Interface** → **Widget Blueprint**
3. Name: `WBP_MainMenu`
4. Open the widget
5. **File** → **Reparent Blueprint** → Select `UNEONWidget` → Click Reparent

---

### ✅ Part 2: Add Image Component

1. Drag **Image** from Palette to canvas
2. **CRITICAL**: Rename to exactly `_BrowserImage` (Details panel → Name field)
3. Set **Anchors**: Min (0, 0), Max (1, 1)
4. Set **Offsets**: Left 0, Top 0, Right 0, Bottom 0
5. Verify "Is Variable" is checked

---

### ✅ Part 3: Configure NEON Settings

**Select the root widget (WBP_MainMenu in hierarchy)**

In Details panel → NEON category:

- `_DevelopmentURL`: `http://localhost:5173/MainMenu/index.html`
- `_LiveURL`: `file:///Content/UI/NEON/dist/MainMenu/index.html`
- `_OpenDevTools`: `true` (for development)
- `_MaxFPS`: `60`
- `_ExternalBeginFrame`: `false`

In Details panel → Behavior category:

- ✅ Is Focusable
- ✅ Supports Keyboard Focus

---

### ✅ Part 4: Save & Compile

1. Click **Compile**
2. Click **Save**
3. Close widget editor

---

## 🚀 Testing the Widget

### Before Testing: Start Vite Dev Server

```bash
cd "C:\Users\tyler\OneDrive\Documentos\Unreal Projects\NohamBaseProject\Noham_Base_Proj_Cpp\Content\UI\NEON"
npm run dev
```

**Wait for:** `ready in XXX ms` message showing `http://localhost:5173`

---

### Create Test Level Blueprint

1. Open your main level (or create a test level)
2. **Blueprints** → **Open Level Blueprint**
3. Add nodes:

```
Event BeginPlay
  └─> Create Widget (Class: WBP_MainMenu)
      └─> Add to Viewport (Z-Order: 0)
          └─> Return Value → Promote to Variable (name: MainMenuWidget)
              └─> Get Player Controller
                  └─> Set Show Mouse Cursor (Show: true)
                      └─> Set Input Mode UI Only
                          └─> Widget to Focus: MainMenuWidget
```

---

### Test in Play-In-Editor

1. Click **Play** (Alt+P)
2. **Expected Results:**
   - Main menu appears with buttons
   - Chrome DevTools opens (if `_OpenDevTools = true`)
   - Mouse cursor visible
   - Can click buttons
   - Console shows React app loaded

3. **Check Output Log for:**
   ```
   LogNEON: Browser created
   LogNEON: Loaded URL: http://localhost:5173/MainMenu/index.html
   ```

---

## 🔧 Quick Troubleshooting

| Problem | Fix |
|---------|-----|
| Blank black screen | Check Vite is running, URL is correct |
| Image component not found error | Rename to exactly `_BrowserImage` |
| Can't click buttons | Check "Is Focusable" + Input Mode UI Only |
| JavaScript errors in console | Check file paths in HTML imports |
| Widget not showing | Verify `Add to Viewport` is called |

---

## 📂 File Paths Reference

**Widget Location:**
```
Content/UI/Widgets/WBP_MainMenu.uasset
```

**HTML Entry Point:**
```
Content/UI/NEON/MainMenu/index.html
```

**React Component:**
```
Content/UI/NEON/MainMenu/index.tsx
```

**Development Server:**
```
http://localhost:5173/MainMenu/index.html
```

**Production Build:**
```
file:///Content/UI/NEON/dist/MainMenu/index.html
```

---

## 🎯 Next Widgets to Create

**Using the same process:**

1. `WBP_Settings` → Points to `Settings/index.html`
2. `WBP_HUD` → Points to `HUD/index.html`
3. `WBP_PauseMenu` → Points to `PauseMenu/index.html`

**Each needs:**
- Parent class: `UNEONWidget`
- Image component: `_BrowserImage`
- Appropriate URLs configured
- Input settings enabled

---

## 💡 Pro Tips

- **Always start Vite before PIE** for hot reload
- **Use DevTools** to debug JavaScript live
- **Check Output Log** for NEON initialization messages
- **Test input** early - ensure clicks work before complex logic
- **Keep widgets simple** initially - verify basic rendering first

---

## ✅ Success Criteria

You know it's working when:

1. ✅ Widget displays in PIE with Vite running
2. ✅ DevTools shows React app with no console errors
3. ✅ Can click buttons and see hover effects
4. ✅ Button clicks trigger UE5 logs (even if placeholder)
5. ✅ Mouse cursor visible and responsive

---

**Full Documentation:** `docs/guides/NEON-Widget-Setup-Guide.md`
