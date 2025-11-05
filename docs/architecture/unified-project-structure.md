# Unified Project Structure

UE5 project structure accommodating both C++ backend systems and NEON HTML/CSS/JS frontend components, organized for efficient development and packaging.

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
