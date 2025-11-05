# Epic 5: Blueprint Setup Guide

**Purpose:** Guide for creating Blueprints that parent to the new clean C++ classes

---

## 🎯 C++ Classes Ready for Blueprint Parenting

After the cleanup, these are the correct C++ classes to use:

### Character Class
- **C++ Class:** `ANohamCharacter`
- **Header Location:** `Source/Noham_Base_Proj_Cpp/Public/Characters/NohamCharacter.h`
- **When creating BP:** Search for "NohamCharacter" in class picker

### Game Mode Class
- **C++ Class:** `ANohamGameMode`
- **Header Location:** `Source/Noham_Base_Proj_Cpp/Public/Core/NohamGameMode.h`
- **When creating BP:** Search for "NohamGameMode" in class picker

---

## 📋 Blueprint Creation Steps

### 1. Create Character Blueprint
1. Open UE5 Editor
2. Content Browser → Right-click in desired folder
3. Blueprint Class → Search "NohamCharacter"
4. Select `ANohamCharacter` as parent
5. Name it: `BP_NohamCharacter` (or your preferred name)
6. Configure character settings in Blueprint

### 2. Create Game Mode Blueprint
1. Content Browser → Right-click
2. Blueprint Class → Search "NohamGameMode"
3. Select `ANohamGameMode` as parent
4. Name it: `BP_NohamGameMode`
5. Set Default Pawn Class to your `BP_NohamCharacter`

### 3. Set Project Game Mode
1. Edit → Project Settings
2. Maps & Modes
3. Default GameMode → Select `BP_NohamGameMode`
4. Save settings

---

## 🧹 Optional: Clean Up Old Template Blueprints

These UE5 First Person template Blueprints reference the old deprecated classes:
- `Content/FirstPerson/Blueprints/BP_FirstPersonCharacter.uasset`
- `Content/FirstPerson/Blueprints/BP_FirstPersonGameMode.uasset`

**You can safely delete these** since we're building fresh from our new C++ classes.

---

## ✅ Verification Checklist

After creating your Blueprints:
- [ ] Character Blueprint compiles without errors
- [ ] Game Mode Blueprint compiles without errors
- [ ] Default Pawn Class is set correctly in Game Mode
- [ ] Project Settings → Default GameMode is set
- [ ] PIE (Play in Editor) launches without errors
- [ ] Character spawns and responds to input

---

## 🔧 C++ Features Available in Blueprints

### ANohamCharacter Exposes:
- `FirstPersonCameraComponent` - VisibleAnywhere, BlueprintReadOnly
- `DefaultMappingContext` - EditAnywhere, BlueprintReadOnly
- `JumpAction` - EditAnywhere, BlueprintReadOnly
- `MoveAction` - EditAnywhere, BlueprintReadOnly
- `LookAction` - EditAnywhere, BlueprintReadOnly
- `GetFirstPersonCameraComponent()` - Public accessor

### ANohamGameMode Exposes:
- `DefaultPawnClass` - Set to `ANohamCharacter` in C++ (can override in BP)
- Standard GameMode functionality

---

## 📝 Naming Convention

Following project standards:
- **C++ Classes:** `ANoham*` prefix for Actors (correct UE5 convention)
- **Blueprints:** `BP_Noham*` prefix for clarity
- **No more:** `AUNoham*` (that was the violation we fixed)

---

**Once Blueprints are created, we're ready to proceed with Epic 5 implementation!**
