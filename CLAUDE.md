# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a space shooter game built with C++17 using SDL2. The game features a component-based architecture with clear separation between game logic, rendering, and resource management.

## Build Commands

### Standard Build
```bash
make build              # CMake + Make build
make build_ninja        # CMake + Ninja build (faster)
```

### Debug Build
```bash
make debug              # Debug build with Make
make debug_ninja        # Debug build with Ninja (recommended)
```

### Run Game
```bash
make run                # Build and run the game
./build/game           # Run directly
```

### Code Quality
```bash
make format            # Format code with clang-format (Chromium style)
make tidy              # Run clang-tidy for static analysis
make cpplint           # Run cpplint checks
make cppcheck          # Run cppcheck analysis
```

### Development Workflow
```bash
make watch_run         # Auto-rebuild and run on file changes (requires entr)
make watch_debug       # Auto-rebuild debug version on changes
```

### Dependencies Installation
```bash
make install_osx_deps     # Install dependencies on macOS
make install_ubuntu_deps  # Install dependencies on Ubuntu
```

## Architecture Overview

### Entity Hierarchy
The game uses inheritance-based entity system:
- `Actor` (base class) → `Ship` → `PlayerShip`
- `Actor` → `Bullet`
- `Enemy` class owns a `Ship` instance (composition)

### Key Systems
1. **ResourceManager**: Centralized asset loading from JSON manifests
   - Textures loaded from `resources/manifests/spritesheets.json`
   - Fonts from `resources/manifests/fonts.json`
   - Sprites from `resources/manifests/sprites.json`

2. **Stage System**: Game state management
   - `Stage` (abstract) → `LevelStage` (gameplay implementation)
   - Handles entity spawning, collision detection, and game flow

3. **Input System**: `InputManager` tracks keyboard state with multiple control schemes

4. **Memory Management**: Extensive use of `shared_ptr` and `unique_ptr` for safety

### Important Files
- `src/config.h`: Game constants and configuration
- `src/game.h`: Main game loop and initialization
- `src/stage.h/cpp`: Core gameplay logic
- `src/resource.h/cpp`: Asset management system

### Libraries Used
- SDL2pp: C++ wrapper for SDL2 (in vendor/libSDL2pp)
- spdlog: Logging (in vendor/spdlog)
- nlohmann/json: JSON parsing (in vendor/json)

## Development Notes

- The project uses C++17 with strict warnings enabled
- Clang sanitizers are enabled in debug builds (address and undefined behavior)
- Resources are copied to build directory during CMake configuration
- The game viewport is 800x600 with configurable bounds in `config.h`