#include "Eispch.h"
#include "Time.h"


namespace Eis
{
	Duration Time::s_DeltaTime{};
	Duration Time::s_UncappedDeltaTime{};
	Duration Time::s_FixedDeltaTime = Duration::FromHz(50); // Default FixedUpdate is 50hz
	Duration Time::s_MaxDeltaTime = Time::s_FixedDeltaTime * 5.0; // Limit to 5 fixed updates

	TimePoint Time::s_FrameStart;

	Duration Time::s_FixedUpdateAccumulator{};

	void Time::Init()
	{
		s_FrameStart = Clock::now(); // smooth the first frame
	}

	void Time::FrameStart()
	{
		const TimePoint now = Clock::now();
		s_UncappedDeltaTime = now - s_FrameStart;
		s_FrameStart = now;

		// TODO: warn when falling behind. ideally in a perf system
		s_DeltaTime = std::min(s_UncappedDeltaTime, s_MaxDeltaTime);

		s_FixedUpdateAccumulator += s_DeltaTime;
	}

	bool Time::ShouldRunFixedUpdate()
	{
		if (s_FixedUpdateAccumulator >= s_FixedDeltaTime)
		{
			s_FixedUpdateAccumulator -= s_FixedDeltaTime;
			return true;
		}
		return false;
	}
}