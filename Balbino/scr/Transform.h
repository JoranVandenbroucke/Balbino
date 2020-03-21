#pragma once
#include "Core.h"
#pragma warning(push)
#pragma warning(disable:4251)

namespace Balbino
{
	class BALBINO_API Transform final
	{
	public:
		const vec3& GetPosition() const { return m_Position; }
		void SetPosition(float x, float y, float z);
	private:
		vec3 m_Position;
	};
}
#pragma warning(pop)