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
		Camera( const std::weak_ptr<GameObject> origine, float fov, float w, float h );
		virtual ~Camera()=default;
		const glm::mat4& GetViewProjection() const;

		virtual void Create() override;
		virtual void Update() override;
		virtual void Draw() const override;
		void Translate(const vec3& v);

		Camera( const Camera& ) = delete;
		Camera( Camera&& ) = delete;
		Camera& operator= ( const Camera& ) = delete;
		Camera& operator= ( const Camera&& ) = delete;

#ifdef _DEBUG
		virtual void DrawInpector() override;
#endif // _DEBUG
	private:
		glm::mat4 m_Projection;
		glm::mat4 m_View;
		glm::mat4 m_ViewProjection;
	};
}
#pragma warning(pop)