namespace Dear
{
    class Editor final : public Balbino::Application
    {
    public:
        Editor() = default;
        ~Editor() override = default;
        Editor( const Editor& ) = delete;
        Editor( Editor&& ) = delete;
        Editor& operator=( const Editor& ) = delete;
        Editor& operator=( Editor&& ) = delete;
        
        void LoadGame() override;
    };
    
    void Editor::LoadGame()
    {
        this->Application::LoadGame();
    }
}

Balbino::Application* Balbino::CreateApplication()
{
    return new Dear::Editor{};
}

void Balbino::DestroyApplication( Balbino::Application* pApplication )
{
    delete (Dear::Editor*) pApplication;
}
