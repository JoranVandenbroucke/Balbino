#pragma once
#include "Core.h"
#pragma warning(push)
#pragma warning(disable:4251)

namespace Balbino
{
	class Texture2D;
	class BALBINO_API Avatar
	{
	public:
		Avatar() = default;

		Avatar( const Avatar& ) = delete;
		Avatar( Avatar&& ) = delete;
		Avatar& operator=( const Avatar& ) = delete;
		Avatar& operator=( Avatar&& ) = delete;

		~Avatar() = default;

		void Draw() const;

		void Fire();
		void Duck();
		void Jump();
		void Fart();
	private:
		std::shared_ptr<Texture2D> m_Texture{};
	};
}
#pragma warning(pop)