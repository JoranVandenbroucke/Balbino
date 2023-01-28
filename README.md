# Balbino
This game engine was created as a school project.
Balbino has been revamped multiple times and is starting to become a hobby and a project to show off.

Used Libraries are:
- [Open Asset Import Library](https://github.com/assimp/assimp)(Assimp), for loading 3d models;
- [EnTT](https://github.com/skypjack/entt) as an entity component system;
- [OpenGL Image](https://github.com/g-truc/gli) (GLI), for reading DDS and KTX files;
- [OpenGL Mathematics](https://github.com/g-truc/glm) (GLM), for math;
- [mINI](https://github.com/pulzed/mINI), for reading and writing ini files;
- [SDL2](https://github.com/libsdl-org/SDL) to initiate the window and handle input events;
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
- Custom (Binary)File type that unifies all assets

64-bit integers, known as UUIDs, manage most tasks. Every asset possesses a UUID. The UUID ensures asset retrieval from any location within the project folder, regardless of a name or location change.

Goals and planned features found on [Trello Page](https://trello.com/b/NjIxYnGU/balbino-development-goals)