# NohamBaseProject Coding Standards

## Critical Fullstack Rules

Define MINIMAL but CRITICAL standards for AI agents. Focus only on project-specific rules that prevent common mistakes. These will be used by dev agents.

- **Type Sharing:** Always define types in packages/shared and import from there
- **API Calls:** Never make direct HTTP calls - use the service layer
- **Environment Variables:** Access only through config objects, never process.env directly
- **Error Handling:** All API routes must use the standard error handler
- **State Updates:** Never mutate state directly - use proper state management patterns
- **NEON Communication:** All UE5 ↔ JavaScript communication must go through NEONBridgeSubsystem
- **Settings Validation:** Always validate settings changes before applying to UE5 systems
- **Steam Integration:** Use ISteamPlatformInterface abstraction, never direct plugin calls
- **Asset References:** HTML/CSS/JS files must be treated as UE5 content assets
- **shadcn Components:** ALWAYS use shadcn MCP tools to add components to `Content/UI/NEON/Components/shadcn/` - NEVER manual installation
- **Performance Budgets:** NEON widgets limited to 3.5ms processing time per frame
- **Memory Management:** Use UE5 smart pointers (TSharedPtr, TWeakPtr) for all subsystem references
- **Input Handling:** All input detection must route through InputManagerSubsystem
- **Debug Code:** Developer tools code must be conditionally compiled (#if WITH_EDITOR)

## Naming Conventions

### UE5 C++ Type Prefixes

All UE5 C++ classes follow standard Unreal Engine prefix conventions combined with project prefix:

| UE5 Prefix | Type | Project Naming | Example |
|------------|------|----------------|---------|
| **A** | Actor-derived | `ANoham[ClassName]` | `ANohamCharacter`, `ANohamGameMode` |
| **U** | UObject-derived | `UNoham[ClassName]` | `UNohamSettingsSubsystem`, `UNohamComponent` |
| **F** | Plain structs/classes | `FNoham[StructName]` | `FNohamSettingsData`, `FNohamInputConfig` |
| **I** | Interface classes | `INoham[InterfaceName]` | `INohamPlatformInterface` |
| **E** | Enums | `ENoham[EnumName]` | `ENohamInputMode`, `ENohamQualityLevel` |
| **T** | Template classes | `TNoham[TemplateName]` | (Rare - use UE5 built-in templates) |

**CRITICAL:** Always use correct UE5 type prefix + `Noham` project prefix for all C++ classes.

### Full Naming Table

| Element | Frontend | Backend | Example |
|---------|----------|---------|---------|
| Components | PascalCase | - | `UserProfile.tsx` |
| Hooks | camelCase with 'use' | - | `useAuth.ts` |
| API Routes | - | kebab-case | `/api/user-profile` |
| Database Tables | - | snake_case | `user_profiles` |
| UE5 Actors | - | A + PascalCase | `ANohamCharacter`, `ANohamGameMode` |
| UE5 UObjects | - | U + PascalCase | `UNohamSettingsSubsystem` |
| UE5 Structs | - | F + PascalCase | `FNohamSettingsData` |
| UE5 Interfaces | - | I + PascalCase | `INohamPlatformInterface` |
| UE5 Enums | - | E + PascalCase | `ENohamInputMode` |
| UE5 Functions | - | PascalCase | `UpdateGraphicsSettings` |
| UE5 Variables | - | PascalCase with type prefix | `bIsDebugModeEnabled`, `CurrentHealth` |
| NEON Events | camelCase | - | `settingsChanged` |
| NEON Functions | camelCase | - | `updateGraphicsSettings` |
| JavaScript Files | kebab-case | - | `game-state-manager.js` |
| CSS Classes | kebab-case | - | `settings-menu-container` |
| shadcn Components | PascalCase | - | `GameButton`, `SettingsCard` |

## UE5-Specific Standards

### C++ Code Organization
- All game-specific classes use `UNoham` or `FNoham` prefix
- Subsystems inherit from appropriate UE5 base classes (`UEngineSubsystem`, `UGameInstanceSubsystem`)
- Use `UPROPERTY()` macros for Blueprint integration
- Mark functions as `BlueprintCallable` only when necessary for NEON bridge
- Use `const` references for large data structures
- Prefer `TArray` over `std::vector` for UE5 integration

**📚 UE5 C++ Reference:**
- [Essential Macros Guide](../unreal-engine/quick-reference/macros.md) - Complete UCLASS, UPROPERTY, UFUNCTION reference
- [Actors System](../unreal-engine/core-systems/actors.md) - Actor creation, lifecycle, spawning patterns
- [Components](../unreal-engine/core-systems/components.md) - Component creation and management
- [UObject System](../unreal-engine/core-systems/objects.md) - Reflection system and best practices
- [Common Patterns](../unreal-engine/quick-reference/common-patterns.md) - Frequently used UE5 patterns

### NEON Integration Standards
- All NEON events use consistent JSON structure with `requestId`, `timestamp`, `data`
- Error responses must include `error.code`, `error.message`, `error.details`
- Batch HUD updates for performance (maximum 60fps update rate)
- Use TypeScript interfaces for all NEON communication data structures
- Validate all incoming data from JavaScript before processing in C++

### shadcn Component Management Standards
- **CRITICAL:** ALWAYS use shadcn MCP tools for component management
- Component location: `Content/UI/NEON/Components/shadcn/`
- Search for components: `mcp__shadcn__search_items_in_registries`
- View component details: `mcp__shadcn__view_items_in_registries`
- Add new components: Use MCP-provided add commands
- NEVER use `npx shadcn@latest add` - this bypasses UE5 Content folder integration

### Steam Platform Standards
- Never call Steam plugin directly - always use `ISteamPlatformInterface`
- Handle offline scenarios gracefully (Steam not running, no internet)
- Cache Steam user data locally to reduce API calls
- Use Steam App ID consistently across all platform interactions
- Implement fallback behavior for non-Steam builds

### Performance Standards
- NEON widget frame budget: 3.5ms maximum processing time
- Settings changes must be applied within 100ms
- UI animations should run at 60fps minimum
- Batch multiple UI updates within single frame when possible
- Use object pooling for frequently created/destroyed objects

### Error Handling Standards
- All subsystem operations return success/failure status
- Log errors with appropriate verbosity levels (`UE_LOG`)
- Provide user-friendly error messages through NEON bridge
- Implement retry logic for network-dependent operations
- Validate all user input before processing

### Security Standards
- Never expose internal C++ implementation details to JavaScript
- Validate all NEON function parameters before processing
- Use UE5 config system encryption for sensitive settings
- Sanitize console command inputs in developer tools
- Implement rate limiting for NEON function calls

---

These standards ensure consistent, maintainable code across the hybrid UE5 + NEON + shadcn architecture while optimizing for AI-assisted development workflows.