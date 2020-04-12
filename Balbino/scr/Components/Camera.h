#pragma once
#pragma warning(push)
#pragma warning(disable:4201)
#pragma warning(disable:4127)
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#pragma warning(pop)
#include "Component.h"
#include <memory>
#include "../Struct.h"

namespace Balbino
{
	class GameObject;
	class Camera : public Component
	{
	public:
		Camera( const std::weak_ptr<GameObject> origine, float fov, float w, float h );
			
		const glm::mat4& GetViewProjection() const;

		void Update();
		void Translate(const vec3& v);
	private:
		glm::mat4 m_Projection;
		glm::mat4 m_View;
		glm::mat4 m_ViewProjection;

	};
}