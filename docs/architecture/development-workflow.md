# Development Workflow

Complete development setup and workflow for the hybrid UE5 + NEON + shadcn architecture, optimized for rapid iteration and AI-assisted development.

### Local Development Setup

#### Prerequisites

```bash
# Required Software
- Unreal Engine 5.5.4
- Visual Studio 2022 (C++ development)
- Node.js 18+ (for shadcn/Vite toolchain)
- Git with LFS support
- Steam SDK (for platform integration testing)

# Node.js Dependencies
npm install -g @vitejs/create-vite
npm install -g tailwindcss
npm install -g typescript (optional)
```

#### Initial Setup

```bash
# 1. Clone Repository
git clone [repository-url] NohamBaseProject
cd NohamBaseProject

# 2. Install NEON Plugin
# Download NEON plugin from GitHub/FAB store
# Extract to Plugins/NEON/ directory
# Enable in UE5 Editor (Edit > Plugins > NEON)

# 3. Setup Web Development Environment
cd Content/UI/NEON/
npm install
npm install neon-web
npm install @shadcn/ui tailwindcss framer-motion

# 4. Install shadcn Components
npx shadcn-ui@latest init
npx shadcn-ui@latest add button slider select input card tabs dialog toast progress badge

# 5. Generate UE5 Project Files
cd ../../../
"C:\Program Files\Epic Games\UE_5.5\Engine\Binaries\DotNET\UnrealBuildTool.exe" -projectfiles -project="NohamBaseProject.uproject" -game -rocket -progress

# 6. Build C++ Project
# Open NohamBaseProject.sln in Visual Studio
# Build Solution (Development Editor configuration)
```

#### Development Commands

```bash
# Start All Services (Parallel Development)
# Terminal 1: UE5 Editor
start NohamBaseProject.uproject

# Terminal 2: NEON Web Assets Hot Reload
cd Content/UI/NEON/
npm run dev        # Vite dev server with hot reload

# Terminal 3: Watch C++ Changes
# Visual Studio hot reload or
cd Source/
# Use UE5 hot reload (Ctrl+Alt+F11)

# Frontend Development Only
cd Content/UI/NEON/
npm run dev        # Start Vite dev server
npm run build      # Build for production
npm run preview    # Preview production build

# Full Project Build
npm run build:ui   # Build web assets
# UE5 Editor: Build > Build All
```

### Environment Configuration

#### Web Development Configuration

```javascript
// vite.config.js - NEON asset bundling
import { defineConfig } from 'vite';
import { resolve } from 'path';

export default defineConfig({
  root: 'Content/UI/NEON',
  base: './',
  build: {
    outDir: 'dist',
    rollupOptions: {
      input: {
        main: resolve(__dirname, 'Content/UI/NEON/MainMenu/index.html'),
        settings: resolve(__dirname, 'Content/UI/NEON/Settings/index.html'),
        hud: resolve(__dirname, 'Content/UI/NEON/HUD/index.html'),
        developer: resolve(__dirname, 'Content/UI/NEON/Developer/index.html')
      }
    },
    minify: 'terser',
    sourcemap: process.env.NODE_ENV === 'development'
  },
  server: {
    port: 5173,
    host: 'localhost',
    cors: true,
    hmr: {
      port: 5174
    }
  },
  css: {
    postcss: {
      plugins: [
        require('tailwindcss'),
        require('autoprefixer')
      ]
    }
  }
});

// package.json - Development scripts
{
  "name": "noham-base-project-ui",
  "version": "1.0.0",
  "type": "module",
  "scripts": {
    "dev": "vite",
    "build": "vite build",
    "preview": "vite preview",
    "build:watch": "vite build --watch",
    "lint": "eslint . --ext js,html",
    "format": "prettier --write .",
    "install:shadcn": "npx shadcn-ui@latest add",
    "clean": "rm -rf dist node_modules/.vite"
  },
  "dependencies": {
    "neon-web": "^1.0.0",
    "framer-motion": "^11.0.0",
    "lucide-icons": "^0.263.0",
    "tailwindcss": "^3.3.0"
  },
  "devDependencies": {
    "vite": "^5.0.0",
    "@vitejs/plugin-legacy": "^5.0.0",
    "autoprefixer": "^10.4.14",
    "postcss": "^8.4.24",
    "terser": "^5.19.0",
    "eslint": "^8.44.0",
    "prettier": "^2.8.8"
  }
}
```

### Integrated Development Workflow

#### Typical Development Session

```bash
# 1. Start Development Environment
./Scripts/start-dev-session.bat

# This script runs:
# - UE5 Editor with hot reload enabled
# - Vite dev server for NEON assets
# - File watchers for C++ changes
# - Performance monitoring dashboard

# 2. UI Development Workflow
cd Content/UI/NEON/Settings/
# Edit HTML/CSS/JS files
# Changes auto-reload in NEON widget via Vite HMR
# Test immediately in UE5 Editor

# 3. C++ Development Workflow
# Edit subsystem files in Visual Studio
# Use UE5 hot reload (Ctrl+Alt+F11)
# Test NEON communication immediately
# Debug with UE5 debugger + browser dev tools

# 4. Steam Integration Testing
# Install selected FAB store Steam plugin
# Configure Steam App ID in settings
# Test achievement unlocks, input detection
# Verify Steam Deck compatibility
```
