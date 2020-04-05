#pragma once
#include "Component.h"

namespace Balbino
{
	class Texture2D;
	class GameObject;

	class Avatar: public Component
	{
	public:
		Avatar( std::weak_ptr<GameObject> origine );

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