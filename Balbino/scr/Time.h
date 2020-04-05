#pragma once
#include "Singleton.h"

namespace Balbino
{
	class BTime: public Singleton<BTime>
	{
	public:
		static float DeltaTime();
		static float MaximumDeltaTime();
		static float Time();
		static float TimeScale();
		static float TimeSinceLevelLoad();
		static float UnscaledDeltaTime();
		static float UnscaledTime();

		void SetDT( float dt );
		void SetTS( float ts );
		void LoadLevel();

		~BTime() = default;
		BTime( BTime& ) = delete;
		BTime( BTime&& ) = delete;
		BTime& operator=( BTime& ) = delete;
		BTime& operator=( BTime&& ) = delete;

	private:
		BTime() = default;
		friend class Singleton<BTime>;

		const float m_MaximumDeltaTime = 1.f;
		float m_DeltaTime = 1.f;
		float m_TimeScale = 1.f;
		float m_Time = 0.f;
		float m_UnscaledTime = 0.f;
		float m_TimeSinceLevelLoad = 0.f;
	};
}