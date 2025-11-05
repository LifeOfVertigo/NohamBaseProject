# Epic 5: Cleanup Checklist - Avoid Creeping Crud

**Purpose:** Systematic removal of incomplete/incorrect implementation before proper rebuild

---

## 🗑️ FILES TO DELETE

### Incomplete NEON UI Files (Vanilla CSS/JS - Not shadcn)
```bash
# These will be completely replaced with shadcn versions
rm Noham_Base_Proj_Cpp/Content/UI/NEON/Developer/debug-overlay.html
rm Noham_Base_Proj_Cpp/Content/UI/NEON/Styles/debug-overlay.css
rm Noham_Base_Proj_Cpp/Content/UI/NEON/Components/Game/debug-overlay.js
```

**Reason:** Use vanilla HTML/CSS instead of required shadcn + Tailwind architecture

---

## 🔄 FILES TO RENAME (Naming Convention Fixes)

### Fix Class Name Violations

**Character Files:**
```bash
# Current (WRONG): AUNohamBaseCharacter (double prefix violation)
# Target (CORRECT): ANohamCharacter

mv Noham_Base_Proj_Cpp/Source/Noham_Base_Proj_Cpp/Public/Components/Characters/UNohamBaseCharacter.h \
   Noham_Base_Proj_Cpp/Source/Noham_Base_Proj_Cpp/Public/Characters/NohamCharacter.h

mv Noham_Base_Proj_Cpp/Source/Noham_Base_Proj_Cpp/Private/Components/Characters/UNohamBaseCharacter.cpp \
   Noham_Base_Proj_Cpp/Source/Noham_Base_Proj_Cpp/Private/Characters/NohamCharacter.cpp
```

**Class Name Changes Required:**
- `class AUNohamBaseCharacter` → `class NOHAM_BASE_PROJ_CPP_API ANohamCharacter`
- Update all references in `.uasset` files (Blueprints)

**Game Mode Files:**
```bash
# Current (WRONG): AUNohamBaseGameMode (double prefix violation)
# Target (CORRECT): ANohamGameMode

mv Noham_Base_Proj_Cpp/Source/Noham_Base_Proj_Cpp/Public/Subsystems/Core/UNohamBaseGameMode.h \
   Noham_Base_Proj_Cpp/Source/Noham_Base_Proj_Cpp/Public/Core/NohamGameMode.h

mv Noham_Base_Proj_Cpp/Source/Noham_Base_Proj_Cpp/Private/Subsystems/Core/UNohamBaseGameMode.cpp \
   Noham_Base_Proj_Cpp/Source/Noham_Base_Proj_Cpp/Private/Core/NohamGameMode.cpp
```

**Class Name Changes Required:**
- `class AUNohamBaseGameMode` → `class NOHAM_BASE_PROJ_CPP_API ANohamGameMode`
- Update `DefaultGameMode` in Project Settings
- Update all Blueprint references

---

## 🧹 DEPRECATED UE5 TEMPLATE FILES

### Legacy Template Files (May Need Deletion)

**Evaluate these for deletion:**
```bash
# Check if anything references these first!
# If not referenced, DELETE:

Noham_Base_Proj_Cpp/Source/Noham_Base_Proj_Cpp/Noham_Base_Proj_CppCharacter.h
Noham_Base_Proj_Cpp/Source/Noham_Base_Proj_Cpp/Noham_Base_Proj_CppCharacter.cpp
Noham_Base_Proj_Cpp/Source/Noham_Base_Proj_Cpp/Noham_Base_Proj_CppGameMode.h
Noham_Base_Proj_Cpp/Source/Noham_Base_Proj_Cpp/Noham_Base_Proj_CppGameMode.cpp
```

**Validation Before Deletion:**
```bash
# Search for references
grep -r "Noham_Base_Proj_CppCharacter" Noham_Base_Proj_Cpp/Source/
grep -r "Noham_Base_Proj_CppGameMode" Noham_Base_Proj_Cpp/Source/

# Check Blueprint references (manual inspection required)
# Look in Content/FirstPerson/Blueprints/BP_FirstPersonCharacter.uasset
# Look in Content/FirstPerson/Blueprints/BP_FirstPersonGameMode.uasset
```

**If No References Found:** DELETE
**If References Found:** Update references to new classes first, then DELETE

---

## 📁 DIRECTORY STRUCTURE CLEANUP

### Remove Empty/Redundant Directories

**After file moves, check for empty directories:**
```bash
# These may become empty after renames
Noham_Base_Proj_Cpp/Source/Noham_Base_Proj_Cpp/Public/Components/Characters/
Noham_Base_Proj_Cpp/Source/Noham_Base_Proj_Cpp/Private/Components/Characters/
Noham_Base_Proj_Cpp/Source/Noham_Base_Proj_Cpp/Public/Subsystems/Core/
Noham_Base_Proj_Cpp/Source/Noham_Base_Proj_Cpp/Private/Subsystems/Core/
```

**Action:** If empty, delete. If not empty, investigate what remains.

### Create Missing Standard Directories

**Create these if they don't exist:**
```bash
mkdir -p Noham_Base_Proj_Cpp/Source/Noham_Base_Proj_Cpp/Public/Characters
mkdir -p Noham_Base_Proj_Cpp/Source/Noham_Base_Proj_Cpp/Private/Characters
mkdir -p Noham_Base_Proj_Cpp/Source/Noham_Base_Proj_Cpp/Public/Core
mkdir -p Noham_Base_Proj_Cpp/Source/Noham_Base_Proj_Cpp/Private/Core
mkdir -p Noham_Base_Proj_Cpp/Source/Noham_Base_Proj_Cpp/Public/Subsystems/Developer
mkdir -p Noham_Base_Proj_Cpp/Source/Noham_Base_Proj_Cpp/Private/Subsystems/Developer
```

---

## 🔍 BLUEPRINT ASSET CLEANUP

### Update Blueprint Parent Classes

**Blueprints Affected:**
1. `Content/FirstPerson/Blueprints/BP_FirstPersonCharacter.uasset`
   - Old Parent: `Noham_Base_Proj_CppCharacter` or `UNohamBaseCharacter`
   - New Parent: `NohamCharacter`

2. `Content/FirstPerson/Blueprints/BP_FirstPersonGameMode.uasset`
   - Old Parent: `Noham_Base_Proj_CppGameMode` or `UNohamBaseGameMode`
   - New Parent: `NohamGameMode`

**Action Required:**
1. Open each Blueprint in UE5 Editor
2. File → Reparent Blueprint
3. Select new parent class
4. Save
5. Fix any broken references

### Check for Broken References

**After cleanup, validate:**
```
1. Open UE5 Editor
2. Window → Developer Tools → Reference Viewer
3. Check each renamed class for red broken references
4. Fix manually or delete orphaned assets
```

---

## 📝 CODE REFERENCE UPDATES

### Files That Reference Renamed Classes

**Search and Replace Required In:**
```bash
# Find all references to old class names
grep -r "UNohamBaseCharacter" Noham_Base_Proj_Cpp/Source/
grep -r "UNohamBaseGameMode" Noham_Base_Proj_Cpp/Source/
grep -r "Noham_Base_Proj_CppCharacter" Noham_Base_Proj_Cpp/Source/
grep -r "Noham_Base_Proj_CppGameMode" Noham_Base_Proj_Cpp/Source/
```

**Update #include statements:**
```cpp
// OLD
#include "Components/Characters/UNohamBaseCharacter.h"
// NEW
#include "Characters/NohamCharacter.h"

// OLD
#include "Subsystems/Core/UNohamBaseGameMode.h"
// NEW
#include "Core/NohamGameMode.h"
```

---

## ⚙️ PROJECT CONFIGURATION UPDATES

### Update Project Settings

**File:** `Noham_Base_Proj_Cpp/Config/DefaultEngine.ini`

**Find and update:**
```ini
[/Script/EngineSettings.GameMapsSettings]
GlobalDefaultGameMode=/Script/Noham_Base_Proj_Cpp.Noham_Base_Proj_CppGameMode
```

**Change to:**
```ini
[/Script/EngineSettings.GameMapsSettings]
GlobalDefaultGameMode=/Script/Noham_Base_Proj_Cpp.NohamGameMode
```

**Or use Blueprint:**
```ini
GlobalDefaultGameMode=/Game/FirstPerson/Blueprints/BP_FirstPersonGameMode.BP_FirstPersonGameMode_C
```

---

## 🧪 VALIDATION AFTER CLEANUP

### Checklist Before Proceeding

**Complete these validations:**

- [ ] All old files deleted
- [ ] All files renamed correctly
- [ ] No naming convention violations remain
- [ ] Directory structure follows source tree spec
- [ ] All #include statements updated
- [ ] No broken Blueprint references
- [ ] Project compiles successfully
- [ ] No orphaned .uasset files
- [ ] Git status clean (old files tracked, new files added)
- [ ] DefaultGameMode configuration updated

### Compilation Test

```bash
# Regenerate project files
"C:\Program Files\Epic Games\UE_5.5\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" -projectfiles -project="Noham_Base_Proj_Cpp.uproject" -game -rocket -progress

# Build project
# Open in UE5 Editor and verify no errors
```

### Runtime Test

- [ ] Launch PIE (Play in Editor)
- [ ] Character spawns and moves correctly
- [ ] No error messages in Output Log
- [ ] No broken asset references in Content Browser

---

## 📋 GIT WORKFLOW

### Commit Strategy

**Branch naming:**
```bash
git checkout -b feature/epic-5-cleanup-and-implementation
```

**Commit sequence:**
```bash
# Commit 1: Delete incomplete files
git rm Noham_Base_Proj_Cpp/Content/UI/NEON/Developer/debug-overlay.html
git rm Noham_Base_Proj_Cpp/Content/UI/NEON/Styles/debug-overlay.css
git rm Noham_Base_Proj_Cpp/Content/UI/NEON/Components/Game/debug-overlay.js
git commit -m "cleanup: Remove incomplete vanilla NEON UI files for Story 5.1"

# Commit 2: Rename and fix naming conventions
git mv <old> <new>
# ... update code references ...
git commit -m "refactor: Fix naming conventions (AUNoham* → ANoham*)"

# Commit 3: Delete deprecated template files (if applicable)
git rm <deprecated-files>
git commit -m "cleanup: Remove deprecated UE5 template files"

# Commit 4: Update project configuration
git add Config/DefaultEngine.ini
git commit -m "config: Update DefaultGameMode to use renamed classes"
```

---

## 🚨 ROLLBACK PLAN

### If Cleanup Breaks Something

**Backup before starting:**
```bash
git stash
git checkout -b backup-before-cleanup
git stash pop
git commit -m "Backup before Epic 5 cleanup"
git checkout main
```

**Rollback if needed:**
```bash
git checkout backup-before-cleanup
```

**Incremental approach:**
- Do one phase at a time
- Test compilation after each phase
- Commit after each successful phase
- Easy to identify which change broke things

---

## ✅ CLEANUP COMPLETE CRITERIA

**Sign-off checklist:**

- [x] All files deleted as planned
- [x] All files renamed as planned
- [x] All code references updated
- [x] All Blueprint references fixed
- [x] Project compiles without errors
- [x] PIE launches without errors
- [x] No naming convention violations
- [x] Directory structure matches spec
- [x] Git history clean and well-documented
- [x] Ready to proceed with implementation

**Once complete, proceed to implementation plan Phase 2 (shadcn setup).**

---

**This checklist ensures zero "creeping crud" remains before beginning the proper implementation.**
