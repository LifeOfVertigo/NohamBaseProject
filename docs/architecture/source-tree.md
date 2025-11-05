# NohamBaseProject Source Tree Structure

## 🚨 CRITICAL: EXISTING PROJECT USAGE
**DO NOT CREATE NEW UE5 PROJECTS**
- Use existing project: `Noham_Base_Proj_Cpp/Noham_Base_Proj_Cpp.uproject`
- Extend existing Source/Noham_Base_Proj_Cpp/ directory structure
- DO NOT run UE5 project creation wizards

## Overview

UE5 project structure accommodating both C++ backend systems and NEON HTML/CSS/JS frontend components, organized for efficient development and packaging.

## Project Structure

```
NohamBaseProject/
├── .github/                           # CI/CD workflows
│   └── workflows/
│       ├── build-windows.yml
│       └── package-steam.yml
├── Source/                            # UE5 C++ source code
│   └── NohamBaseProject/
│       ├── Private/
│       │   ├── Subsystems/
│       │   │   ├── Settings/
│       │   │   ├── Input/
│       │   │   ├── Platform/
│       │   │   ├── NEON/
│       │   │   ├── Developer/
│       │   │   └── Core/
│       │   ├── Components/
│       │   │   ├── Characters/
│       │   │   └── UI/
│       │   └── Data/
│       ├── Public/
│       │   ├── Subsystems/
│       │   ├── Components/
│       │   └── Data/
│       ├── NohamBaseProject.Build.cs         # UE5 build configuration
│       └── NohamBaseProject.h                # Module header
├── Content/                           # UE5 content assets
│   ├── UI/                           # NEON web UI assets
│   │   ├── NEON/                     # Main NEON content directory
│   │   │   ├── MainMenu/
│   │   │   ├── Settings/
│   │   │   ├── HUD/
│   │   │   ├── Developer/
│   │   │   ├── Components/           # Reusable UI components
│   │   │   │   ├── shadcn/           # shadcn component library
│   │   │   │   ├── Game/             # Game-specific components
│   │   │   │   └── Utils/
│   │   │   ├── Styles/
│   │   │   ├── Assets/
│   │   │   └── package.json          # npm dependencies for shadcn/Vite
│   │   └── Widgets/                  # UMG widget containers
│   ├── Data/                         # Data tables and configurations
│   ├── Characters/                   # Character assets
│   ├── Levels/                       # Game levels
│   ├── Blueprints/                   # Blueprint assets
│   └── Audio/                        # Audio assets
├── Plugins/                          # UE5 plugins
│   ├── NEON/                         # NEON plugin (when available)
│   └── SteamIntegration/             # Steam plugin placeholder
├── Config/                           # UE5 configuration files
├── Scripts/                          # Development and build scripts
├── Docs/                            # Project documentation
├── Tools/                           # Development tools
├── .gitignore                       # Git ignore patterns
├── .gitattributes                   # Git LFS configuration for UE5 assets
├── NohamBaseProject.uproject        # UE5 project file
└── README.md                        # Project overview and setup
```

## Key Directory Details

### Source/NohamBaseProject/
UE5 C++ module containing all native game systems and NEON integration logic.

#### Subsystems Organization
- **Settings/**: Game settings management and Steam Cloud sync
- **Input/**: Multi-platform input detection and controller switching
- **Platform/**: Abstract Steam integration interface and implementations
- **NEON/**: Bridge communication between UE5 and HTML UI
- **Developer/**: Debug tools, performance monitoring, console commands
- **Core/**: Game mode, achievements, save/load operations

#### Components Organization
- **Characters/**: Player controller, movement, interaction, health systems
- **UI/**: NEON widget wrappers and HUD data management

### Content/UI/NEON/
Web development workspace containing HTML/CSS/JS for game UI.

#### Component Library Structure
- **shadcn/**: Professional UI components (Button, Slider, Input, etc.)
  - **CRITICAL:** All shadcn components MUST be placed in `Content/UI/NEON/Components/shadcn/`
  - **CRITICAL:** Use shadcn MCP tools to add/manage shadcn components - never manual installation
  - MCP handles component installation directly into UE5 Content folder structure
- **Game/**: Project-specific UI components and layouts
- **Utils/**: NEON communication helpers and validation utilities

#### Page Organization
- **MainMenu/**: Game entry point and navigation
- **Settings/**: Comprehensive settings interface with real-time preview
- **HUD/**: Real-time game state visualization
- **Developer/**: Debug tools and development utilities

### Content/Data/
UE5 Data Tables for configuration and game content.
- Input mappings for different platforms
- UI prompt configurations
- Game balance data
- Achievement definitions

### Config/
UE5 project configuration files including:
- Game settings defaults
- Input mappings
- Platform-specific configurations
- Steam integration settings

### Scripts/
Development automation and build scripts:
- Development environment setup
- Web asset building and optimization
- UE5 packaging automation
- Steam deployment scripts

## File Organization Principles

### UE5 C++ Standards
- Follow UE5 module structure conventions
- Separate Public/Private headers appropriately
- Use consistent naming with project prefix (`Noham`)
- Organize by functional responsibility, not technical implementation

### Web Asset Organization
- Treat HTML/CSS/JS as UE5 content assets
- Maintain clear separation between reusable components and page-specific code
- Use npm/Vite toolchain for dependency management
- Bundle all web assets for UE5 packaging system

### Cross-Platform Considerations
- Steam integration through abstract interface
- Input handling supports multiple controller types
- UI adapts to different input modes (Keyboard/Mouse, Gamepad, Steam Deck)
- Performance optimizations for both PC and Steam Deck targets

### Development Workflow Support
- Hot reload support for both C++ and web assets
- Clear separation of concerns for parallel development
- Integrated testing and debugging workflows
- CI/CD pipeline integration with automated builds

## Asset Management

### Version Control (Git LFS)
Large UE5 assets use Git LFS:
- `.uasset` files (Blueprints, materials, etc.)
- Audio files
- Texture assets
- Compiled binaries

### Package Management
- npm for web dependencies (Tailwind, Framer Motion, base libraries)
- **shadcn MCP** for shadcn/ui component management
  - **NEVER use `npx shadcn@latest add` commands**
  - **ALWAYS use shadcn MCP tools** (mcp__shadcn__*) to search, view, and add components
  - Components are automatically placed in `Content/UI/NEON/Components/shadcn/`
- UE5 marketplace for engine plugins
- FAB store for Steam integration plugins
- Manual integration for NEON plugin

### Build Artifacts
Generated files not tracked in version control:
- `Binaries/` directory (compiled C++ code)
- `Intermediate/` directory (build intermediates)
- `Content/UI/NEON/dist/` (bundled web assets)
- `node_modules/` (npm dependencies)

---

This structure enables efficient hybrid development with UE5 native systems, NEON web UI integration, and Steam platform deployment while maintaining clear separation of concerns and supporting AI-assisted development workflows.