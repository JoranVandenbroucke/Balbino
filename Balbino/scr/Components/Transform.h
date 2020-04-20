#pragma once
#include "Component.h"
#include "../Struct.h"
#include "../Core.h"
#include <memory>
#include <vector>
#include <fstream>
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

	class BALBINO_API Transform final : public Component, public std::enable_shared_from_this<Transform>
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

		virtual void Create() override;
		virtual void Update() override;
		virtual void Draw() const override;

		virtual void Save( std::ostream& file )override;
		virtual void Load( std::istream& file )override;
		
		void SetParrent(std::shared_ptr<Transform> parent);
		int GetNumberOfChilderen();
		void RemoveChild(int index);

		glm::mat4x4 TransfomationMatrix{ 1.f };

#ifdef _DEBUG
		virtual void DrawInpector() override;
		virtual void DrawHierarchy();
#endif // _DEBUG

		Transform( const Transform& ) = delete;
		Transform( Transform&& ) = delete;
		Transform& operator= ( const Transform& ) = delete;
		Transform& operator= ( const Transform&& ) = delete;
	private:
		vec3 m_Position;
		vec3 m_Rotation;
		vec3 m_Scale;

		std::shared_ptr<Transform> m_Parent;
		std::vector<std::shared_ptr<Transform>> m_Childeren;
	};
}
#pragma warning(pop)