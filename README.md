# Balbino

Balbino is an evolving game engine initially created as a school project. It has since morphed into a passion project,
credited with multiple revamps, and serves as an impressive demonstration of programified creativity.

---

## 3rd Party Libraries

The engine is equipped with the below third-party libraries:

- [Open Asset Import Library](https://github.com/assimp/assimp) (Assimp) for 3D model loading;
- [Catch2](https://github.com/catchorg/Catch2) for robust unit testing;
- [EnTT](https://github.com/skypjack/entt) for effective entity-component system management;
- [OpenGL Image](https://github.com/g-truc/gli) (GLI) for reading DDS and KTX files;
- [OpenGL Mathematics](https://github.com/g-truc/glm) (GLM) for all math related functions;
- [HDRI Loader from Igor Kravtchenko](https://www.flipcode.com/archives/HDR_Image_Reader.shtml) for HDRI support;
- [mINI](https://github.com/pulzed/mINI) for INI file reading and writing;
- [ImGui](https://github.com/ocornut/imgui), [ImGuizmo](https://github.com/CedricGuillemet/ImGuizmo),
  and [imnodes](https://github.com/Nelarius/imnodes) for UI creation;
- [mikktspace](https://github.com/mmikk/MikkTSpace) for tangent loading;
- [SDL2](https://github.com/libsdl-org/SDL) for window initialisation and input event handling;
- [SDL2_image](https://github.com/libsdl-org/SDL_image) for image input;
- [SDL2_mixer](https://www.libsdl.org/projects/SDL_mixer/) for sound (Todo);
- [SDL2_net](https://www.libsdl.org/projects/SDL_net/) for networking (Todo);
- [SDL2_rtf](https://www.libsdl.org/projects/SDL_rtf/) for rich type font (Todo);
- [SDL2_ttf](https://www.libsdl.org/projects/SDL_ttf/)  for TrueType fonts support (Todo);
- [Vulkan SDK](https://vulkan.lunarg.com/) as the rendering API.

---

## Features

- Balbino uses a homemade file system and importer that converts assets like 3D models, Textures and Audio into a binary
  file;
- Files, Assets and Components have a UUID so that they can be found back and identified no mather where hey are;
- The math library is currently just a wrapper for GLM but there are plans to replace GLM with a homemade library;
- Balbino Editor has a Shader Graph and a Material Editor, made for easy creation and updating of shaders Sand
  Materials;
- Error handling and Logging can be done trough FawnTrace;
- The renderer just uses Forward Pass rendering (plans for Forward+ rendering);
- There is a custom Memory Manager ... but I'm too scared to enable it ...

---

## Building

To set up the development environment, the project provides `setup.ps1` for Windows systems, and `setup.sh` for
Unix-based systems. These scripts help in configuring the project setting automatically.

Additionally, multiple CMake presets are available to ease the setup process for different development and distribution
scenarios:
---

### Windows

Visual Studio

```
Debug-Windows-VS-VS
DebugEditor-Windows-VS-VS
Release-Windows-VS-VS
ReleaseEditor-Windows-VS-VS
```

Which can be used as:

```powershell
cmake --preset Debug-Windows-VS-VS -S .
```

---
Visual Studio with LLVM

```
Debug-Windows-VS-LLVM
DebugEditor-Windows-VS-LLVM
Release-Windows-VS-LLVM
ReleaseEditor-Windows-VS-LLVM
```

Which can be used as:

```powershell
cmake --preset Debug-Windows-VS-LLVM -S .
```

---
If Ninja is preferred over Visual Studio, then you can also use:
Ninja with Visual studio compiler

```
Debug-Windows-NJ-VS
DebugEditor-Windows-NJ-VS
Release-Windows-NJ-VS
ReleaseEditor-Windows-NJ-VS
```

Which can be used as:

```powershell
cmake --preset Debug-Windows-NJ-VS -S .
```

---
Ninja with LLVM compiler

```
Debug-Windows-NJ-LLVM
DebugEditor-Windows-NJ-LLVM
Release-Windows-NJ-LLVM
ReleaseEditor-Windows-VS-LLVM
```

Which can be used as:

```powershell
cmake --preset Debug-Windows-NJ-LLVM -S .
```

---

## Future plans

A comprehensive list of goals and planned features of Balbino is available on the
project's [Trello Page](https://trello.com/b/NjIxYnGU/balbino-development-goals).
