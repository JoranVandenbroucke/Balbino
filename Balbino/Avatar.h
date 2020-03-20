#pragma once
#include "SceneObject.h"

namespace Balbino
{
	class Texture2D;
	class Avatar: public SceneObject
	{
	public:
		Avatar() = default;

		Avatar( const Avatar& ) = delete;
		Avatar( Avatar&& ) = delete;
		Avatar& operator=( const Avatar& ) = delete;
		Avatar& operator=( Avatar&& ) = delete;

		~Avatar() = default;

		virtual void Create() override;
		virtual void Update() override;
		virtual void Draw() const override;


		void Fire();
		void Duck();
		void Jump();
		void Fart();

	private:
		std::shared_ptr<Texture2D> m_Texture{};
	};
}

