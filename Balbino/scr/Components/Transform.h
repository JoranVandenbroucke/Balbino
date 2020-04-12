#pragma once
#include "Component.h"
#include "../Core.h"
#include "../Struct.h"
#pragma warning(push)
#pragma warning(disable:4201)
#pragma warning(disable:431)
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#pragma warning(pop)
#pragma warning(push)
#pragma warning(disable:4251)
#pragma warning(disable:4275)

namespace Balbino
{
	class GameObject;

	class BALBINO_API Transform final : public Component
	{
	public:
		explicit Transform( const std::weak_ptr<GameObject> origine );
		virtual ~Transform();

		const vec3& GetPosition() const { return m_Position; }
		const vec3& GetRotation() const { return m_Rotation; }
		const vec3& GetScale() const { return m_Scale; }
		void SetPosition(float x, float y, float z);
		void SetRotation(float x, float y, float z);
		void SetScale(float x, float y, float z);

		virtual void Create();
		virtual void Update() override;
		virtual void Draw() const override;

		glm::mat4x4 myModelMatrix{ 1.f };

#ifdef _DEBUG
		virtual void DrawInpector() override;
#endif // _DEBUG

		glm::vec4 m_OriginalPoints{ 1.f };

		Transform( const Transform& ) = delete;
		Transform( Transform&& ) = delete;
		Transform& operator= ( const Transform& ) = delete;
		Transform& operator= ( const Transform&& ) = delete;
	private:
		vec3 m_Position;
		vec3 m_Rotation;
		vec3 m_Scale;

	};
}
#pragma warning(pop)