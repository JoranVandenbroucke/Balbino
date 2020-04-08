#pragma once
#include "Component.h"
#include "../Core.h"
#include "../Struct.h"
#pragma warning(push)
//#pragma warning(disable:4251)
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
		void SetPosition(float x, float y, float z);

		virtual void Create();
		virtual void Update() override;
		virtual void Draw() const override;
#ifdef _DEBUG
		virtual void DrawInpector() const override;
#endif // _DEBUG


		Transform( const Transform& ) = delete;
		Transform( Transform&& ) = delete;
		Transform& operator= ( const Transform& ) = delete;
		Transform& operator= ( const Transform&& ) = delete;
	private:
		vec3 m_Position;
	};
}
#pragma warning(pop)