# Epic 3: Core Character Controller & Input Systems

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
