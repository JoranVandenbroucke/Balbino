#pragma once

namespace BubbleBobble
{
	class Balbino::Texture2D;
	class Bub: public Balbino::SceneObject
	{
	public:
		Bub() = default;

		Bub( const Bub& ) = delete;
		Bub( Bub&& ) = delete;
		Bub& operator=( const Bub& ) = delete;
		Bub& operator=( Bub&& ) = delete;

		~Bub() = default;

		virtual void Create() override;
		virtual void Update() override;
		virtual void Draw() const override;
	private:
		Balbino::Avatar m_Avatar;
	};
}

