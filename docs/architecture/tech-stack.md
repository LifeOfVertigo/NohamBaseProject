# Tech Stack

This is the **DEFINITIVE** technology selection for the entire project. All development must use these exact versions and tools.

### Technology Stack Table

| Category | Technology | Version | Purpose | Rationale |
|----------|------------|---------|---------|-----------|
| **Game Engine** | Unreal Engine | 5.5.4 | Core game runtime and development environment | Specific version for consistency across projects, proven stability |
| **Frontend Language** | HTML5/CSS3/JavaScript | ES2022 | UI development within NEON widgets | Modern web standards with broad browser compatibility |
| **Frontend Framework** | Vanilla JS + NEON | NEON May 28, 2025 | Web UI rendering within UE5 widgets | Direct control over performance, minimal overhead. **CRITICAL: NEON is ENGINE-LEVEL plugin, NOT project-level. Do not disable in .uproject** |
| **UI Component Library** | shadcn/ui | Latest (2025) | Professional UI component system | Production-ready components, excellent theming, accessibility built-in. **CRITICAL: Use shadcn MCP tools only - components live in UE5 Content folder** |
| **CSS Framework** | Tailwind CSS | 3.x | Utility-first styling for shadcn components | Required for shadcn, rapid styling, consistent design tokens |
| **Animation Library** | Framer Motion | 11.x | UI animations and micro-interactions | Performance optimized, declarative animations, accessibility support |
| **State Management** | Vanilla JS + UE5 Bridge | Custom | Game state synchronization with UI | Minimal overhead, direct UE5 integration, no unnecessary abstraction |
| **Backend Language** | C++ | C++20 | Native game systems and performance-critical code | UE5 standard, maximum performance, full engine feature access |
| **Backend Framework** | UE5 Framework | 5.5.4 | Game systems, subsystems, and Blueprint integration | Native UE5 patterns, proven architecture for game development |

### 📚 UE5 C++ Development Resources

**Core References:**
- [UE5 Documentation Index](../unreal-engine/README.md) - Complete local documentation cache
- [Essential Macros](../unreal-engine/quick-reference/macros.md) - UCLASS, UPROPERTY, UFUNCTION reference
- [Common Patterns](../unreal-engine/quick-reference/common-patterns.md) - Frequently used UE5 C++ patterns

**System-Specific Guides:**
- [Actors & Components](../unreal-engine/core-systems/actors.md) - Core UE5 object system
- [Module System](../unreal-engine/core-systems/modules.md) - Project organization and .Build.cs files
- [Input Handling](../unreal-engine/gameplay/input.md) - Enhanced Input system implementation
- [Networking](../unreal-engine/gameplay/networking.md) - Replication and multiplayer patterns
- [Containers & Types](../unreal-engine/core-systems/containers.md) - TArray, TMap, FString usage
| **API Style** | UE5 Blueprint/C++ Bridge | Native | Communication between game logic and UI | Direct function calls, type safety, minimal serialization overhead |
| **Database** | UE5 Config System | Native | Settings persistence and game state storage | Built-in persistence, Steam Cloud integration, no external dependencies |
| **Cache** | UE5 Asset Cache | Native | Asset loading and texture caching | Optimized for game assets, automatic memory management |
| **File Storage** | UE5 Content Browser | Native | Game assets, HTML/CSS/JS files as content | Integrated with UE5 packaging, version control friendly |
| **Authentication** | Steam API | Latest | Player identification and platform features | Required for Steam deployment, handles user accounts |
| **Frontend Testing** | Jest + Testing Library | Latest | HTML/CSS/JS component testing | Industry standard, good shadcn component support |
| **Backend Testing** | UE5 Automation Framework | Native | C++ and Blueprint testing | Integrated testing, game-specific test patterns |
| **E2E Testing** | Manual + UE5 PIE | Native | Full game flow testing | Play-in-Editor for rapid iteration, manual for user experience |
| **Build Tool** | Unreal Build Tool (UBT) | 5.5.4 | C++ compilation and project building | UE5 standard, handles complex dependencies |
| **Bundler** | UE5 Packaging | Native | Final game executable creation | Optimized game packaging, platform-specific builds |
| **Web Asset Bundler** | Vite | 5.x | HTML/CSS/JS bundling for NEON | Fast development builds, efficient production bundles |
| **IaC Tool** | N/A | N/A | Not applicable for desktop game deployment | Steam handles distribution infrastructure |
| **CI/CD** | GitHub Actions | Latest | Automated building and testing | Free for public repos, good UE5 community support |
| **Monitoring** | UE5 Stats System | Native | Runtime performance monitoring | Built-in profiling, game-specific metrics |
| **Logging** | UE5 Logging | Native | Debug and runtime logging | Categorized logging, configurable verbosity |
| **Icon Library** | Lucide Icons | Latest | SVG icons for UI components | shadcn standard, comprehensive game-appropriate icons |
| **Input Handling** | Steam Input + UE5 | Native | Multi-controller and platform input | Steam Deck compatibility, controller auto-detection |
