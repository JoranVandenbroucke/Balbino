#pragma once

namespace BubbleBobble
{
	class Bub: public Balbino::GameObject
	{
	public:
		Bub() = default;

		Bub( const Bub& ) = delete;
		Bub( Bub&& ) = delete;
		Bub& operator=( const Bub& ) = delete;
		Bub& operator=( Bub&& ) = delete;

		~Bub() = default;

		virtual void Create() override;
	private:
	};
}

