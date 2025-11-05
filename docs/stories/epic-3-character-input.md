# Epic 3: Core Character Controller & Input Systems

**Epic Goal:** Implement a comprehensive FPS character controller with movement, interaction capabilities, and multi-platform input framework. This epic delivers playable character control that works seamlessly across keyboard/mouse, gamepad, and Steam Deck, providing the essential gameplay foundation for all future game projects.

**Success Criteria:** Fully functional FPS character with smooth movement, interaction system, and seamless input switching across all supported platforms.

**Dependencies:** Epic 1 (Foundation), Epic 2 (NEON UI for input prompts)
**Estimated Duration:** 2-3 weeks

---

## Story 3.1: Basic FPS Movement Controller

**As a player,**  
**I want responsive first-person movement with WASD controls, mouse look, and smooth camera handling,**  
**so that I can navigate the game world intuitively with standard FPS controls.**

### Acceptance Criteria:
1. ⚠️ First-person camera controller with smooth mouse look and configurable sensitivity
2. ⚠️ WASD movement with proper acceleration and deceleration
3. ⚠️ Movement speed configurable through game settings
4. ⚠️ Camera collision prevention and smooth movement interpolation
5. ⚠️ Movement system maintains 120+ FPS performance target

### Implementation Tasks:
- [ ] Refactor existing BP_FirstPersonCharacter to use Enhanced Input system
- [ ] Create NohamCharacterController.h/.cpp with movement framework
- [ ] Implement smooth mouse look with sensitivity scaling
- [ ] Add WASD movement with configurable speed and acceleration
- [ ] Create camera collision detection and correction
- [ ] Add movement interpolation for smooth motion
- [ ] Integrate with settings system for sensitivity/speed configuration
- [ ] Performance optimization to meet 120+ FPS target
- [ ] Create movement testing level for validation

### Definition of Done:
- Character movement feels responsive and smooth
- Mouse sensitivity configurable and saves to settings
- Movement speed adjustable via game settings
- Camera collision works reliably in various scenarios
- Performance consistently meets 120+ FPS target
- Movement works correctly in both editor and packaged builds

**Priority:** CRITICAL  
**Estimated Effort:** 5-6 days  
**Assignee:** Developer  
**Dependency:** Epic 1 (Template system), Epic 2 (Settings UI for configuration)

---

## Story 3.2: Advanced Movement Mechanics

**As a player,**  
**I want additional movement options including crouch, jump, and aim functionality,**  
**so that I can interact with the game environment in standard FPS ways.**

### Acceptance Criteria:
1. ❌ Crouch functionality with smooth height transition and speed modification
2. ❌ Jump mechanics with configurable height and gravity settings
3. ❌ Aim-down-sights functionality with FOV adjustment and movement speed reduction
4. ❌ Sprint capability with stamina considerations (framework for future implementation)
5. ❌ All movement mechanics work smoothly together without conflicts

### Implementation Tasks:
- [ ] Implement crouch system with capsule collision adjustment
- [ ] Add smooth crouch transitions using interpolation
- [ ] Create jump mechanics with air control and landing
- [ ] Implement aim-down-sights with FOV and speed changes
- [ ] Add sprint framework with speed boost
- [ ] Create animation integration points for future character meshes
- [ ] Test all movement combinations for conflicts
- [ ] Add movement state debugging visualization
- [ ] Optimize performance for all movement states

### Definition of Done:
- Crouch smoothly adjusts collision and movement speed
- Jump feels responsive with proper air control
- Aim-down-sights provides tactical advantage with appropriate trade-offs
- All movement mechanics work together without glitches
- Framework ready for future animation integration
- Performance maintains 120+ FPS with all mechanics active

**Priority:** HIGH  
**Estimated Effort:** 6-7 days  
**Assignee:** Developer  
**Dependency:** Story 3.1 (Basic Movement)

---

## Story 3.3: Interaction System

**As a player,**  
**I want to interact with objects in the game world using the E key,**  
**so that I can engage with interactive elements and trigger game events.**

### Acceptance Criteria:
1. ❌ Interaction raycast system detecting objects within interaction range
2. ❌ Visual feedback for interactable objects (outline, prompt, or similar)
3. ❌ E key interaction with objects implementing interaction interface
4. ❌ Interaction prompts display appropriate input method (keyboard vs gamepad)
5. ❌ Example interactable objects created to demonstrate system functionality

### Implementation Tasks:
- [ ] Create IInteractable interface for objects
- [ ] Implement interaction raycast system in character controller
- [ ] Add interaction range configuration to settings
- [ ] Create visual feedback system (outline shader or highlight)
- [ ] Implement interaction prompts using NEON UI components
- [ ] Add input method detection for prompt display
- [ ] Create example interactable objects (door, pickup, button)
- [ ] Integrate interaction feedback with HUD system
- [ ] Test interaction system with various object types

### Definition of Done:
- Interaction raycast reliably detects objects within range
- Visual feedback clearly indicates interactable objects
- Interaction prompts show correct input method (E key vs gamepad button)
- Example objects demonstrate all interaction patterns
- System integrates smoothly with NEON UI for prompts
- Interaction system performs well with multiple objects

**Priority:** HIGH  
**Estimated Effort:** 5-6 days  
**Assignee:** Developer  
**Dependency:** Story 3.1 (Basic Movement), Epic 2 (NEON UI for prompts)

---

## Story 3.4: Multi-Platform Input Framework

**As a player using different input devices,**  
**I want seamless switching between keyboard/mouse, gamepad, and Steam Deck controls,**  
**so that I can play comfortably regardless of my preferred input method or platform.**

### Acceptance Criteria:
1. ❌ Input detection automatically switches between keyboard/mouse and gamepad modes
2. ❌ UI prompts dynamically update to show correct input icons for active device
3. ❌ All character controller functions mapped for both input types
4. ❌ Steam Deck specific input considerations implemented and tested
5. ❌ Input settings allow full customization of key bindings and controller mappings

### Implementation Tasks:
- [ ] Create InputManagerSubsystem.h/.cpp for input mode detection
- [ ] Implement automatic input device switching
- [ ] Create input mapping configuration system
- [ ] Add gamepad input mappings for all character functions
- [ ] Implement Steam Deck specific input handling
- [ ] Create dynamic input prompt system for UI
- [ ] Add key binding customization interface
- [ ] Create input mode debugging and testing tools
- [ ] Test all input methods thoroughly
- [ ] Document input system for future extension

### Definition of Done:
- Input switching works seamlessly without user intervention
- UI prompts update correctly when input method changes
- All character functions accessible via keyboard/mouse and gamepad
- Steam Deck input mapping works correctly
- Key binding customization saves and loads correctly
- Input system extensible for future game-specific controls

**Priority:** CRITICAL  
**Estimated Effort:** 8-9 days  
**Assignee:** Developer  
**Dependency:** Story 3.1-3.3 (All movement and interaction systems)

---

## Epic 3 Success Validation

### Epic Completion Checklist:
- [ ] All 4 stories completed and acceptance criteria met
- [ ] Character movement smooth and responsive across all input methods
- [ ] Interaction system functional with visual feedback
- [ ] Multi-platform input switching works seamlessly
- [ ] Performance targets met (120+ FPS) with all systems active
- [ ] Input customization saves and loads correctly

### Epic Demo:
1. Demonstrate smooth FPS movement with keyboard/mouse
2. Switch to gamepad mid-game, show seamless transition
3. Test all movement mechanics (crouch, jump, aim, sprint)
4. Interact with objects using both input methods
5. Show UI prompts updating dynamically with input changes
6. Demonstrate Steam Deck compatibility

### Performance Validation:
- [ ] 120+ FPS maintained with all movement systems active
- [ ] Input lag <16ms for all input methods
- [ ] Memory usage <100MB for character controller systems
- [ ] No frame drops during input method switching

### Next Epic Dependencies:
Epic 3 provides the character controller foundation for Epic 4 (Settings Management) and Epic 5 (Developer Tools). The input framework established here will be extended for settings interfaces and developer tool interactions.