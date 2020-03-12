#pragma once
namespace Balbino
{
	class Time
	{
	public:
		static Time& Get();
		static float DeltaTime();
		static float UnscaledDeltaTime();
		static float TimeScale();

		void SetDT( float dt );
	private:
		Time() = default;

		float m_TimeScale = 1.f;
		float m_DeltaTime = 1.f;
	};
}