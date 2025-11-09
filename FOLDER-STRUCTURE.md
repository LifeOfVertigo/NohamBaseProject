# Project Folder Structure Guide

## Overview

This document explains what belongs **inside** the Unreal Engine project folder vs. **outside** in the development workspace.

---

## ✅ INSIDE Unreal Engine Project (`Noham_Base_Proj_Cpp/`)

### This is the folder that should be self-contained and portable:

```
Noham_Base_Proj_Cpp/
├── Noham_Base_Proj_Cpp.uproject          # UE5 project file
├── Config/                                # UE5 configuration files
├── Content/                               # All UE5 content assets
│   ├── CORE/                             # Core game Blueprints
│   ├── Widgets/                          # UI Widget Blueprints
│   │   └── CoreMenuWidgets/
│   │       └── WBP_MainMenu.uasset      # Main menu widget
│   └── UI/                               # Web UI assets
│       └── NEON/                         # NEON web UIs
│           ├── MainMenu/
│           │   ├── index.tsx            # React source (for reference)
│           │   └── dist/                # ✅ COMPILED OUTPUT (UE5 loads this)
│           │       ├── index.html
│           │       ├── neon.js          # NEON library
│           │       └── assets/
│           ├── Settings/
│           │   └── dist/
│           └── Components/              # Shared React components
├── NEON/                                 # Alternative NEON location (legacy)
│   └── MainMenu/dist/                   # ✅ Also gets build output (for compatibility)
├── Plugins/                              # UE5 plugins
│   └── NEON/                            # NEON UE5 plugin
├── Source/                               # C++ source code
├── Binaries/                             # Compiled binaries (auto-generated)
├── Intermediate/                         # Build intermediates (auto-generated)
└── Saved/                                # Auto-saves, logs (auto-generated)
```

**What UE5 Needs:**
- ✅ `.uproject` file
- ✅ `Content/` folder with all assets
- ✅ **COMPILED web UIs** in `Content/UI/NEON/*/dist/`
- ✅ `Plugins/` folder
- ✅ `Source/` folder (C++ code)
- ✅ `Config/` folder

**What UE5 Does NOT Need:**
- ❌ React/TypeScript source files (`.tsx`, `.ts`) - these are for developers only
- ❌ `node_modules/`
- ❌ Build tooling (`package.json`, `vite.config.ts`, etc.)

---

## ❌ OUTSIDE Unreal Engine Project (Development Workspace)

### These folders are for **building** the web UIs, NOT for UE5:

```
NohamBaseProject/                         # Master workspace (this folder)
├── Noham_Base_Proj_Cpp/                 # ← The UE5 project (above)
├── shadcn-base/                         # ⚙️ BUILD TOOLING for React UIs
│   ├── package.json                     # npm dependencies
│   ├── vite.config.ts                   # Build configuration
│   ├── node_modules/                    # Dependencies (NEVER commit to UE)
│   ├── src/                             # React source files
│   │   └── main.tsx                     # Entry point
│   ├── dist/                            # ← Build output (temporary)
│   └── index.html                       # Source HTML template
├── neon-ue-web-main/                    # 🔧 NEON library source
│   ├── index.ts                         # TypeScript source
│   ├── neon-global.js                   # Compiled version (used in builds)
│   └── package.json
├── docs/                                # Documentation
├── .git/                                # Git repository
├── .bmad-core/                          # BMad AI tools
└── README.md

```

**Purpose of External Folders:**
- `shadcn-base/` - **React build system** (Vite + TypeScript)
- `neon-ue-web-main/` - **NEON library source** (official from GitHub)
- These are **development tools** only

---

## 🔄 Build Process Flow

```
1. DEVELOP:
   shadcn-base/src/main.tsx               (Edit React code)
   ↓

2. BUILD:
   cd shadcn-base && npm run build:ue     (Compile TypeScript → JavaScript)
   ↓

3. DEPLOY:
   shadcn-base/dist/  →  COPY TO  →  Noham_Base_Proj_Cpp/Content/UI/NEON/MainMenu/dist/
                    AND  →  Noham_Base_Proj_Cpp/NEON/MainMenu/dist/
   ↓

4. USE IN UE5:
   WBP_MainMenu loads: /MainMenu/dist/index.html
```

---

## 📋 What to Include in Version Control (Git)

### ✅ COMMIT to Git:
```
Noham_Base_Proj_Cpp/
├── *.uproject                            # Project file
├── Config/**                             # All config files
├── Content/**/*.uasset                   # All UE5 assets
├── Content/UI/NEON/*/dist/**             # ✅ COMPILED web UIs
├── Plugins/**                            # Plugin files
└── Source/**/*.cpp, *.h                  # C++ source code
```

### ❌ DO NOT COMMIT to Git:
```
Noham_Base_Proj_Cpp/
├── Binaries/                             # Auto-generated
├── Intermediate/                         # Auto-generated
├── Saved/                                # Auto-generated
├── DerivedDataCache/                     # Auto-generated
└── .vs/                                  # Visual Studio temp files

shadcn-base/
├── node_modules/                         # npm dependencies (huge!)
├── dist/                                 # Build output (temporary)
└── .vite/                                # Vite cache

neon-ue-web-main/
└── node_modules/                         # npm dependencies
```

**Your `.gitignore` should exclude these folders.**

---

## 🎯 Current Status

### ✅ What's Set Up Correctly:

1. **NEON library deployed** to both locations:
   - `Noham_Base_Proj_Cpp/Content/UI/NEON/MainMenu/dist/neon.js` ✅
   - `Noham_Base_Proj_Cpp/NEON/MainMenu/dist/neon.js` ✅

2. **Build script** (`shadcn-base/package.json`):
   - `npm run build:ue` builds and copies to **both** UE5 locations
   - Auto-copies official NEON library

3. **React app** with NEON integration:
   - Loads NEON as global script
   - Registers callbacks for Blueprint → Web communication
   - Debug overlays for testing

### ⚠️ What Could Be Improved:

1. **Consolidate NEON locations** (currently two):
   - Determine which one WBP_MainMenu actually uses
   - Remove the unused one (or add note explaining why both exist)

2. **Move source files** (optional):
   - React source (`.tsx`) files are currently in `Content/UI/NEON/MainMenu/`
   - Could move to `shadcn-base/` and only deploy `dist/` to UE project
   - This would make UE project cleaner (only compiled files)

---

## 🚀 For New Team Members

**To set up development environment:**

```bash
# 1. Clone repository
git clone <repo-url>
cd NohamBaseProject

# 2. Install build tools
cd shadcn-base
npm install

# 3. Build web UIs
npm run build:ue

# 4. Open UE5 project
# Open: Noham_Base_Proj_Cpp/Noham_Base_Proj_Cpp.uproject in UE5
```

**To make changes to web UI:**

```bash
# 1. Edit React code
cd shadcn-base
# Edit files...

# 2. Rebuild and deploy
npm run build:ue

# 3. Refresh in UE5
# Widget will auto-reload if browser is open
```

---

## 📦 For Distributing/Sharing Project

**To share the UE5 project:**

1. **ZIP the entire `Noham_Base_Proj_Cpp/` folder**
2. Ensure it includes:
   - ✅ `Content/UI/NEON/*/dist/` folders (compiled UIs)
   - ✅ All `.uasset` files
   - ✅ `Plugins/` folder
   - ✅ `Source/` folder
3. **Exclude** (can be rebuilt):
   - Binaries/
   - Intermediate/
   - Saved/
   - DerivedDataCache/

**Recipient can:**
- Extract folder
- Right-click `.uproject` → "Generate Visual Studio project files"
- Open in UE5
- Everything works (including web UIs)

---

## 🏗️ Architecture Summary

```
┌─────────────────────────────────────────────────────────┐
│         DEVELOPMENT WORKSPACE (Git Repo)                │
│  ┌───────────────────────────────────────────────────┐  │
│  │    Noham_Base_Proj_Cpp/ (UE5 Project)            │  │
│  │    - Self-contained                               │  │
│  │    - Can be shared independently                  │  │
│  │    - Contains compiled web UIs                    │  │
│  └───────────────────────────────────────────────────┘  │
│  ┌───────────────────────────────────────────────────┐  │
│  │    shadcn-base/ (Build Tools)                     │  │
│  │    - Compiles React → JavaScript                  │  │
│  │    - Deploys to UE5 project                       │  │
│  └───────────────────────────────────────────────────┘  │
│  ┌───────────────────────────────────────────────────┐  │
│  │    neon-ue-web-main/ (NEON Library)               │  │
│  │    - Official source from GitHub                  │  │
│  └───────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────┘
```

---

## ✅ Everything is Now Ready!

**Test in UE5:**
1. Play in Editor
2. Check top-left for "✅ Ready" indicator
3. Wait 10 seconds for Blueprint to call
4. Should see JavaScript alert + top-right message

**The UE5 project (`Noham_Base_Proj_Cpp/`) now contains everything it needs to run independently!**
