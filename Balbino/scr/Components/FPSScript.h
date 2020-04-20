#pragma once
#include "Component.h"
#include "../Core.h"
#pragma warning(push)
#pragma warning(disable:4251)
#pragma warning(disable:4275)

namespace Balbino
{
	class GameObject;
	class Transform;
	class Text;

	class BALBINO_API FPSScript final: public Component
	{
	public:
		explicit FPSScript( const std::weak_ptr<GameObject> origine );
		virtual ~FPSScript() = default;

		virtual void Create() override;
		virtual void Update() override;
		virtual void Draw() const override;

		virtual void Save( std::ostream& file )override;
		virtual void Load( std::istream& file )override;

#ifdef _DEBUG
		virtual void DrawInpector() override;
#endif // _DEBUG

		FPSScript( const FPSScript& ) = delete;
		FPSScript( FPSScript&& ) = delete;
		FPSScript& operator= ( const FPSScript& ) = delete;
		FPSScript& operator= ( const FPSScript&& ) = delete;
	private:
		std::weak_ptr<Text> m_Text{};
	};
}
#pragma warning(pop)