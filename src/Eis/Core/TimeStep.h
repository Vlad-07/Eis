#pragma once


namespace Eis
{
	class TimeStep
	{
	public:
		TimeStep(float time = 0.0f)	: m_Time(time) {}

		operator float() { return GetSeconds(); }

		float GetSeconds() const { return m_Time; }
		float GetMilliseconds() const { return m_Time * 1000.0f; }

	private:
		float m_Time;
	};
}