#include "BalbinoPCH.h"
#include "Time.h"
#include <algorithm>

using namespace Balbino;
BTime& BTime::Get()
{
	static BTime time{};
	return time;
}

float BTime::DeltaTime()
{
	return ( std::min )( Get().m_DeltaTime * Get().m_TimeScale, Get().m_MaximumDeltaTime );
}

float Balbino::BTime::MaximumDeltaTime()
{
	return Get().m_MaximumDeltaTime;
}

float Balbino::BTime::Time()
{
	return Get().m_Time;
}

float BTime::UnscaledDeltaTime()
{
	return ( std::min )( Get().m_DeltaTime, Get().m_MaximumDeltaTime );
}

float Balbino::BTime::UnscaledTime()
{
	return Get().m_UnscaledTime;
}

float BTime::TimeScale()
{
	return Get().m_TimeScale;
}

float Balbino::BTime::TimeSinceLevelLoad()
{
	return Get().m_TimeSinceLevelLoad;
}

void Balbino::BTime::SetDT( float dt )
{
	m_DeltaTime = dt;
	m_Time += dt * m_TimeScale;
	m_UnscaledTime += dt;
}

void Balbino::BTime::SetTS( float ts )
{
	m_TimeScale = ts;
}

void Balbino::BTime::LoadLevel()
{
	m_TimeSinceLevelLoad = 0.f;
}
