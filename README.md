# Balbino
This Game Engine was originaly used in a school assignment.
Balbino has now been revamped multiple times and is starting to become a hobie and a project to show off to sompanies.

used Lybraries are:
- [Open Asset Import Library](https://github.com/assimp/assimp)(Assimp), for loading 3d models;
- [EnTT](https://github.com/skypjack/entt) as a entity component system;
- [OpenGL Image](https://github.com/g-truc/gli) (GLI), for reading in DDS files;
- [OpenGL Mathematics](https://github.com/g-truc/glm) (GLM), for math;
- [mINI](https://github.com/g-truc/glm), for reading and writing ini files;
- [SDL2](https://github.com/libsdl-org/SDL), to initiate the window and handel input events;
- [SDL2_images](https://github.com/libsdl-org/SDL_image), for loading in images;
- [Vulkan SDK](https://vulkan.lunarg.com/), as render API;
- [ImGui](https://github.com/ocornut/imgui), [ImGuizmo](https://github.com/CedricGuillemet/ImGuizmo) and [imnodes](https://github.com/Nelarius/imnodes) for the UI;
- [HDRI loader from Igor Kravtchenko](https://www.flipcode.com/archives/HDR_Image_Reader.shtml), to load in HDRI's;
- [mikktspace](https://github.com/mmikk/MikkTSpace), to correctly load tangents;

Current features include:
- Mesh Importer
- Basic UI
- Basic Shader Editor
- Basic Lights
- Basic Material Editor
- Custom (Binalry)File type to unify all assets

Most things are handled trough UUIDs (64bit integers)
Assets are UUID bound, his means no mather were an asset is located it will find it back, even the name can change.

My Goasls and planned features can be found back on my [Trello Page](https://trello.com/b/NjIxYnGU/balbino-development-goals)