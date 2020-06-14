#pragma once
#include "Component.h"

namespace Balbino
{
	class GameObject;
	class Transform;
	class Text;

	class FPSScript final: public Component
	{
	public:
		explicit FPSScript( const GameObject* const origine );
		virtual ~FPSScript() = default;

		FPSScript( const FPSScript& ) = delete;
		FPSScript( FPSScript&& ) = delete;
		FPSScript& operator= ( const FPSScript& ) = delete;
		FPSScript& operator= ( const FPSScript&& ) = delete;

		virtual void Create() override;
		virtual void Update() override;
		virtual void Draw() const override;

		virtual void Save( std::ostream& file )override;
		virtual void Load( std::istream& file )override;

#ifdef BALBINO_DEBUG
		virtual void DrawInpector() override;
#endif // BALBINO_DEBUG

	private:
		Text* m_Text{};

	};
}