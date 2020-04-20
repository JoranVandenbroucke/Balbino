#pragma once
#include "Component.h"
#include "../Struct.h"
#include "../Core.h"
#include <memory>
#pragma warning(push)
#pragma warning(disable:4251)
#pragma warning(disable:4275)
#pragma warning(push)
#pragma warning(disable:4201)
#pragma warning(disable:4127)
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#pragma warning(pop)

namespace Balbino
{
	class GameObject;

	class BALBINO_API Camera final: public Component
	{
	public:
		Camera( const std::weak_ptr<GameObject> origine, float fov, float w );
		virtual ~Camera()=default;

		virtual void Create() override;
		virtual void Update() override;
		virtual void Draw() const override;

		virtual void Save( std::ostream& file )override;
		virtual void Load( std::istream& file )override;


		Camera( const Camera& ) = delete;
		Camera( Camera&& ) = delete;
		Camera& operator= ( const Camera& ) = delete;
		Camera& operator= ( const Camera&& ) = delete;

#ifdef _DEBUG
		virtual void DrawInpector() override;
#endif // _DEBUG
	private:
		bool m_IsOrtographic;
		float m_OrthographicSize;
		float m_FieldOfView;
		float m_Depth;
	};
}
#pragma warning(pop)