# NEON Widget + React Integration Guide

## The Problem Explained

### Why Basic HTML Works But React/shadcn Doesn't

**Basic HTML** (`test-basic.html`):
- ✅ Self-contained with inline CSS/JS
- ✅ Works with `file://` protocol immediately
- ✅ No external dependencies

**React/TypeScript** (`test-shadcn.tsx`, MainMenu):
- ❌ `.tsx` files cannot be executed by browsers natively
- ❌ Requires transpilation: TypeScript → JavaScript
- ❌ Requires bundling: Combining modules + npm dependencies
- ❌ `file://` protocol can't access `node_modules`

### Why Dev Server "Works" (But Isn't a Solution)

When you run `npm run dev`:
1. Vite dev server starts at `http://localhost:5173`
2. Server intercepts `.tsx` file requests
3. Transpiles TypeScript → JavaScript on-the-fly
4. Bundles dependencies dynamically
5. Serves via HTTP with Hot Module Replacement (HMR)

**Why this won't work for distribution:**
- Players can't run `npm run dev`
- Game needs offline menus
- No external dependencies allowed

## The Solution: Production Builds

### Step 1: Build the React App

```bash
cd shadcn-base
npm run build
```

This creates a `dist` folder with:
- `index.html` - Entry point with relative paths
- `assets/index-[hash].js` - Bundled JavaScript (React + all dependencies)
- `assets/index-[hash].css` - Bundled CSS (Tailwind + components)

### Step 2: Copy to NEON Content Folder

```bash
# From project root
cp -r shadcn-base/dist/* Noham_Base_Proj_Cpp/Content/UI/NEON/MainMenu/dist/
```

Or use the automated script:
```bash
cd shadcn-base
./build-for-ue.sh  # Linux/Mac
build-for-ue.bat   # Windows
```

### Step 3: Configure NEON Widget in Unreal Engine

In your NEON widget blueprint or C++:

**For Editor (Development):**
- Development URL: `http://localhost:5173` (when dev server is running)

**For Game (Production):**
- Live URL: `/Content/UI/NEON/MainMenu/dist/index.html`

The widget will automatically use:
- Development URL in Editor (`WITH_EDITOR`)
- Live URL in packaged game

## Development Workflow

### During Development

1. **Start dev server:**
   ```bash
   cd shadcn-base
   npm run dev
   ```

2. **In Unreal Editor:**
   - Set NEON widget Development URL to `http://localhost:5173`
   - Make changes in React code
   - See updates instantly via HMR

### For Testing/Distribution

1. **Build production bundle:**
   ```bash
   cd shadcn-base
   npm run build
   ```

2. **Copy to NEON folder:**
   ```bash
   cp -r dist/* ../Noham_Base_Proj_Cpp/Content/UI/NEON/MainMenu/dist/
   ```

3. **In Unreal Editor:**
   - Set NEON widget Live URL to `/Content/UI/NEON/MainMenu/dist/index.html`
   - Test in PIE (Play In Editor)
   - Verify it works WITHOUT dev server running

4. **Package game:**
   - Bundled files will be included automatically
   - No external server needed
   - Works completely offline

## Key Configuration Changes Made

### vite.config.ts

```typescript
export default defineConfig({
  base: './', // Relative paths for file:// protocol
  build: {
    outDir: 'dist',
    assetsDir: 'assets',
    rollupOptions: {
      output: {
        entryFileNames: 'assets/[name]-[hash].js',
        chunkFileNames: 'assets/[name]-[hash].js',
        assetFileNames: 'assets/[name]-[hash].[ext]'
      }
    }
  }
})
```

**Why this matters:**
- `base: './'` generates relative paths (`./assets/...` instead of `/assets/...`)
- Relative paths work with `file://` protocol
- Absolute paths would look for files at root of C: drive

## Troubleshooting

### Problem: "Unstyled text in top left corner"

**Cause:** Loading `.tsx` files directly without transpilation
**Solution:** Use production build from `dist` folder

### Problem: "Nothing shows unless dev server is running"

**Cause:** NEON widget pointing to `http://localhost:5173` or raw `.tsx` files
**Solution:** Build production bundle and point to `/Content/UI/NEON/MainMenu/dist/index.html`

### Problem: "CSS not loading in packaged game"

**Cause:** Absolute paths in HTML (`/assets/...`)
**Solution:** Already fixed with `base: './'` in vite.config.ts

### Problem: "JavaScript errors about missing modules"

**Cause:** Not all dependencies bundled
**Solution:** Run `npm install` then `npm run build` again

## File Structure After Build

```
Noham_Base_Proj_Cpp/
└── Content/
    └── UI/
        └── NEON/
            └── MainMenu/
                ├── dist/               ← Production bundle (commit to git)
                │   ├── index.html     ← Entry point
                │   └── assets/
                │       ├── index-[hash].js   ← All React code + dependencies
                │       └── index-[hash].css  ← All styles
                ├── index.html         ← Old (references .tsx, won't work)
                └── index.tsx          ← React source (for reference)
```

## Quick Commands Reference

```bash
# Install dependencies
cd shadcn-base && npm install

# Start dev server (for development)
npm run dev

# Build for production (for testing/distribution)
npm run build

# Copy to NEON folder (after building)
cp -r dist/* ../Noham_Base_Proj_Cpp/Content/UI/NEON/MainMenu/dist/

# All-in-one: build and copy
npm run build && cp -r dist/* ../Noham_Base_Proj_Cpp/Content/UI/NEON/MainMenu/dist/
```
