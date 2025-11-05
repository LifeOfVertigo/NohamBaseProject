# Project Brief: UE5 Master Base Project (NohamBaseProject)

## Executive Summary

**NohamBaseProject** is a master Unreal Engine 5.5.4 base project designed to accelerate indie game development for a small studio (1-2 developers). The project integrates the NEON plugin for HTML/JavaScript UI development with shadcn components, enabling AI-assisted menu creation and rapid prototyping. This foundation provides standardized systems, AI-optimized code structure, and essential game components that can be leveraged across multiple Steam PC game projects, significantly reducing development time and improving code quality.

## Problem Statement

**Current Development Inefficiencies:**
- Rebuilding core systems (menus, settings, input handling, character controllers) from scratch for each new game project
- Time-consuming manual UI development that could be accelerated with AI assistance
- Inconsistent code structure and naming conventions across projects
- Lack of standardized documentation and development tools
- Difficulty maintaining code quality and professional standards with limited resources

**Impact:**
- Slower time-to-market for game prototypes and full releases
- Reduced development efficiency despite having generated significant side income from 3-4 successful games
- Missing opportunities to leverage AI tools effectively due to poor code structure
- Inconsistent user experience across game releases

**Why Now:**
The studio is ready to scale development efficiency and quality to maximize their proven ability to create successful indie games. With AI development tools becoming more sophisticated, having a properly structured base project will unlock significant productivity gains.

## Proposed Solution

**Core Concept:**
A comprehensive UE5 master base project that combines proven indie game development patterns with modern AI-assisted development workflows. The solution leverages the NEON plugin to bridge HTML/JavaScript UI development with Unreal Engine, enabling rapid menu creation using shadcn components and AI code generation.

**Key Differentiators:**
- **AI-Optimized Architecture:** Code structure designed specifically for AI assistants to understand and extend
- **NEON+shadcn Integration:** HTML/JS menus within UE5 widgets for rapid, beautiful UI development
- **Lean Foundation Approach:** Only essential systems included, avoiding over-engineering
- **Multi-Platform Ready:** Built-in Steam Deck compatibility and multi-controller support
- **Claude-Friendly Documentation:** Locally stored documentation to minimize token usage

**Success Factors:**
- Human-readable code that remains AI-parseable
- Modular component architecture for maximum reusability
- Template-driven development for consistent quality
- Foundation-first implementation focusing on infrastructure before features

## Target Users

### Primary User Segment: Indie Game Developers (Internal)
**Profile:** Small development studio (1-2 developers) with proven track record in Steam PC game development
**Current Workflow:** Manual recreation of core systems for each project, custom UI development, individual project management
**Pain Points:** 
- Time spent on repetitive foundational development
- Inconsistent code quality and structure
- Difficulty leveraging AI development tools effectively
**Goals:** 
- Accelerate prototype-to-release timelines
- Improve code quality and maintainability
- Maximize development efficiency with limited resources
- Scale successful game development process

### Secondary User Segment: AI Development Tools
**Profile:** Claude Code, BMAD method agents, and other AI assistants
**Current Interaction:** Limited by inconsistent code structure and lacking documentation
**Requirements:**
- Standardized naming conventions and file organization
- Comprehensive, locally-accessible documentation
- Clear template patterns for extension
**Goals:**
- Generate accurate, functional code suggestions
- Understand project context quickly
- Extend existing systems without breaking patterns

## Goals & Success Metrics

### Business Objectives
- **Reduce prototype development time by 50%** within 3 months of base project completion
- **Improve code consistency score to 90%+** using standardized conventions and templates
- **Enable AI-assisted development for 80% of UI/menu tasks** through NEON+shadcn integration
- **Achieve 100% base project reusability** across new game concepts

### User Success Metrics
- **Setup time for new game project: <2 hours** from base project to functional prototype
- **Menu creation time: <1 day** for complete game menu system using AI assistance
- **Documentation lookup time: <30 seconds** for any component or system reference
- **Cross-platform compatibility: 95%** success rate for Steam Deck without additional work

### Key Performance Indicators (KPIs)
- **Lines of code reused:** >80% of foundational code per new project
- **Development velocity:** 2x faster feature implementation compared to current process
- **Code quality:** Zero critical bugs in base systems across all derived projects
- **AI assistance effectiveness:** >70% of AI-generated code accepted without modification

## MVP Scope

### Core Features (Must Have)
- **AI-Readable Code Structure:** Standardized C++ naming conventions, file organization, and comprehensive documentation
- **NEON Plugin Integration:** Complete setup with shadcn UI components integrated into UE5 widgets
- **Essential Menu Systems:** Main menu, settings (game/audio/graphics/keybinds/language), escape menu, basic HUD, developer menu
- **Basic FPS Character Controller:** First-person movement, crouch, jump, aim, interact (E key) functionality
- **Settings Management:** Graphics (resolution, fullscreen, VSync, FPS cap), audio (volume controls), game settings with persistence
- **Multi-Controller Input Framework:** Keyboard/mouse, gamepad, Steam Deck support with context switching
- **Development Tools:** Time controls, performance metrics display, collision visualization, function trigger menu
- **Documentation Integration:** Local shadcn, UE5 C++, and component documentation

### Out of Scope for MVP
- Game-specific save/load systems (handled per project)
- Advanced lighting systems (game-dependent)
- Complex post-processing effects beyond basics
- Ray tracing or hyper-realistic features
- Multi-language content translation (framework only)
- Advanced AI NPCs or game logic
- Multiplayer networking
- Advanced audio mixing beyond basic volume controls

### MVP Success Criteria
A functional UE5 project that can be copied, renamed, and have a basic game prototype running with complete menu system and character controller within 2 hours, with all code following AI-readable conventions and complete documentation available locally.

## Post-MVP Vision

### Phase 2 Features
- **Advanced Localization System:** Complete translation framework with component-level translation exclusions
- **Enhanced Development Tools:** Additional debugging utilities, automated testing frameworks, performance profiling tools
- **Template Library Expansion:** Pre-built templates for common game mechanics and UI patterns
- **Asset Pipeline Integration:** Standardized workflows for importing and managing low-poly/retro art assets
- **Steam Integration:** Achievement system, cloud saves, workshop support templates

### Long-term Vision
Transform into the definitive foundation for AI-assisted indie game development, potentially becoming a template that other small studios can adopt. Enable rapid exploration of game concepts with professional-quality implementation from day one.

### Expansion Opportunities
- **Community Template Sharing:** Platform for sharing game-specific extensions and templates
- **AI Agent Specialization:** Custom AI agents trained specifically on this base project structure
- **Educational Content:** Tutorials and courses teaching AI-assisted game development using this foundation

## Technical Considerations

### Platform Requirements
- **Target Platforms:** Steam PC (primary), Steam Deck (secondary)
- **Engine Version:** Unreal Engine 5.5.4 (specific version for consistency)
- **Performance Requirements:** Optimized for low-poly/retro graphics, stable 60+ FPS on mid-range hardware

### Technology Preferences
- **Frontend:** NEON plugin + shadcn UI components + HTML/JavaScript
- **Backend:** Unreal Engine 5 C++ with Blueprint integration
- **UI Framework:** shadcn component library with custom NEON integration
- **Development Tools:** Claude Code CLI, BMAD method agents

### Architecture Considerations
- **Repository Structure:** Clear separation of base systems vs. game-specific code
- **Component Architecture:** Modular design enabling easy extension and modification
- **Documentation System:** Local file-based documentation accessible to AI and humans
- **Naming Conventions:** Human-readable yet concise, AI-parseable patterns

## Constraints & Assumptions

### Constraints
- **Budget:** Minimal - must use free/open-source solutions and existing NEON plugin investment
- **Timeline:** Foundation must be completed within 4-6 weeks for immediate project needs
- **Resources:** 1-2 developers with existing UE5 and C++ experience
- **Technical:** Must maintain compatibility with existing successful game projects' patterns

### Key Assumptions
- NEON plugin will continue to be maintained and compatible with UE5.5.4
- shadcn UI components can be effectively integrated within NEON widgets
- AI development tools (Claude Code) will continue improving and remain accessible
- Steam platform will remain primary target for indie game distribution
- Low-poly/retro art style preference will continue across future projects

## Risks & Open Questions

### Key Risks
- **NEON Plugin Compatibility:** Plugin may have undiscovered limitations or conflicts with UE5.5.4
- **AI Integration Complexity:** Time investment in AI-readable structure may not yield expected productivity gains
- **Over-Engineering Risk:** Adding too many systems could make the base project complex rather than helpful
- **Documentation Maintenance:** Local documentation may become outdated as tools evolve

### Open Questions
- How much time will NEON+shadcn integration actually require compared to traditional UE5 UI development?
- What's the optimal balance between pre-built systems and customization flexibility?
- How should the base project be versioned and updated across multiple derived game projects?
- What template formats will provide maximum value for AI-assisted development?

### Areas Needing Further Research
- NEON plugin limitations and best practices for complex UI interactions
- shadcn component performance within UE5 widget context
- Optimal file structure for AI code comprehension and extension
- Steam Deck specific input and performance considerations

## Appendices

### A. Research Summary
**Brainstorming Session Results (2025-10-25):**
- Comprehensive analysis of 35+ features across 9 system categories
- Morphological analysis revealing optimal foundation-first development approach
- Clear prioritization: Documentation/structure → NEON integration → Character controller
- Identified key themes: AI optimization, modular architecture, multi-platform support

### B. References
- [Brainstorming Session Results](docs/brainstorming-session-results.md)
- NEON Plugin Documentation (to be downloaded locally)
- shadcn UI Component Library (to be downloaded locally)
- UE5.5.4 C++ API Documentation (to be downloaded locally)

## Next Steps

### Immediate Actions
1. **Establish code structure and documentation standards** (Timeline: 1-2 days)
2. **Download and organize local documentation** for shadcn, NEON, and UE5 C++ (Timeline: 2-3 days)
3. **Implement basic FPS character controller** with movement, crouch, jump, aim, interact (Timeline: 3-4 days)
4. **Set up NEON plugin integration** with initial shadcn component testing (Timeline: 2-3 weeks)
5. **Create template system** for rapid class and component creation (Timeline: 1 week)

### PM Handoff
This Project Brief provides the full context for NohamBaseProject. Please start in 'PRD Generation Mode', review the brief thoroughly to work with the user to create the PRD section by section as the template indicates, asking for any necessary clarification or suggesting improvements.