#pragma once

namespace BubbleBobble
{
	class DAELogo: public Balbino::GameObject
	{
	public:
		virtual void Create() override;

		DAELogo() = default;
		virtual ~DAELogo() = default;
		DAELogo( const DAELogo& other ) = delete;
		DAELogo( DAELogo&& other ) = delete;
		DAELogo& operator=( const DAELogo& other ) = delete;
		DAELogo& operator=( DAELogo&& other ) = delete;
	protected:
	};
}