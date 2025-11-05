# Introduction

This document outlines the complete fullstack architecture for **NohamBaseProject**, including backend systems, frontend implementation, and their integration. It serves as the single source of truth for AI-driven development, ensuring consistency across the entire technology stack.

This unified approach combines what would traditionally be separate backend and frontend architecture documents, streamlining the development process for modern fullstack applications where these concerns are increasingly intertwined.

### Starter Template or Existing Project

Based on the PRD analysis, this uses an **existing UE5 project** (Noham_Base_Proj_Cpp) with specific technology constraints:

- **Base:** Existing Unreal Engine 5.5.4 project (Noham_Base_Proj_Cpp.uproject)
- **Unique Approach:** NEON plugin integration for HTML/JavaScript UI within UE5 widgets
- **Target Platform:** Steam PC primary, Steam Deck secondary
- **Architecture:** Traditional UE5 C++ structure with web-based UI layer

**🚨 CRITICAL:** Use existing project `Noham_Base_Proj_Cpp` - DO NOT create new UE5 projects. All development extends the existing project structure with NEON+shadcn integration.

### Change Log

| Date | Version | Description | Author |
|------|---------|-------------|---------|
| 2025-10-25 | v1.0 | Initial fullstack architecture from PRD | Winston (Architect) |
