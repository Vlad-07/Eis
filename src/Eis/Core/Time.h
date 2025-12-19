#pragma once

#include <chrono>
#include "Eis/Core/Log.h"


namespace Eis
{
	// Feels shady mixing usings with a class

	using Clock = std::chrono::steady_clock;
	using TimePoint = std::chrono::time_point<Clock>;
	using ChronoDuration = std::chrono::nanoseconds;
	using ChronoDurationDouble = std::chrono::duration<double>;


	class Duration
	{
	public:
		static Duration FromSec(double sec) { return Duration(sec); }
		static Duration FromMs(double ms) { return Duration(std::chrono::duration_cast<ChronoDuration>(std::chrono::duration<double, std::milli>(ms))); }
		static Duration FromHz(double hz) { return Duration(hz != 0.0 ? 1.0 / hz : 0.0); } // From FPS

	public:
		Duration(ChronoDuration d) : m_Duration(d) {}
		Duration(double sec = 0.0) : m_Duration(CastSec(sec)) {}

		double GetSeconds() const { return ToDoubleSec(m_Duration).count(); }
		double GetMilliseconds() const { return std::chrono::duration<double, std::milli>(m_Duration).count(); }

		operator double() const { return GetSeconds(); }

		const Duration& operator=(const ChronoDuration& other) { m_Duration = other; return *this; }
		const Duration& operator=(const Duration& other) { m_Duration = other.m_Duration; return *this; }

		// will implement operators as needed

		Duration operator+(const Duration& other) const { return Duration(m_Duration + other.m_Duration); }
		Duration operator-(const Duration& other) const { return Duration(m_Duration - other.m_Duration); }

		void operator+=(const Duration& other) { m_Duration += other.m_Duration; }
		void operator-=(const Duration& other) { m_Duration -= other.m_Duration; }

		bool operator>(const Duration& other) const { return m_Duration > other.m_Duration; }
		bool operator<(const Duration& other) const { return m_Duration < other.m_Duration; }
		bool operator>=(const Duration& other) const { return m_Duration >= other.m_Duration; }
		bool operator<=(const Duration& other) const { return m_Duration <= other.m_Duration; }

		ChronoDuration GetChronoDuration() const { return m_Duration; }

	private:
		static ChronoDuration CastSec(double sec)
		{ return std::chrono::duration_cast<ChronoDuration>(ChronoDurationDouble(sec)); }

		static ChronoDurationDouble ToDoubleSec(ChronoDuration d)
		{ return std::chrono::duration_cast<ChronoDurationDouble>(d); }

	private:
		ChronoDuration m_Duration; // sec
	};


	class Time
	{
	public:
		Time() = delete;
		Time(const Time&) = delete;
		Time& operator=(const Time&) = delete;
		~Time() = default;

		static TimePoint Now() { return Clock::now(); }
		static TimePoint GetFrameStart() { return s_FrameStart; }

		static Duration GetDeltaTime() { return s_DeltaTime; }
		static Duration GetUncappedDeltaTime() { return s_UncappedDeltaTime; }
		static Duration GetFixedDeltaTime() { return s_FixedDeltaTime; }

		static Duration GetMaxDeltaTime() { return s_MaxDeltaTime; }

		static void SetFixedDeltaTime(Duration d) { s_FixedDeltaTime = d; }
		static void SetMaxDeltaTime(Duration d) { EIS_CORE_ASSERT(d > s_FixedDeltaTime, "MaxDeltaTime must be larger than FixedDeltaTime!"); s_MaxDeltaTime = d; }

	private:
		static void Init();
		static void FrameStart();
		static bool ShouldRunFixedUpdate(); // FixedUpdate MUST be called when this returns true

		friend class Application;

	private:
		static Duration s_DeltaTime;
		static Duration s_UncappedDeltaTime;
		static Duration s_FixedDeltaTime;
		static Duration s_MaxDeltaTime;

		static TimePoint s_FrameStart;

		static Duration s_FixedUpdateAccumulator;
	};
}