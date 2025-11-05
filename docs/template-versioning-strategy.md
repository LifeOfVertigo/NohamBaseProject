# Base Template Versioning Strategy

## Overview

Simple versioning system for managing the NohamBaseProject base template across multiple derived game projects, ensuring updates can be applied safely without breaking existing projects.

## Version Format

**Semantic Versioning:** `MAJOR.MINOR.PATCH`

- **MAJOR:** Breaking changes that require manual migration (e.g., 1.0.0 → 2.0.0)
- **MINOR:** New features that are backward compatible (e.g., 1.0.0 → 1.1.0)  
- **PATCH:** Bug fixes and small improvements (e.g., 1.0.0 → 1.0.1)

## Version Identification

### In Code
```cpp
// NohamBaseProject.h
#define NOHAM_BASE_VERSION_MAJOR 1
#define NOHAM_BASE_VERSION_MINOR 0
#define NOHAM_BASE_VERSION_PATCH 0
#define NOHAM_BASE_VERSION_STRING "1.0.0"
```

### In Documentation
```markdown
<!-- docs/VERSION.md -->
# NohamBaseProject v1.0.0

**Release Date:** 2025-11-15
**Compatibility:** UE5 5.5.4
**Breaking Changes:** None (initial release)
```

### In Project Files
```ini
; Noham_Base_Proj_Cpp.uproject
"EngineAssociation": "5.5.4",
"NohamBaseVersion": "1.0.0"
```

## Update Types & Procedures

### PATCH Updates (1.0.0 → 1.0.1)
**Safe for all projects - Apply immediately**

**Examples:**
- Bug fixes in existing systems
- Performance optimizations
- Documentation corrections
- Small UI improvements

**Update Process:**
1. Copy updated files to derived project
2. Rebuild project
3. Test basic functionality
4. Update version identifier

### MINOR Updates (1.0.0 → 1.1.0)
**Safe for all projects - Optional adoption**

**Examples:**
- New optional subsystems
- Additional debug tools
- New component templates
- Enhanced documentation

**Update Process:**
1. Review changelog for new features
2. Copy desired new files to derived project
3. Update version identifier
4. Test new features if adopted
5. Update project documentation

### MAJOR Updates (1.0.0 → 2.0.0)
**Breaking changes - Manual migration required**

**Examples:**
- UE5 engine version upgrade
- NEON plugin major version change
- Core architecture refactoring
- Breaking API changes

**Update Process:**
1. **DO NOT** auto-update existing projects
2. Create migration guide
3. Test migration on copy of project
4. Plan migration timeline
5. Execute migration with full testing

## File Organization

### Base Template Repository
```
NohamBaseProject/
├── VERSION.md                     # Current version info
├── CHANGELOG.md                   # Version history
├── docs/
│   ├── migration/                 # Migration guides
│   │   ├── v1.0-to-v1.1.md
│   │   └── v1.1-to-v2.0.md
│   └── versioning/                # Versioning docs
├── tools/
│   └── update-project.py          # Update automation script
└── [standard project structure]
```

### Derived Project Tracking
```
GameProject1/
├── .noham-base-version            # Contains: "1.0.0"
├── NOHAM-MODIFICATIONS.md         # Track customizations
└── [game-specific files]
```

## Update Automation

### Simple Update Script
```python
# tools/update-project.py
def update_project(target_project_path, update_type):
    current_version = read_project_version(target_project_path)
    available_version = read_base_template_version()
    
    if update_type == "patch":
        copy_safe_files(target_project_path)
        update_version_file(target_project_path, available_version)
        print(f"Updated {target_project_path} to {available_version}")
    
    elif update_type == "minor":
        show_available_features()
        prompt_user_selection()
        copy_selected_files(target_project_path)
        update_version_file(target_project_path, available_version)
    
    elif update_type == "major":
        print("MAJOR update requires manual migration!")
        show_migration_guide()
```

### Usage Examples
```bash
# Safe patch update
python tools/update-project.py ../MyGame1 patch

# Optional minor update
python tools/update-project.py ../MyGame1 minor

# Major update (manual process)
python tools/update-project.py ../MyGame1 major
```

## Version Compatibility Matrix

| Base Template | UE5 Version | NEON Plugin | shadcn | Compatible Projects |
|---------------|-------------|-------------|---------|-------------------|
| 1.0.x         | 5.5.4       | 1.0.x      | 2025.x  | All 1.x projects  |
| 1.1.x         | 5.5.4       | 1.0.x      | 2025.x  | All 1.x projects  |
| 2.0.x         | 5.6.x       | 2.0.x      | 2026.x  | 2.x projects only |

## Best Practices

### For Base Template Development
1. **Always increment version** when making any changes
2. **Document breaking changes** thoroughly in CHANGELOG.md
3. **Test updates** on sample derived project before release
4. **Provide migration guides** for major updates
5. **Maintain backward compatibility** within major versions

### For Derived Projects
1. **Track base template version** in `.noham-base-version` file
2. **Document customizations** in `NOHAM-MODIFICATIONS.md`
3. **Test updates on backup** before applying to main project
4. **Review changelogs** before applying any updates
5. **Plan major updates** during appropriate development phases

### For Team Coordination
1. **Coordinate base template updates** across team
2. **Use patch updates freely** for bug fixes
3. **Plan minor updates** during sprint planning
4. **Schedule major updates** during project milestones
5. **Document update decisions** in project notes

## Migration Support

### Breaking Change Documentation
```markdown
# Migration Guide: v1.5 to v2.0

## Breaking Changes
1. **NEON Plugin Update:** Requires NEON v2.0
2. **Settings API Change:** Old API removed

## Migration Steps
1. Update NEON plugin to v2.0
2. Replace old settings calls with new API
3. Update project dependencies
4. Test all functionality

## Compatibility Issues
- Old NEON widgets need manual conversion
- Settings files require migration script
```

### Rollback Strategy
```bash
# Simple rollback process
git checkout tags/v1.4.2  # Roll back to known good version
python tools/update-project.py ../MyGame1 patch
```

This simple versioning strategy ensures base template evolution doesn't break existing projects while enabling easy adoption of improvements and new features.

<function_calls>
<invoke name="TodoWrite">
<parameter name="todos">[{"id": "create-epic-story-breakdown", "content": "Create comprehensive epic/story breakdown from PRD", "status": "completed"}, {"id": "design-template-versioning", "content": "Design simple base template versioning strategy", "status": "completed"}]