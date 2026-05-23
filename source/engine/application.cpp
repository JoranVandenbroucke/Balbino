#include "application.hpp"

#include "shaders.hpp"

import FawnAlgebra;
import std;
using namespace fawn_algebra;

namespace BalbinoApp
{
struct SplashScreenArt
{
    fawn_vision::Shader shader;
};

void Application::Initialize()
{
    // todo : initialize memory manager (ensure memory is managed before other
    // components) todo : initialize logger/assert (set up logging and
    // assertions early for debugging)

    if (!fawn_vision::InitializeSDL())
    {
        throw std::invalid_argument("Failed to initialize the sdl");
    }

    // todo : name by "game_name", if in editor update title to "balbino engine:
    // game_name - open scene"
    if (constexpr fawn_vision::WindowCreateInfo windowCreateInfo{u8"Balbino Engine", 0, 860, 640, fawn_vision::window_flags::borderless | fawn_vision::window_flags::vulkan};
        !fawn_vision::CreateWindow(windowCreateInfo, m_window))
    {
        throw std::invalid_argument("Failed to create a window");
    }
    if (fawn_vision::CreateRenderer(m_window, m_renderer) != fawn_vision::gfx_status::ok)
    {
        throw std::invalid_argument("Failed to create a renderer");
    }
    SplashScreenArt* splashScreenArt{};
    auto passBuilder = fawn_vision::AddRasterRenderPass<SplashScreenArt>(m_renderGraph, splashScreenArt);

    const std::array<fawn_vision::ShaderData, 2> shaderData{
        fawn_vision::ShaderData{fawn_vision::shader_stage::vertex, {g_fullscreenVert, g_fullscreenVert + g_fullscreenVertSize}},
        fawn_vision::ShaderData{fawn_vision::shader_stage::fragment, {g_fullscreenFrag, g_fullscreenFrag + g_fullscreenFragSize}},
    };
    if (fawn_vision::CreateShader(m_renderer, std::span(shaderData), splashScreenArt->shader) != fawn_vision::gfx_status::ok)
    {
        throw std::invalid_argument("Failed To Create Shader");
    }

    fawn_vision::SetRenderFunc<SplashScreenArt>(
        m_renderGraph, passBuilder,
        [&window = m_window](const SplashScreenArt* pSplashScreenArt, const fawn_vision::RenderPassContext& renderContext)
        {
            fawn_vision::BindShader(renderContext, pSplashScreenArt->shader);

            fawn_vision::SetViewport(renderContext, 0.0f, 0.0f, static_cast<float>(window.width), static_cast<float>(window.height));
            fawn_vision::SetScissor(renderContext, static_cast<std::uint32_t>(window.width), static_cast<std::uint32_t>(window.height), 0, 0);

            fawn_vision::SetAlphaToCoverageEnable(renderContext, false);
            fawn_vision::SetColorBlendEnable(renderContext, false);

            fawn_vision::SetColorBlendEquation(renderContext, fawn_vision::blend_factor::zero, fawn_vision::blend_factor::zero, fawn_vision::blend_operator::add,
                                               fawn_vision::blend_factor::one, fawn_vision::blend_factor::one, fawn_vision::blend_operator::add);
            fawn_vision::SetColorWriteMask(renderContext, static_cast<fawn_vision::color_component>(15));
            fawn_vision::SetCullMode(renderContext, fawn_vision::cull_mode::back);
            fawn_vision::SetDepthBiasEnable(renderContext, false);
            fawn_vision::SetDepthCompareOperator(renderContext, fawn_vision::compare_operator::less_or_equal);
            fawn_vision::SetDepthTestEnable(renderContext, false);
            fawn_vision::SetDepthWriteEnable(renderContext, false);
            fawn_vision::SetFrontFace(renderContext, true);
            fawn_vision::SetLineWidth(renderContext, 1.0f);
            fawn_vision::SetPolygonMode(renderContext, fawn_vision::polygon_mode::fill);
            fawn_vision::SetPrimitiveRestartEnable(renderContext, false);
            fawn_vision::SetPrimitiveTopology(renderContext, fawn_vision::primitive_topology::triangle_list);
            fawn_vision::SetRasterizationSamples(renderContext, 1U);
            fawn_vision::SetRasterizerDiscardEnable(renderContext, false);
            fawn_vision::SetStencilTestEnable(renderContext, false);
            fawn_vision::SetVertexInput(renderContext);
            fawn_vision::DrawFullscreen(renderContext);
        });
    Draw();
    fawn_vision::CleanupRenderGraph(m_renderGraph);
    fawn_vision::Cleanup(m_renderer, splashScreenArt->shader);

    if (deer_ui::Initialize(m_renderer, m_renderGraph, m_uiRenderer) != 0)
    {
        throw std::invalid_argument("Failed to initialize the ui renderer");
    }

    // todo : initialize font engine (set up font rendering)
    // todo : initialize user interface (UI) components (if your application
    // has a GUI) todo : initialize randomization (if needed for procedural
    // generation) todo : initialize file manager (prepare for file I/O
    // operations) todo : initialize asset manager (manage game assets like
    // textures, models, etc.) todo : initialize input manager (handle user
    // input) todo : initialize audio engine (set up audio processing) todo
    // : initialize physics engine (if applicable, for handling physics
    // simulations) todo : initialize AI module (initialize artificial
    // intelligence systems) todo : initialize network manager (if
    // applicable, for networking functionalities) todo : initialize scene
    // manager (manage different scenes in the application) todo :
    // initialize scripting language (if applicable, set up scripting
    // support)
}

void Application::LoadGame()
{
    // todo : initialize editor
    // todo : initialize first scene(s)

    // todo : read out desired flags;
    // todo : read out desired position;
    // todo : read out desired size;
    // if (!fawn_vision::SetWindowFlags(m_window, fawn_vision::window_flags::maximized | fawn_vision::window_flags::resizable))
    // {
    //     std::cerr << "could not set window flag\n";
    // }
    // if (!fawn_vision::SetWindowPosition(m_window))
    // {
    //     std::cerr << "could not set window position\n";
    // }
    // if (!fawn_vision::SetWindowSize(m_window, 860, 640))
    // {
    //     std::cerr << "could not set window size\n";
    // }
    // if (fawn_vision::RecreateRenderer(m_window, m_renderer) != fawn_vision::gfx_status::ok)
    // {
    //     std::cerr << "could not set create renderer\n";
    // }

    /*
        // Create a canvas that fills the screen
        deer_ui::CanvasHandle hud;
        deer_ui::CreateCanvas(m_uiRenderer, {0, 0}, // position
                           {1920, 1080},         // size (0,0 = fill)
                           0,                    // layer
                           deer_ui::canvas_flag::accepts_inputs, hud);

        // ---------------------------------------------------------------------------
        // Simple square — just a colored quad, no components
        // ---------------------------------------------------------------------------

        deer_ui::UIElementHandle square;
        deer_ui::AddElement(m_uiRenderer, hud, deer_ui::invalidElement,
                            deer_ui::LayoutProperties{
                                .padding = {4, 4},
                                .margin  = {8, 8},
                                .size    = {64, 64},
                            },
                            {0.2F, 0.6F, 1.0F, 1.0F}, // blue
                            square);

        // ---------------------------------------------------------------------------
        // Button — square + click component
        // ---------------------------------------------------------------------------

        deer_ui::UIElementHandle button;
        deer_ui::AddElement(m_uiRenderer, hud, deer_ui::invalidElement,
                            deer_ui::LayoutProperties{
                                .padding = {8, 4},
                                .margin  = {8, 8},
                                .size    = {120, 40},
                            },
                            {0.3F, 0.3F, 0.3F, 1.0F}, // dark grey
                            button);

        deer_ui::AttachClickComponent(
            m_uiRenderer, hud, button,
            []
            {
                std::println("pressed");
            }, // onPress
            []
            {
                std::println("released");
            }, // onRelease
            []
            {
                std::println("hovered");
            }); // onHover
    */
}

void Application::Cleanup()
{
    deer_ui::Cleanup(m_renderer, m_uiRenderer);
    fawn_vision::CleanupRenderGraph(m_renderGraph);
    fawn_vision::ReleaseRenderer(m_renderer);
    fawn_vision::ReleaseWindow(m_window);
}

void Application::Run()
{
    LoadGame();
    std::cout << "game loaded\n";

    while (m_isRunning)
    {
        const std::array<std::pair<fawn_vision::event_type, std::function<void(const fawn_vision::EventContext&)>>, 2> handlers{
            std::make_pair(fawn_vision::event_type::quit,
                           [&](const fawn_vision::EventContext&)
                           {
                               m_isRunning = false;
                           }),
            // todo : add keyboard input support
            std::make_pair(fawn_vision::event_type::window_resized,
                           [&](const fawn_vision::EventContext&)
                           {
                               WindowResize();
                           })};

        fawn_vision::PollEvents(std::span(handlers));
        // todo : update physics
        // todo : update scene(s)
        Draw();
    }
}
void Application::Draw()
{
    fawn_vision::ExecuteAll(m_renderer, m_renderGraph);
}

void Application::WindowResize()
{
    // todo: handle resize
    if (fawn_vision::RecreateRenderer(m_window, m_renderer) != fawn_vision::gfx_status::ok)
    {
        std::cerr << "could not recreate renderer";
    }
}
} // namespace BalbinoApp
