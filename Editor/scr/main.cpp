#include "pch.h"

namespace Dear
{
	class Editor final : public Balbino::Application
	{
	public:
		Editor() = default;
		virtual ~Editor() override = default;
		Editor( const Editor& ) = delete;
		Editor( Editor&& ) = delete;
		Editor& operator=( const Editor& ) = delete;
		Editor& operator=( Editor&& ) = delete;

		virtual void LoadGame() const override;
	};
	void Editor::LoadGame() const
	{
		this->Application::LoadGame();
	}
}


Balbino::Application* Balbino::CreateApplication()
{
	return new Dear::Editor{};
}
