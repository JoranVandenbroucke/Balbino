#include <Balbino.h>

#include "Versions.hpp"

namespace Dear
{
    class Game final : public BalbinoApp::Application
    {
    public:
        Game()                         = default;
        ~Game() override               = default;
        Game( const Game& )            = delete;
        Game( Game&& )                 = delete;
        Game& operator=( const Game& ) = delete;
        Game& operator=( Game&& )      = delete;

        void LoadGame() override;
    };

    void Game::LoadGame()
    {
        this->Application::LoadGame();
    }
}// namespace Dear

BalbinoApp::Application* BalbinoApp::CreateApplication()
{
    return new Dear::Game {};
}

void BalbinoApp::DestroyApplication( Application* pApplication )
{
    delete dynamic_cast<Dear::Game*>( pApplication );
}
