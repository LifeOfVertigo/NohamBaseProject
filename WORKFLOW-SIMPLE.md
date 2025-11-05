# Simple NEON + React Workflow

## Do Players Need a Web Server?

**NO! Absolutely not.** Players get the bundled files, which are pure HTML/JS/CSS that work offline.

---

## Two Development Modes

### 🎨 Mode 1: UI Development (Fast Iteration)
**When:** Making changes to React components, styling, layout

```bash
cd shadcn-base
npm run dev
```

**What happens:**
- Dev server starts at `http://localhost:5173`
- Open in your **browser** (Chrome, Firefox, etc.)
- Make changes to `.tsx` files
- See updates **instantly** (no rebuild needed)

**NOT for Unreal Engine testing!**

---

### 🎮 Mode 2: Unreal Engine Testing

**When:** Testing how UI works inside your game

```bash
cd shadcn-base
npm run build:ue
```

**What happens:**
- Builds production bundle
- Copies to `Noham_Base_Proj_Cpp/Content/UI/NEON/MainMenu/dist/`
- Ready to test in Unreal Engine

**Then in Unreal Engine:**
- Play in Editor (PIE)
- No web server needed
- This is exactly what players will experience

---

## The Full Picture

```
┌─────────────────────────────────────────────────────────┐
│                    DEVELOPMENT                          │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  Your React Code (.tsx files)                          │
│         ↓                                               │
│    npm run dev ──→ Browser Testing (fast iteration)    │
│         ↓                                               │
│  npm run build:ue ──→ Unreal Engine Testing            │
│         ↓                                               │
│    dist/ folder ──→ Packaged Game                      │
│                         ↓                               │
│                    PLAYERS                              │
│                 (no server needed!)                     │
└─────────────────────────────────────────────────────────┘
```

---

## Your Questions Answered

### Q: Do I always use the dist build in Unreal?
**A: YES!** The NEON widget in Unreal Engine should point to:
```
Live URL: /Content/UI/NEON/MainMenu/dist/index.html
```

### Q: Do players need to start a web server?
**A: NO!** The dist files are bundled, self-contained HTML/JS/CSS. They work completely offline.

### Q: When do I use `npm run dev`?
**A: ONLY when working on UI in your browser** for fast iteration. Not for Unreal Engine.

### Q: When do I use `npm run build:ue`?
**A: Before testing in Unreal Engine** and before packaging your game.

---

## Quick Reference

| Task | Command | Where to Test |
|------|---------|---------------|
| UI development | `npm run dev` | Browser at localhost:5173 |
| UE testing | `npm run build:ue` | Unreal Engine (PIE) |
| Shipping | Already done! | Packaged game |

---

## What Gets Bundled?

The `dist/` folder contains:

```
dist/
├── index.html              ← Entry point
└── assets/
    ├── index-[hash].js    ← ALL React code + dependencies (195 KB)
    └── index-[hash].css   ← ALL styles (Tailwind + components)
```

This is **everything**:
- ✅ React library
- ✅ React DOM
- ✅ All shadcn components
- ✅ Radix UI primitives
- ✅ Tailwind CSS
- ✅ Your custom code

**Total size:** ~200 KB (tiny!)

**No external files needed. No internet. No server. Just works.**
