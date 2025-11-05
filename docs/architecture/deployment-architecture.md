# Deployment Architecture

Steam-focused deployment strategy for UE5 + NEON + shadcn hybrid architecture, optimized for PC and Steam Deck distribution.

### Deployment Strategy

#### Platform Deployment Approach

**Primary Platform: Steam PC**
- **Build Target:** Windows 64-bit executable
- **Distribution:** Steam Direct platform
- **Packaging:** UE5 native packaging with embedded NEON web assets
- **Performance Target:** 120+ FPS on medium hardware
- **Steam Features:** Achievements, Cloud Saves, Steam Input, Steam Overlay

**Secondary Platform: Steam Deck**
- **Build Target:** Same Windows 64-bit executable (Proton compatibility)
- **Distribution:** Same Steam store listing with Steam Deck verification
- **Performance Target:** 60+ FPS on Steam Deck hardware
- **Handheld Features:** Steam Input, touch controls, docked/undocked detection
- **UI Adaptation:** Automatic input mode switching, touch-friendly scaling

### CI/CD Pipeline

#### GitHub Actions Workflow

```yaml
# .github/workflows/build-and-deploy.yml
name: Build and Deploy to Steam

on:
  push:
    branches: [main, release/*]
  pull_request:
    branches: [main]

jobs:
  build-web-assets:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
        with:
          lfs: true
      
      - name: Setup Node.js
        uses: actions/setup-node@v4
        with:
          node-version: '18'
          cache: 'npm'
          cache-dependency-path: 'Content/UI/NEON/package-lock.json'
      
      - name: Install dependencies
        run: |
          cd Content/UI/NEON/
          npm ci
      
      - name: Build production assets
        run: |
          cd Content/UI/NEON/
          npm run build:production
          npm run optimize:bundle-size
      
      - name: Upload web assets
        uses: actions/upload-artifact@v4
        with:
          name: neon-web-assets
          path: Content/UI/NEON/dist/

  build-ue5-project:
    runs-on: windows-latest
    needs: build-web-assets
    steps:
      - uses: actions/checkout@v4
        with:
          lfs: true
      
      - name: Download web assets
        uses: actions/download-artifact@v4
        with:
          name: neon-web-assets
          path: Content/UI/NEON/dist/
      
      - name: Build C++ Project
        run: |
          "C:\Program Files\Epic Games\UE_5.5\Engine\Binaries\DotNET\UnrealBuildTool.exe" ^
            NohamBaseProject Win64 Shipping ^
            -project="NohamBaseProject.uproject" ^
            -rocket -progress
      
      - name: Package UE5 Project
        run: |
          "C:\Program Files\Epic Games\UE_5.5\Engine\Binaries\DotNET\UnrealBuildTool.exe" ^
            NohamBaseProject Win64 Shipping ^
            -project="NohamBaseProject.uproject" ^
            -rocket -cook -stage -package -pak

  deploy-to-steam:
    runs-on: ubuntu-latest
    needs: build-ue5-project
    if: github.ref == 'refs/heads/main'
    steps:
      - name: Deploy to Steam
        uses: game-ci/steam-deploy@v3
        with:
          username: ${{ secrets.STEAM_USERNAME }}
          configVdf: ${{ secrets.STEAM_CONFIG_VDF }}
          appId: ${{ secrets.STEAM_APP_ID }}
          buildDescription: "Build ${{ github.sha }}"
          rootPath: ./game-build/
```

### Steam Platform Integration

#### Steam Store Configuration

```yaml
# Steam store metadata
App Configuration:
  Name: "NohamBaseProject"
  Type: "Game"
  Platforms: ["Windows"]
  Steam Deck Compatibility: "Verified"
  
Features:
  - Steam Cloud (Save Game Sync)
  - Steam Achievements
  - Steam Input API
  - Steam Overlay
  - Steam Remote Play
  - Steam Deck Optimized

System Requirements:
  Minimum:
    OS: "Windows 10 64-bit"
    Processor: "Intel i5-4590 / AMD FX 8350"
    Memory: "8 GB RAM"
    Graphics: "NVIDIA GTX 970 / AMD R9 280"
    DirectX: "Version 12"
    Storage: "5 GB available space"
  
  Recommended:
    OS: "Windows 11 64-bit"
    Processor: "Intel i7-8700K / AMD Ryzen 5 3600"
    Memory: "16 GB RAM"
    Graphics: "NVIDIA GTX 1070 / AMD RX 580"
    DirectX: "Version 12"
    Storage: "5 GB available space"

Steam Deck:
  Supported: "Yes"
  Optimized: "Yes"
  Target FPS: "60"
  Control Scheme: "Gamepad + Touch"
```

### Performance Optimization for Deployment

#### Asset Optimization

```bash
# Web Asset Optimization
cd Content/UI/NEON/
npm run build:production

# Outputs:
# - Minified HTML/CSS/JS bundles
# - Compressed shadcn components
# - Optimized images and icons
# - Tree-shaken dependencies
# - Gzipped assets for faster loading

# Bundle Size Targets:
# - Total web assets: <50MB
# - UE5 content: <4GB
# - Final package: <5GB
# - Steam Deck install: <3GB (compressed)
```

### Monitoring and Analytics

#### Production Monitoring

```yaml
# Steam Analytics Integration
Steam Metrics:
  - Player session duration
  - Achievement unlock rates
  - Settings preference analysis
  - Steam Deck vs PC usage patterns
  - Performance metrics (FPS drops, crashes)

Custom Analytics:
  - NEON UI performance metrics
  - Settings change frequency
  - Menu navigation patterns
  - Input mode switching frequency
  - Error rates and crash reports
```

---

🤖 Generated with [Claude Code](https://claude.ai/code)

Co-Authored-By: Claude <noreply@anthropic.com>