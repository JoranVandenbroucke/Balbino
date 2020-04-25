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
		Avatar( std::weak_ptr<GameObject> origine );

		Avatar( const Avatar& ) = delete;
		Avatar( Avatar&& ) = delete;
		Avatar& operator=( const Avatar& ) = delete;
		Avatar& operator=( Avatar&& ) = delete;

		~Avatar() = default;

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
		std::weak_ptr<ConsoleAudio> m_ConsoleAudio;
		std::weak_ptr<LoggedAudio> m_LoggedAudio;
	};
}