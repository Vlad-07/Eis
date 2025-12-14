#include "Eispch.h"
#include "Time.h"


namespace Eis
{
	Duration Time::s_DeltaTime = Duration::FromHz(60); // smooth the first frame
	Duration Time::s_FixedDeltaTime = Duration::FromHz(50); // Default FixedUpdate is 50hz
	Duration Time::s_MaxDeltaTime = Time::s_FixedDeltaTime * 5.0f; // Limit to 5 fixed updates

	TimeTypes::TimePoint Time::s_FrameStart;

	Duration Time::s_FixedUpdateAccumulator{};

	void Time::Init()
	{
		s_FrameStart = TimeTypes::Clock::now();
	}

	void Time::FrameStart()
	{
		const TimeTypes::TimePoint now = TimeTypes::Clock::now();
		s_DeltaTime = now - s_FrameStart;
		s_FrameStart = now;

		s_FixedUpdateAccumulator += std::min(s_DeltaTime, s_MaxDeltaTime);
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