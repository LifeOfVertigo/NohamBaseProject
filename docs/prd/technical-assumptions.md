# Technical Assumptions

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
