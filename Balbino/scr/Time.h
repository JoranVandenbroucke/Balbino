#pragma once
#include "Singleton.h"

namespace Balbino
{
	class BTime: public Singleton<BTime>
	{
	public:
		static float DeltaTime();
		static float FixedDeltaTime();
		static float FixedTime();
		static float MaximumDeltaTime();
		static float Time();
		static float TimeScale();
		static float TimeSinceLevelLoad();
		static float UnscaledFixedDeltaTime();
		static float UnscaledFixedTime();
		static float UnscaledDeltaTime();
		static float UnscaledTime();

		void SetDT( float dt );
		void SetTS( float ts );
		void UpdateFT();
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
		float m_FixedDeltaTime = 1.f/60.f;
		float m_TimeScale = 1.f;
		float m_Time = 0.f;
		float m_FixedTime = 0.f;
		float m_UnscaledTime = 0.f;
		float m_UnscaledFixedTime = 0.f;
		float m_TimeSinceLevelLoad = 0.f;
	};
}