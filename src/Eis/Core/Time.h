#pragma once

#include <chrono>
#include "Eis/Core/Log.h"


namespace Eis
{
	namespace TimeTypes
	{
		using Clock = std::chrono::steady_clock;
		using TimePoint = std::chrono::time_point<Clock>;
		using ChronoDuration = std::chrono::duration<float>;
	}

	class Duration
	{
	public:
		static Duration FromSec(float sec) { return Duration(sec); }
		static Duration FromMs(float ms) { return Duration(ms * 0.001f); }
		static Duration FromHz(float hz) { return Duration(hz != 0.0f ? 1.0f / hz : 0.0f); } // From FPS

	public:
		Duration(TimeTypes::ChronoDuration d) : m_Duration(d) {}
		Duration(float sec = 0.0f) : m_Duration(sec) {}

		float GetSeconds() const { return m_Duration.count(); }
		float GetMilliseconds() const { return m_Duration.count() * 1000.0f; }

		operator float() const { return GetSeconds(); }
		void operator=(TimeTypes::ChronoDuration other) { m_Duration = other; }
		void operator=(Duration other) { m_Duration = other.m_Duration; }

		// will implement operators as needed

		Duration operator*(float d) { return Duration(m_Duration * d); }

		void operator+=(const Duration& other) { m_Duration += other.m_Duration; }
		void operator-=(const Duration& other) { m_Duration -= other.m_Duration; }

		bool operator>=(const Duration& other) { return m_Duration >= other.m_Duration; }

		TimeTypes::ChronoDuration GetChronoDuration() const { return m_Duration; }

	private:
		TimeTypes::ChronoDuration m_Duration; // sec
	};


	class Time
	{
	public:
		Time() = delete;
		Time(const Time&) = delete;
		Time& operator=(const Time&) = delete;
		~Time() = default;

		static TimeTypes::TimePoint Now() { return TimeTypes::Clock::now(); }
		static TimeTypes::TimePoint GetLastTimePoint() { return s_FrameStart; }

		static Duration GetDeltaTime() { return s_DeltaTime; }
		static Duration GetFixedDeltaTime() { return s_FixedDeltaTime; }

		static Duration GetMaxDeltaTime() { return s_MaxDeltaTime; }

		static void SetFixedDeltaTime(Duration d) { s_FixedDeltaTime = d; }
		static void SetMaxDeltaTime(Duration d) { EIS_ASSERT(d > s_FixedDeltaTime, "MaxDeltaTime must be larger than FixedDeltaTime!"); s_MaxDeltaTime = d; }

	private:
		static void Init();
		static void FrameStart();
		static bool ShouldRunFixedUpdate(); // FixedUpdate MUST be called when this returns true

		friend class Application;

	private:
		static Duration s_DeltaTime;
		static Duration s_FixedDeltaTime;
		static Duration s_MaxDeltaTime;

		static TimeTypes::TimePoint s_FrameStart;

		static Duration s_FixedUpdateAccumulator;
	};
}