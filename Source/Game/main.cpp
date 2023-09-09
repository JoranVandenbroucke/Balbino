#include <vector>
namespace Dear
{
//    class ComponentFactory {
//    public:
//        static Component* createComponent(const uuid_t& uuid) {
//            // Simulate creation based on UUID, you can extend this with more components
//            if (uuid_compare(uuid, PositionComponentUUID) == 0) {
//                return new PositionComponent();
//            }
//            if (uuid_compare(uuid, RenderComponentUUID) == 0) {
//                return new RenderComponent();
//            }
//            return nullptr;
//        }
//
//    private:
//        static uuid_t PositionComponentUUID;
//        static uuid_t RenderComponentUUID;
//    };

    class Editor final : public BalbinoApp::Application
    {
    public:
        Editor()                           = default;
        ~Editor() override                 = default;
        Editor( const Editor& )            = delete;
        Editor( Editor&& )                 = delete;
        Editor& operator=( const Editor& ) = delete;
        Editor& operator=( Editor&& )      = delete;

        void LoadGame() override;
    };

    void Editor::LoadGame()
    {

        this->Application::LoadGame();
    }
}// namespace Dear

BalbinoApp::Application* BalbinoApp::CreateApplication()
{
    return new Dear::Editor {};
}

void BalbinoApp::DestroyApplication( BalbinoApp::Application* pApplication )
{
    delete (Dear::Editor*)pApplication;
}
