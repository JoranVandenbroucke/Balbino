#pragma once
#include "Component.h"

namespace Balbino
{
	class ConsoleAudio;
	class LoggedAudio;
	class GameObject;

	class Avatar: public Component
	{
	public:
		explicit Avatar( const GameObject* const origine );
		virtual ~Avatar() = default;

		Avatar( const Avatar& ) = delete;
		Avatar( Avatar&& ) = delete;
		Avatar& operator=( const Avatar& ) = delete;
		Avatar& operator=( Avatar&& ) = delete;

		virtual void Create() override;
		virtual void Update() override;
		virtual void Draw() const override;

		virtual void Save( std::ostream& file )override;
		virtual void Load( std::istream& file )override;
#ifdef _DEBUG
		virtual void DrawInpector() override;
#endif // _DEBUG

		void Fire();
		void Duck();
		void Jump();
		void Fart();
	private:
		ConsoleAudio* m_ConsoleAudio;

		LoggedAudio* m_LoggedAudio;

	};
}