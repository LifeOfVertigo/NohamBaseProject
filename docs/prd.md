# NohamBaseProject Product Requirements Document (PRD)

## Goals and Background Context

### Goals
Based on your Project Brief, here are the key desired outcomes for the NohamBaseProject PRD:

• **Accelerate prototype development** - Reduce development time by 50% through reusable base systems and AI-optimized architecture
• **Establish AI-readable codebase** - Create standardized C++ conventions and documentation that enable effective AI-assisted development  
• **Integrate NEON+shadcn UI system** - Enable rapid, beautiful menu creation using HTML/JavaScript within UE5 widgets
• **Build reusable foundation components** - Essential systems (character controller, input handling, settings management) for 100% reusability across projects
• **Enable multi-platform support** - Steam PC primary with Steam Deck compatibility and multi-controller input framework
• **Create comprehensive documentation system** - Local file-based documentation accessible to both AI assistants and human developers

### Background Context

NohamBaseProject addresses critical inefficiencies in indie game development for a small but successful studio (1-2 developers) with proven Steam PC game releases. Currently, core systems like menus, settings, input handling, and character controllers are rebuilt from scratch for each project, resulting in slower time-to-market and inconsistent code quality despite the studio's track record of 3-4 successful games generating significant side income.

The solution leverages Unreal Engine 5.5.4 as a foundation, integrating the NEON plugin for HTML/JavaScript UI development with shadcn components. This creates a unique bridge enabling AI-assisted menu creation while maintaining the performance and capabilities of native UE5 development. The architecture prioritizes AI-readability through standardized naming conventions and comprehensive local documentation, positioning the studio to maximize productivity gains from modern AI development tools.

### Change Log
| Date | Version | Description | Author |
|------|---------|-------------|---------|
| 2025-10-25 | v1.0 | Initial PRD creation from Project Brief | John (PM) |

## Requirements

### Functional Requirements

**FR1:** The base project shall provide a standardized C++ code structure with AI-readable naming conventions and file organization
**FR2:** The system shall integrate NEON plugin with shadcn UI components for HTML/JavaScript menu development within UE5 widgets
**FR3:** The base project shall include essential menu systems: main menu, settings (game/audio/graphics/keybinds/language), escape menu, basic HUD, and developer menu
**FR4:** The system shall provide a basic FPS character controller with first-person movement, crouch, jump, aim, and interact (E key) functionality
**FR5:** The base project shall implement settings management for graphics (resolution, fullscreen, VSync, FPS cap), audio (volume controls), and game settings with persistence
**FR6:** The system shall support multi-controller input framework with keyboard/mouse, gamepad, and Steam Deck support including context switching
**FR7:** The base project shall include development tools: time controls, performance metrics display, collision visualization, and function trigger menu
**FR8:** The system shall provide local documentation integration for shadcn, UE5 C++, and component documentation accessible to AI assistants

### Non-Functional Requirements

**NFR1:** The base project must maintain stable 120+ FPS performance on medium hardware with low-poly/retro graphics optimization
**NFR2:** Setup time for new game projects using this base must be under 2 hours from copy to functional prototype
**NFR3:** Menu creation time must be under 1 day for complete game menu systems using AI assistance
**NFR4:** The system must achieve 95% Steam Deck compatibility without additional development work
**NFR5:** Code structure must enable >80% base code reusability across derived game projects
**NFR6:** Documentation lookup time must be under 30 seconds for any component or system reference
**NFR7:** The base project must maintain compatibility with UE5.5.4 and existing successful game project patterns

## User Interface Design Goals

### Overall UX Vision
Clean, responsive interface that leverages modern web UI patterns through shadcn components while maintaining native UE5 performance. The UI should feel familiar to PC gamers with Steam-like navigation patterns, emphasizing clarity and accessibility over flashy effects. AI-assisted development workflow should enable rapid iteration and consistent design language across all menus.

### Key Interaction Paradigms
- **Primary Navigation:** Tab-based menu systems with keyboard shortcuts and gamepad d-pad navigation
- **Settings Management:** Immediate preview with apply/revert functionality for graphics settings
- **Context-Sensitive UI:** Dynamic input prompts that switch between keyboard/mouse and gamepad icons based on active input method
- **Developer Tools Integration:** Quick-access overlay menus triggered by function keys, non-intrusive performance displays

### Core Screens and Views
- **Main Menu** - Game launch entry point with new game, continue, settings, quit options
- **Settings Menu** - Tabbed interface (Game/Audio/Graphics/Controls/Language) with real-time preview
- **In-Game HUD** - Minimal overlay showing essential game state information
- **Escape/Pause Menu** - Quick access to settings, save, return to main menu during gameplay
- **Developer Menu** - Debug tools, performance metrics, collision visualization, time controls

### Accessibility: WCAG AA
Ensuring keyboard navigation, sufficient color contrast, and readable text scaling to meet WCAG AA standards while maintaining the retro/indie aesthetic.

### Branding
Flexible theming system that can accommodate various indie game aesthetics - from pixel art to low-poly 3D. Default clean, modern styling using shadcn's design system with customizable color schemes and typography that can be easily modified per game project.

### Target Device and Platforms: Web Responsive
Primary focus on PC desktop with full keyboard/mouse support, secondary Steam Deck optimization with touch-friendly elements and gamepad navigation. All UI elements scale appropriately for different screen resolutions while maintaining readability.

## Technical Assumptions

### Repository Structure: Monorepo
Single repository containing the base project with clear separation between foundational systems and game-specific extensions. This approach supports your goal of rapid project setup (<2 hours) by maintaining all base components in one place while enabling easy duplication and customization for new games.

### Service Architecture
**Monolith within UE5 Framework:** Traditional UE5 C++ project structure with modular component architecture. Core systems (input, settings, UI, character controller) implemented as reusable UE5 components and subsystems. NEON plugin integration maintains separation between HTML/JS UI layer and native C++ game logic while operating within single executable.

### Testing Requirements
**Unit + Integration Testing:** Automated unit tests for core C++ components and integration tests for NEON+shadcn UI interactions. Manual testing convenience methods for rapid gameplay verification. Focus on base system reliability since derived game projects will depend on this foundation's stability.

### Additional Technical Assumptions and Requests

**Engine & Platform:**
- Unreal Engine 5.5.4 (specific version for consistency across projects)
- Target: Steam PC primary, Steam Deck secondary
- Performance: Optimized for 120+ FPS on medium hardware with low-poly/retro graphics

**UI Technology Stack:**
- NEON plugin for HTML/JavaScript integration within UE5 widgets
- shadcn UI component library for modern, accessible interface elements
- CSS-in-JS or similar styling approach compatible with NEON rendering

**Development Tools Integration:**
- Claude Code CLI compatibility with standardized file structure
- BMAD method agents support through clear naming conventions
- Local documentation system (shadcn, UE5 C++, custom components)

**Input & Platform Support:**
- Multi-controller framework: keyboard/mouse, Xbox/PlayStation controllers, Steam Deck controls
- Input context switching with dynamic UI prompts
- Steam API integration for platform-specific features

**Code Architecture:**
- AI-readable naming conventions and file organization
- Template-driven development patterns for rapid component creation
- Modular design enabling easy extension without modifying base systems

## Epic List

**Epic 1: Foundation & AI-Optimized Infrastructure**
Establish project setup, standardized code structure, documentation system, and basic development tools while delivering a functional "Hello World" UE5 project with AI-readable architecture.

**Epic 2: NEON Plugin Integration & UI Framework**
Integrate NEON plugin with shadcn components, establish HTML/JavaScript UI pipeline, and deliver functional main menu system demonstrating the AI-assisted UI development workflow.

**Epic 3: Core Character Controller & Input Systems**
Implement FPS character controller with movement, interaction, and multi-platform input framework, delivering playable character control with Steam Deck compatibility.

**Epic 4: Settings Management & Persistence**
Create comprehensive settings system for graphics, audio, controls, and game preferences with real-time preview and persistence, completing the essential menu ecosystem.

**Epic 5: Developer Tools & Documentation Integration**
Implement debug tools, performance metrics, local documentation access, and AI development conveniences, finalizing the development-ready base project.

## Epic 1: Foundation & AI-Optimized Infrastructure

**Epic Goal:** Establish the fundamental project structure, AI-readable code architecture, and essential development infrastructure while delivering a functional UE5 base project that demonstrates the standardized patterns and documentation system. This epic creates the foundation that enables all subsequent development and AI-assisted workflows.

### Story 1.1: Project Structure & AI-Readable Architecture
As a developer,
I want a standardized UE5 project with AI-optimized file organization and naming conventions,
so that AI assistants can quickly understand and extend the codebase while maintaining human readability.

**Acceptance Criteria:**
1. UE5 5.5.4 project created with clear folder structure separating base systems from game-specific code
2. C++ naming conventions documented and implemented following AI-readable patterns
3. File organization enables quick navigation for both humans and AI assistants
4. Basic project compiles and runs successfully in UE5 editor
5. README and development setup instructions created and validated

### Story 1.2: Local Documentation System
As an AI assistant and developer,
I want comprehensive local documentation for UE5 C++, NEON plugin, and shadcn components,
so that development can proceed efficiently without external API lookups or token-heavy web searches.

**Acceptance Criteria:**
1. UE5 C++ API documentation downloaded and organized locally
2. NEON plugin documentation integrated into project structure
3. shadcn component library documentation available offline
4. Documentation access patterns established for AI assistant usage
5. Quick reference guides created for common development tasks

### Story 1.3: Basic Development Tools Integration
As a developer,
I want essential development tools and debugging capabilities integrated into the base project,
so that I can efficiently develop and troubleshoot during the foundation building phase.

**Acceptance Criteria:**
1. Basic console commands implemented for common development tasks
2. Performance monitoring framework established (FPS display, memory usage)
3. Debug logging system configured with appropriate verbosity levels
4. Basic collision visualization tools available
5. Development menu accessible via function key (F1 or similar)

### Story 1.4: Template System Foundation
As a developer,
I want standardized templates for creating new classes and components,
so that all future development follows consistent patterns and AI assistants can predict code structure.

**Acceptance Criteria:**
1. C++ class templates created for common UE5 component types
2. Header and implementation file templates follow established naming conventions
3. Template generation tools or scripts provided for rapid class creation
4. Example components created demonstrating template usage
5. Documentation explaining template patterns and usage guidelines

## Epic 2: NEON Plugin Integration & UI Framework

**Epic Goal:** Integrate NEON plugin with shadcn components to establish the HTML/JavaScript UI pipeline within UE5 widgets. This epic delivers a functional main menu system that demonstrates AI-assisted UI development workflow and proves the core technical concept enabling rapid menu creation.

### Story 2.1: NEON Plugin Setup & Configuration
As a developer,
I want NEON plugin properly integrated and configured in the UE5 project,
so that HTML/JavaScript UI components can be rendered within UE5 widgets and communicate with C++ game logic.

**Acceptance Criteria:**
1. NEON plugin installed and configured for UE5 5.5.4 compatibility
2. Basic HTML rendering test successful within UE5 widget
3. JavaScript-to-C++ communication channel established and tested
4. NEON widget creation workflow documented with examples
5. Performance baseline established for HTML rendering within UE5

### Story 2.2: shadcn Component Integration
As a developer,
I want shadcn UI components working within NEON widgets,
so that I can leverage modern web UI patterns for rapid menu development with consistent styling.

**Acceptance Criteria:**
1. shadcn component library integrated into NEON rendering context
2. Basic components (buttons, inputs, cards) render correctly in UE5
3. shadcn theming system adapted for game UI requirements
4. Component styling responsive to different screen resolutions
5. Integration documented with component usage examples

### Story 2.3: Main Menu Implementation
As a player,
I want a functional main menu with New Game, Continue, Settings, and Quit options,
so that I can navigate the game's primary functions through an intuitive interface.

**Acceptance Criteria:**
1. Main menu UI created using shadcn components within NEON widget
2. Menu options (New Game, Continue, Settings, Quit) functional and properly routed
3. Menu navigation supports both keyboard/mouse and gamepad input
4. Visual design follows modern UI patterns while fitting indie game aesthetic
5. Menu transitions smooth and performant (maintains target FPS)

### Story 2.4: AI-Assisted UI Development Workflow
As a developer,
I want documented patterns for AI-assisted menu creation using NEON+shadcn,
so that future UI development can leverage AI code generation effectively and consistently.

**Acceptance Criteria:**
1. Standardized patterns documented for creating new NEON-based menus
2. Template files created for common UI component combinations
3. AI prompt examples provided for generating menu code
4. Code generation workflow tested and validated with AI assistant
5. Best practices guide created for NEON+shadcn+AI development

## Epic 3: Core Character Controller & Input Systems

**Epic Goal:** Implement a comprehensive FPS character controller with movement, interaction capabilities, and multi-platform input framework. This epic delivers playable character control that works seamlessly across keyboard/mouse, gamepad, and Steam Deck, providing the essential gameplay foundation for all future game projects.

### Story 3.1: Basic FPS Movement Controller
As a player,
I want responsive first-person movement with WASD controls, mouse look, and smooth camera handling,
so that I can navigate the game world intuitively with standard FPS controls.

**Acceptance Criteria:**
1. First-person camera controller with smooth mouse look and configurable sensitivity
2. WASD movement with proper acceleration and deceleration
3. Movement speed configurable through game settings
4. Camera collision prevention and smooth movement interpolation
5. Movement system maintains 120+ FPS performance target

### Story 3.2: Advanced Movement Mechanics
As a player,
I want additional movement options including crouch, jump, and aim functionality,
so that I can interact with the game environment in standard FPS ways.

**Acceptance Criteria:**
1. Crouch functionality with smooth height transition and speed modification
2. Jump mechanics with configurable height and gravity settings
3. Aim-down-sights functionality with FOV adjustment and movement speed reduction
4. Sprint capability with stamina considerations (framework for future implementation)
5. All movement mechanics work smoothly together without conflicts

### Story 3.3: Interaction System
As a player,
I want to interact with objects in the game world using the E key,
so that I can engage with interactive elements and trigger game events.

**Acceptance Criteria:**
1. Interaction raycast system detecting objects within interaction range
2. Visual feedback for interactable objects (outline, prompt, or similar)
3. E key interaction with objects implementing interaction interface
4. Interaction prompts display appropriate input method (keyboard vs gamepad)
5. Example interactable objects created to demonstrate system functionality

### Story 3.4: Multi-Platform Input Framework
As a player using different input devices,
I want seamless switching between keyboard/mouse, gamepad, and Steam Deck controls,
so that I can play comfortably regardless of my preferred input method or platform.

**Acceptance Criteria:**
1. Input detection automatically switches between keyboard/mouse and gamepad modes
2. UI prompts dynamically update to show correct input icons for active device
3. All character controller functions mapped for both input types
4. Steam Deck specific input considerations implemented and tested
5. Input settings allow full customization of key bindings and controller mappings

## Epic 4: Settings Management & Persistence

**Epic Goal:** Create a comprehensive settings system for graphics, audio, controls, and game preferences with real-time preview capabilities and persistent storage. This epic completes the essential menu ecosystem by providing players full control over their gameplay experience while demonstrating advanced NEON+shadcn UI patterns.

### Story 4.1: Graphics Settings Implementation
As a player,
I want comprehensive graphics settings with real-time preview,
so that I can optimize visual quality and performance for my hardware setup.

**Acceptance Criteria:**
1. Graphics settings menu created using NEON+shadcn components with tabbed interface
2. Resolution selection with automatic detection of supported resolutions
3. Fullscreen/windowed mode toggle with immediate application
4. VSync and FPS cap settings with real-time performance feedback
5. Settings changes preview immediately without requiring restart

### Story 4.2: Audio Settings System
As a player,
I want audio control settings for different sound categories,
so that I can customize my audio experience according to my preferences and environment.

**Acceptance Criteria:**
1. Audio settings tab with volume sliders for Master, Music, SFX, and Voice categories
2. Audio device selection for systems with multiple output devices
3. Volume changes apply immediately with audio feedback
4. Audio settings persist between game sessions
5. Mute toggles available for each audio category

### Story 4.3: Input & Control Settings
As a player,
I want to customize my input controls and sensitivity settings,
so that I can configure the game to match my preferred control scheme and comfort level.

**Acceptance Criteria:**
1. Control settings tab showing current key bindings for all game actions
2. Key rebinding interface allowing players to customize any input mapping
3. Mouse sensitivity sliders with real-time feedback during adjustment
4. Gamepad settings including dead zone and sensitivity configurations
5. Reset to defaults option restores original control scheme

### Story 4.4: Settings Persistence & Management
As a player,
I want my settings automatically saved and restored,
so that my preferences are maintained across game sessions without manual intervention.

**Acceptance Criteria:**
1. Settings automatically saved to persistent storage when changed
2. Settings loaded on game startup and applied correctly
3. Apply/Revert functionality for testing settings before committing
4. Settings validation prevents invalid configurations
5. Backup and restore mechanism for settings recovery

## Epic 5: Developer Tools & Documentation Integration

**Epic Goal:** Implement comprehensive debug tools, performance metrics, local documentation access, and AI development conveniences that finalize the development-ready base project. This epic ensures maximum developer productivity and provides the tools necessary for efficient game development and AI-assisted workflows.

### Story 5.1: Debug Tools & Visualization
As a developer,
I want essential debug tools and visualization capabilities,
so that I can efficiently troubleshoot issues during development without unnecessary complexity.

**Acceptance Criteria:**
1. Collision visualization toggle showing hitboxes, triggers, and navigation meshes
2. Debug draw functions for rays, spheres, boxes, and custom shapes
3. Object inspection tool displaying component details and property values
4. Debug camera mode allowing free-look navigation independent of player character
5. Debug tools accessible via function keys and developer menu interface

### Story 5.2: Performance Monitoring System
As a developer,
I want real-time performance monitoring,
so that I can maintain the 120+ FPS target and identify performance issues.

**Acceptance Criteria:**
1. Real-time FPS counter with frame time statistics
2. Memory usage monitoring showing essential memory consumption data
3. Performance metrics overlay toggleable and non-intrusive during gameplay
4. Performance data exportable for analysis when needed
5. Performance monitoring maintains minimal overhead on game performance

### Story 5.3: AI-Focused Documentation System
As an AI assistant,
I want access to essential project documentation for code generation,
so that I can understand project patterns and generate appropriate C++ code that connects to Blueprint systems.

**Acceptance Criteria:**
1. Local documentation for project coding conventions and patterns accessible to AI
2. C++ to Blueprint integration patterns documented with examples
3. NEON plugin integration documentation available for AI reference
4. shadcn component usage patterns documented for UI development
5. Template and pattern documentation optimized for AI parsing and understanding

### Story 5.4: AI Development Integration
As a developer working with AI assistants,
I want tools that enhance AI code generation for C++ components,
so that AI-generated C++ integrates smoothly with Blueprint workflows.

**Acceptance Criteria:**
1. C++ component templates following Blueprint-friendly patterns
2. Code generation templates for common C++ to Blueprint interfaces
3. Standardized naming conventions documented for AI consistency
4. AI prompt templates for generating Blueprint-compatible C++ code
5. Component creation patterns that ensure proper Blueprint integration

### Story 5.5: Essential Time Controls
As a developer,
I want basic time manipulation controls for testing,
so that I can efficiently test different scenarios during development.

**Acceptance Criteria:**
1. Time scale controls (pause, slow motion, fast forward) accessible via hotkeys
2. Time controls integrated into developer menu interface
3. Time manipulation works reliably across all game systems
4. Time controls reset properly when disabled
5. Time scale changes visible in developer overlay for confirmation