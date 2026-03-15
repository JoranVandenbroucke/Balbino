# Balbino

Balbino started as a school project and has grown through many iterations.
It's a passion project designed to showcase my skills to fellow developers and collaborators.
I’m open to feedback, especially on performance improvements.
While I appreciate suggestions and pull requests for new features, I likely won't accept them as this a personal project
in which I want to stay the main maintainer.

## Libraries

- [SDL3](https://github.com/libsdl-org/sdl): Window creation, input handling, etc.
- [FawnAlgebra](https://github.com/JoranVandenbroucke/FawnAlgebra): Custom math library
- [FawnVision](https://github.com/JoranVandenbroucke/FawnVision): Custom rendering library
    - [Vulkan](https://www.lunarg.com/vulkan-sdk/): Graphics API
    - [DeerUI](https://github.com/JoranVandenbroucke/FawnVision/tree/main/Source/Interface/UI): Custom UI library
    - [DeerFont](https://github.com/JoranVandenbroucke/FawnVision): Custom font loading library
    - Headless API (*TBA): For testing
- FawnAI (TBA): Custom game AI library for NPC/character behavior
- FawnAudio (TBA): Custom audio library
- [FawnDirection (TBA)](https://github.com/JoranVandenbroucke/FawnVision): Custom render direction library
- [FawnMemory (TBA)](https://github.com/JoranVandenbroucke/FawnVision): Custom memory management library
- FawnPhysics (TBA): Physics library based on [PhysX](https://github.com/NVIDIAGameWorks/PhysX)
- BalbinoEditor (TBA): The engine’s editor

\* TBA: To be added

## Building

Balbino uses CMake and provides several build presets:

- Debug
- Debug Editor
- Release
- Release Editor

### Windows

Use the `Windows` prefix for building on Windows:

```powershell
cmake -B Build -S . --preset Windows-DebugEditor
cmake --build --preset Windows-DebugEditor
```

### Linux

For Linux builds, use the `Linux` prefix:

```bash
cmake -B Build -S . --preset Linux-DebugEditor
cmake --build --preset Linux-DebugEditor
```

### Mac

To be added.
