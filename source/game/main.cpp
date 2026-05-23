#include <balbino.hpp>

namespace dear
{
class Game final : public BalbinoApp::Application
{
  public:
    Game()                               = default;
    ~Game() override                     = default;
    Game(const Game&)                    = delete;
    Game(Game&&)                         = delete;
    auto operator=(const Game&) -> Game& = delete;
    auto operator=(Game&&) -> Game&      = delete;

    void LoadGame() override;
};

void Game::LoadGame()
{
    this->Application::LoadGame();
}
} // namespace dear

auto BalbinoApp::CreateApplication() -> BalbinoApp::Application*
{
    return new dear::Game{};
}

void BalbinoApp::DestroyApplication(Application* pApplication)
{
    delete pApplication;
}
