# MACHI Game Engine

A lightweight, modular C++ game engine built with OpenGL, designed for cross-platform game development with a focus on clean architecture and extensibility.

**Current Version:** 0.0.08

## Overview

MACHI is a foundational game engine that prioritizes a clean, event-driven architecture. The engine handles window management, rendering, input processing, and frame timing, with an intentional design that leaves room for future systems like physics, animation, and audio.

Think of MACHI as the backbone of a game—it manages all the core plumbing so you can focus on game logic rather than low-level graphics code.

## Features

### Current (v0.0.08)
- **Window Management** - Cross-platform window creation and management via GLFW
- **Event System** - Decoupled event-driven communication between engine components
- **Rendering Pipeline** - OpenGL-based 3D rendering with shader support
- **Texture Management** - Multi-texture support with STB image loading
- **3D Camera Controls** - Keyboard-controlled camera movement (WASD)
- **Frame Timing** - Delta-time calculations and FPS monitoring
- **Comprehensive Logging** - File and console logging for debugging

### Planned
- Scene Management (SceneManager)
- Physics Engine
- Audio System
- Particle Systems
- UI Framework
- Animation System
- Asset Management

## Technical Stack

| Component | Technology |
|-----------|-----------|
| Build System | CMake 3.10+ |
| Graphics API | OpenGL with GLAD |
| Window Management | GLFW3 |
| Math Library | GLM |
| Image Loading | STB Image |
| Language | C++17 |
| Platform Support | Windows, macOS, Linux |

## Getting Started

### Prerequisites

**Linux/macOS:**
```bash
# macOS (with Homebrew)
brew install cmake glfw3 glm

# Ubuntu/Debian
sudo apt-get install cmake libglfw3-dev libglm-dev
```

**Windows:**
- CMake 3.10 or later
- Visual Studio 2017 or later (for MSVC)
- GLFW3 and GLM (can be installed via vcpkg or built from source)

### Building

1. Clone the repository:
```bash
git clone https://github.com/yourusername/machi-engine.git
cd machi-engine
```

2. Build using the provided script:
```bash
./build.sh                    # Debug build
./build.sh -d                # Debug mode
./build.sh -c                # Clean rebuild
./build.sh --no-run           # Build without running
```

Or build manually:
```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
./machi
```

### Quick Start

```cpp
#include "Engine.hpp"

int main() {
    Engine engine("My Game", 1024, 768);
    
    if (!engine.initialize()) {
        std::cerr << "Failed to initialize engine!" << std::endl;
        return -1;
    }
    
    engine.run();
    return 0;
}
```

## Architecture

### Engine Initialization Flow

The engine initializes subsystems in a specific order to ensure dependencies are met:

```
WindowSystem (OpenGL context)
    ↓
InputSystem (requires OpenGL context)
    ↓
Engine Loop (main game loop)
```

### Core Components

**Engine** - Main orchestrator that manages the game loop, timing, and subsystem lifecycle.

**WindowManager** - Abstracts GLFW window creation and management, handles platform-specific details.

**Event System** - Decoupled message passing system that allows components to communicate without tight coupling:
  - Window events (resize, close)
  - Input events (keyboard, mouse)
  - Custom engine events

**Shader & Texture** - Graphics resource management for rendering.

**Logger** - Configurable logging system with file output support.

## Usage Examples

### Configuring the Engine

```cpp
EngineConfig config;
config.windowTitle = "My Game";
config.windowWidth = 1280;
config.windowHeight = 720;
config.targetFPS = 60.0f;
config.enableVSync = true;
config.clearR = 0.1f;
config.clearG = 0.1f;
config.clearB = 0.1f;

Engine engine(config);
engine.initialize();
```

### Listening to Events

```cpp
engine.addEventListener([](const EngineEvent& event) {
    if (event.type == EngineEventType::KeyPress) {
        int key = event.data.keyboard.key;
        // Handle key press
    }
});
```

### Controlling the Engine

```cpp
// Check engine state
float deltaTime = engine.getDeltaTime();
float fps = engine.getFPS();

// Manipulate engine at runtime
engine.setClearColor(0.2f, 0.3f, 0.4f, 1.0f);
engine.enableDepthTest(true);
engine.setWindowSize(1920, 1080);

// Control execution
engine.pause();
engine.resume();
engine.shutdown();
```

## Keyboard Controls

| Key | Action |
|-----|--------|
| W/A/S/D | Move camera |
| ESC | Close application |
| F1 | Print debug stats |
| F2 | Toggle fullscreen |

## Project Structure

```
machi-engine/
├── include/           # Header files
│   ├── Engine.hpp
│   ├── WindowManager.hpp
│   ├── Shader.hpp
│   ├── Texture.hpp
│   ├── Logger.hpp
│   └── InputManager.hpp
├── src/              # Implementation files
│   ├── main.cpp
│   ├── Engine.cpp
│   ├── WindowManager.cpp
│   ├── Shader.cpp
│   ├── Texture.cpp
│   └── Logger.cpp
├── external/         # Third-party libraries
│   ├── glad/
│   └── stb/
├── resources/        # Shaders, textures, assets
│   ├── shaders/
│   └── textures/
├── build.sh          # Build automation script
├── CMakeLists.txt
└── README.md
```

## Performance

- **Frame Rate:** Targets 60 FPS on modern hardware
- **Resolution:** Tested up to 1920x1080 (higher resolutions supported)
- **Rendering:** Hardware-accelerated OpenGL 3.3+
- **Memory:** Minimal baseline footprint (~50MB compiled)

## Development Roadmap

### Phase 1 (Current)
- [x] Window management
- [x] Basic rendering pipeline
- [x] Event system
- [x] Input handling
- [x] Logging system

### Phase 2 (In Progress)
- [ ] Scene Management
- [ ] Renderer abstraction layer
- [ ] Extended camera system

### Phase 3 (Planned)
- [ ] Physics engine
- [ ] Audio system
- [ ] Particle effects
- [ ] UI framework

### Phase 4 (Future)
- [ ] Animation system
- [ ] Asset pipeline
- [ ] Editor tools
- [ ] Performance profiling

## Contributing

Contributions are welcome! Here are some ways you can help:

1. **Report bugs** - Use the issue tracker for bug reports
2. **Suggest features** - Open an issue to discuss new features
3. **Submit pull requests** - Fork, create a branch, and submit a PR
4. **Improve documentation** - Help keep docs clear and up-to-date

## Building from Source (Advanced)

If dependencies aren't available through your package manager:

```bash
# GLFW from source
git clone https://github.com/glfw/glfw.git
cd glfw && mkdir build && cd build
cmake .. && cmake --build .
sudo cmake --install .

# GLM (header-only, just clone)
git clone https://github.com/g-truc/glm.git /usr/local/include/glm
```

## Troubleshooting

### "OpenGL context creation failed"
- Ensure your graphics drivers are up to date
- Verify OpenGL 3.3+ support on your system

### "GLFW not found"
- Install GLFW development libraries for your OS
- Update CMake find_package paths if installed in custom location

### Build errors on macOS
- Xcode command line tools may need updating: `xcode-select --install`
- Ensure GLFW is installed: `brew install glfw3`

### Performance issues
- Check FPS counter (F1 key)
- Reduce render resolution or complexity
- Profile with your platform's profiler

## License

This project is licensed under the MIT License—see LICENSE file for details.

## Acknowledgments

- GLFW for cross-platform windowing
- OpenGL for graphics rendering
- GLM for mathematics
- STB libraries for image loading

## Contact & Support

For questions, issues, or suggestions, open an issue on the GitHub repository.

---

**Happy game developing!** 🎮
