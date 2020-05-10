#pragma once
#include "Component.h"
#include "../Struct.h"
#include <memory>
#include <vector>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace Balbino
{
	class GameObject;

	class Transform final : public Component, public std::enable_shared_from_this<Transform>
	{
	public:
		explicit Transform( const GameObject* const origine );

		virtual ~Transform();

		const Vector3& GetPosition() const { return m_Position; }
		const Vector3& GetRotation() const { return m_Rotation; }
		const Vector3& GetScale() const { return m_Scale; }
		void SetPosition(float x, float y, float z);
		void SetRotation(float x, float y, float z);
		void SetScale(float x, float y, float z);

		virtual void Create() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Draw() const override;

		virtual void Save( std::ostream& file )override;
		virtual void Load( std::istream& file )override;
		
		void SetParrent( Transform* parent);
		int GetNumberOfChilderen();
		void RemoveChild(int index);
		void DestroyChilderen();
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
		Vector3 m_Position;
		Vector3 m_Rotation;
		Vector3 m_Scale;

		Transform* m_Parent;

		std::vector<Transform*> m_pChilderen;

	};
}