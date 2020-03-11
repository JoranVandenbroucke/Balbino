#pragma once
class Time
{
public:
	static Time& Get();
	static float DeltaTime();
	static float UnscaledDeltaTime();
	static float TimeScale();
private:
	Time()=default;

	float m_TimeScale;
	float m_DeltaTime;
};