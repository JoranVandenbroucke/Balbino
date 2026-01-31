#include "Application.h"

#include <stdexcept>

#include <SDL3/SDL.h>

#include "Embedded/Shaders.hpp"

import FawnAlgebra;
using namespace FawnAlgebra;

namespace BalbinoApp
{
struct SplashScreenArt
{
    FawnVision::SShader shader;
};
void Application::Initialize()
{
    // todo : initialize memory manager (ensure memory is managed before other components)
    // todo : initialize logger/assert (set up logging and assertions early for debugging)

    if (FawnVision::InitializeSDL() != 0)
    {
        throw std::invalid_argument("Failed to initialize the renderer");
    }

    // todo : name by "game_name", if in editor update title to "balbino engine: game_name - open scene"
    if (constexpr FawnVision::SWindowCreateInfo windowCreateInfo{u8"Balbino Engine", 0, 650, 1000, static_cast<uint32_t>(FawnVision::window_flags::borderless | FawnVision::window_flags::vulkan)};
        CreateWindow(windowCreateInfo, m_window) != 0)
    {
        throw std::invalid_argument("Failed to create a window");
    }
    if (CreateRenderer(m_window, m_renderer) != 0)
    {
        throw std::invalid_argument("Failed to create a renderer");
    }
    SplashScreenArt* splashScreenArt{};
    auto passBuilder = FawnVision::AddRasterRenderPass<SplashScreenArt>(m_renderGraph, splashScreenArt);

    if (const FawnVision::ShaderCreateInfo shaderCreateInfo{{FawnVision::shader_stage::vertex, {g_fullscreenVert, g_fullscreenVert + sizeof(g_fullscreenVert)}}, {FawnVision::shader_stage::fragment, {g_fullscreenFrag, g_fullscreenFrag + sizeof(g_fullscreenFrag)}}};
        CreateShader(m_renderer, shaderCreateInfo, splashScreenArt->shader) != 0)
    {
        throw std::invalid_argument("Failed To Create Shader");
    }
    FawnVision::SetRenderFunc<SplashScreenArt>(m_renderGraph, passBuilder,
                                               [&window = m_window](const SplashScreenArt* pSplashScreenArt, const FawnVision::SRenderPassContext& renderContext)
                                               {
                                                   BindShader(renderContext, pSplashScreenArt->shader);

                                                   SetViewport(renderContext, 0.0f, 0.0f, static_cast<float>(window.width), static_cast<float>(window.height));
                                                   SetScissor(renderContext, window.width, window.height, 0, 0);

                                                   SetAlphaToCoverageEnable(renderContext, false);
                                                   SetColorBlendEnable(renderContext, false);
                                                   SetColorBlendEquation(renderContext, FawnVision::blend_factor::zero, FawnVision::blend_factor::one, FawnVision::blend_operator::add, FawnVision::blend_factor::zero,
                                                                         FawnVision::blend_factor::one, FawnVision::blend_operator::add);
                                                   SetColorWriteMask(renderContext, static_cast<FawnVision::color_component>(15));
                                                   SetCullMode(renderContext, FawnVision::cull_mode::back);
                                                   SetDepthBiasEnable(renderContext, false);
                                                   SetDepthCompareOperator(renderContext, FawnVision::compare_operator::less_or_equal);
                                                   SetDepthTestEnable(renderContext, false);
                                                   SetDepthWriteEnable(renderContext, false);
                                                   SetFrontFace(renderContext, true);
                                                   SetLineWidth(renderContext, 1.0f);
                                                   SetPolygonMode(renderContext, FawnVision::polygon_mode::fill);
                                                   SetPrimitiveRestartEnable(renderContext, false);
                                                   SetPrimitiveTopology(renderContext, FawnVision::primitive_topology::triangle_list);
                                                   SetRasterizationSamples(renderContext, 1U);
                                                   SetRasterizerDiscardEnable(renderContext, false);
                                                   SetStencilTestEnable(renderContext, false);
                                                   SetVertexInput(renderContext);
                                                   DrawFullscreen(renderContext);
                                               });
    Draw();
    CleanupRenderGraph(m_renderGraph);
    FawnVision::Cleanup(m_renderer, splashScreenArt->shader);

    if (DeerUI::Initialize(m_renderer, m_renderGraph, m_uiRenderer) != 0)
    {
        throw std::invalid_argument("Failed to initialize the ui renderer");
    }

    // todo : initialize font engine (set up font rendering)
    // todo : initialize user interface (UI) components (if your application has a GUI)
    // todo : initialize randomization (if needed for procedural generation)
    // todo : initialize file manager (prepare for file I/O operations)
    // todo : initialize asset manager (manage game assets like textures, models, etc.)
    // todo : initialize input manager (handle user input)
    // todo : initialize audio engine (set up audio processing)
    // todo : initialize physics engine (if applicable, for handling physics simulations)
    // todo : initialize AI module (initialize artificial intelligence systems)
    // todo : initialize network manager (if applicable, for networking functionalities)
    // todo : initialize scene manager (manage different scenes in the application)
    // todo : initialize scripting language (if applicable, set up scripting support)
}

void Application::LoadGame()
{
    // todo : initialize editor
    // todo : initialize first scene(s)

    // todo : read out desired flags;
    // todo : read out desired position;
    // todo : read out desired size;
    SetWindowFlags(m_window, static_cast<uint32_t>(FawnVision::window_flags::maximized | FawnVision::window_flags::resizable));
    SetWindowPosition(m_window);
    SetWindowSize(m_window, 860, 640);
    RecreateRenderer(m_window, m_renderer);

    DeerUI::CanvasHandle canvasHandle;
    DeerUI::UIElementHandle uiElementHandle;
    CreateCanvas(m_uiRenderer,ushort2{0u,0u},ushort2{80u,60u}, DeerUI::canvas_flag::background, canvasHandle );
    AddElement(m_uiRenderer, canvasHandle, DeerUI::LayoutProperties{.padding = ushort2{5u,10u}, .margin = ushort2{4u,4u}, .size = ushort2{64u,24u}, .border = uchar2{0u,0u}, .flex = uchar2{1u,1u}}, uiElementHandle);
}

void Application::Cleanup()
{
    DeerUI::Cleanup(m_renderer, m_uiRenderer);
    CleanupRenderGraph(m_renderGraph);
    ReleaseRenderer(m_renderer);
    ReleaseWindow(m_window);
}

void Application::Run()
{
    LoadGame();

    while (m_isRunning)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                m_isRunning = false;
            }
            else if (event.type == SDL_EVENT_WINDOW_RESIZED)
            {
                GetWindowSize(m_window);
                WindowResize();
            }
        }
        // todo : update physics
        // todo : update scene(s)
        Draw();
    }
}
void Application::Draw()
{
    ExecuteAll(m_renderer, m_renderGraph);
}

void Application::WindowResize()
{
    // todo: handle resize
    RecreateRenderer(m_window, m_renderer);
}
} // namespace BalbinoApp
