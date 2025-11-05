# NohamBaseProject

A standardized UE5 5.5.4 base project template designed for AI-assisted development with NEON UI integration, Steam platform support, and shadcn component library.

## Project Overview

This base template provides:
- Clean UE5 C++ project structure following AI-readable naming conventions
- Foundation for NEON plugin integration (HTML/CSS/JS UI in UE5)
- Steam platform integration framework
- shadcn/ui component library support
- Comprehensive development tools and debugging framework

## Prerequisites

- **Unreal Engine 5.5.4** (exact version required)
- **Visual Studio 2022** with C++ game development workload
- **NEON Plugin** installed at engine level
- **Git** for version control
- **Node.js 18+** (for future web UI dependencies)

## Setup Instructions

### 1. Open the Project

```bash
# Navigate to the project directory
cd "C:\Users\tyler\OneDrive\Documentos\Unreal Projects\NohamBaseProject"

# Double-click Noham_Base_Proj_Cpp.uproject to open in UE5 Editor
```

### 2. Generate Visual Studio Project Files (if needed)

```bash
# Right-click on Noham_Base_Proj_Cpp.uproject and select "Generate Visual Studio project files"
```

### 3. Build the Project

```bash
# Open Noham_Base_Proj_Cpp.sln in Visual Studio 2022
# Set to Development Editor configuration
# Build Solution (Ctrl+Shift+B)
```

## Project Structure

```
NohamBaseProject/
├── Noham_Base_Proj_Cpp/               # Main UE5 project
│   ├── Source/Noham_Base_Proj_Cpp/    # C++ source code
│   │   ├── Private/
│   │   │   ├── Subsystems/            # Game subsystems
│   │   │   │   ├── Core/              # Core game systems [HAS FILES]
│   │   │   │   ├── Settings/          # Settings management [READY]
│   │   │   │   ├── Input/             # Input handling [READY]
│   │   │   │   ├── Platform/          # Steam integration [READY]
│   │   │   │   ├── NEON/              # UI bridge [READY - Epic 2]
│   │   │   │   └── Developer/         # Debug tools [READY - Story 1.3-R]
│   │   │   ├── Components/            # Actor components
│   │   │   │   ├── Characters/        # Character systems [HAS FILES]
│   │   │   │   └── UI/                # UI components [READY]
│   │   │   └── Data/                  # Data structures [READY]
│   │   └── Public/                    # Public headers (mirrors Private/)
│   ├── Content/                       # UE5 content assets
│   │   ├── FirstPerson/               # Base template content
│   │   ├── UI/                        # NEON UI assets (future)
│   │   ├── Data/                      # Data tables
│   │   ├── Characters/                # Character assets
│   │   ├── Levels/                    # Game levels
│   │   ├── Blueprints/                # Blueprint assets
│   │   └── Audio/                     # Audio assets
│   ├── Config/                        # UE5 configuration
│   └── Scripts/                       # Build and automation scripts (future)
├── docs/                              # Project documentation
│   ├── architecture/                  # Architecture documentation
│   ├── stories/                       # Development stories
│   ├── unreal-engine/                 # UE5 C++ documentation cache
│   └── developer-tools/               # Developer tools docs (future)
├── templates/                         # C++ class templates (Story 1.4-R)
└── CLEANUP_LOG.md                     # Epic 1 cleanup documentation
```

**Note:** Directories marked [READY] have been created with `.gitkeep` files and are ready for implementation in upcoming stories.

## Key Classes

### Character System
- **ANohamCharacter**: Clean base character with first-person camera and enhanced input
- Located in `Public/Components/Characters/NohamCharacter.h`

### Game Mode
- **ANohamGameMode**: Basic game mode that spawns the base character
- Located in `Public/Subsystems/Core/NohamGameMode.h`

## Development Guidelines

### Naming Conventions

Follow the UE5 prefix conventions + `Noham` project prefix (see `docs/architecture/coding-standards.md`):

- **Actors**: `ANoham[ClassName]` (e.g., `ANohamCharacter`, `ANohamGameMode`)
- **UObjects/Components**: `UNoham[ClassName]` (e.g., `UNohamSettingsSubsystem`)
- **Structs**: `FNoham[StructName]` (e.g., `FNohamSettingsData`)
- **Interfaces**: `INoham[InterfaceName]` (e.g., `INohamPlatformInterface`)
- **Enums**: `ENoham[EnumName]` (e.g., `ENohamInputMode`)
- **Functions**: PascalCase (e.g., `UpdateGraphicsSettings`)
- **Variables**: PascalCase with type prefix (e.g., `bIsDebugModeEnabled`, `CurrentHealth`)
- **Files**: Match class names exactly

### Code Organization

- **Subsystems** for singleton game services
- **Components** for reusable functionality
- **Public headers** only for interfaces used by other modules
- **Private implementation** in dedicated folders by category

## Current Status

✅ **Story 1.1-R COMPLETE**: Project structure remediation finished (2025-10-25)
- ✅ Old FPS template files removed (verified non-existent)
- ✅ Complete directory structure created (14 new directories)
- ✅ Files reorganized to correct subdirectories
- ✅ Class redirects added to DefaultEngine.ini
- ✅ Naming conventions clarified in coding-standards.md
- ✅ Project files regenerated successfully
- ✅ README updated with final structure

See `CLEANUP_LOG.md` for complete remediation details.

✅ **Story 1.2 SUBSTANTIALLY COMPLETE**: Documentation system
- ✅ Excellent UE5 documentation cache at `docs/unreal-engine/`
- ✅ shadcn MCP integration functional
- ✅ Context7 MCP integration functional
- ⚠️ Minor: AI prompt templates deferred to Story 1.4-R

### Next Steps (Epic 1 Remediation)

1. **Story 1.3-R**: Developer Tools Implementation (CRITICAL - 3-4 days)
   - Implement DeveloperToolsSubsystem
   - Performance monitoring
   - Collision debugging
   - Console commands
   - F1 developer menu

2. **Story 1.4-R**: Template System Implementation (HIGH PRIORITY - 4-5 days)
   - C++ class templates
   - Python generation script
   - ExampleSubsystem
   - Template documentation

## Building and Testing

### Development Builds

```bash
# In UE5 Editor: Build -> Build [ProjectName]
# Or via Visual Studio: Build -> Build Solution
```

### Verify Functionality

1. Open the project in UE5 Editor
2. Play in Editor (PIE) to test basic movement
3. Verify character spawns and camera works correctly
4. Check console for any errors

## Troubleshooting

### Common Issues

1. **Build Errors**: Ensure UE5 5.5.4 and NEON plugin are correctly installed
2. **Missing Dependencies**: Run `Generate Visual Studio project files` again
3. **Blueprint Compilation**: Check for missing C++ class references

## Next Epic Dependencies

Epic 1 provides the foundation for Epic 2 (NEON Integration). All base systems and coding standards must be validated before proceeding to UI framework implementation.

## License

Copyright Epic Games, Inc. All Rights Reserved.
Licensed under the Unreal Engine EULA.