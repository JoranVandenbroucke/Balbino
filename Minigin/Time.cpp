#include "MiniginPCH.h"
#include "Time.h"
using namespace Balbino;
Time& Time::Get()
{
	static Time time{};
	return time;
}

float Time::DeltaTime()
{
	return Get().m_DeltaTime * Get().m_TimeScale;
}

float Time::UnscaledDeltaTime()
{
	return Get().m_DeltaTime;
}

float Time::TimeScale()
{
	return Get().m_TimeScale;
}

void Balbino::Time::SetDT( float dt )
{
	m_DeltaTime = dt;
}
