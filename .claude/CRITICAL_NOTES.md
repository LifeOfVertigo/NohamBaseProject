# CRITICAL PROJECT NOTES FOR AI AGENTS

## 🚨 DO NOT MODIFY THESE ITEMS

### NEON Plugin Configuration
**CRITICAL:** The NEON plugin is an **ENGINE-LEVEL** plugin, NOT a project-level plugin.

- **Location:** Installed at UE5 engine level (not in project Plugins/ folder)
- **Project File:** MUST remain enabled in `Noham_Base_Proj_Cpp.uproject`
- **DO NOT:** Disable, remove, or mark as "Optional" in the .uproject file
- **DO NOT:** Attempt to "fix" missing plugin errors by disabling NEON
- **Reason:** NEON is properly installed at engine level and required for UI functionality

**If you see "plugin not found" errors:** This is likely a build/path issue, NOT a plugin configuration issue. Do not modify the .uproject file.

### Project Build Requirements
- The project MUST be recompiled after adding new C++ files
- Use UE5's automatic rebuild feature (click "Yes" when prompted)
- Alternative: Build via Visual Studio in Development Editor configuration
- **DO NOT:** Attempt to "fix" build errors by modifying plugin configurations

## ✅ SAFE TO MODIFY
- C++ source files in Source/Noham_Base_Proj_Cpp/
- Documentation files in docs/
- Configuration files in Config/ (with caution)

## ❌ NEVER MODIFY WITHOUT EXPLICIT USER PERMISSION
- `.uproject` file plugin section
- Engine-level configurations
- Core module structure (without user approval)
