#pragma once

namespace Hazzel {
	class TimeStep {
	public:
		TimeStep(float time = 0.0f) 
			: m_Time(time)
		{

		}

		operator float() const { return m_Time; }

		inline float GetSeconds() const { return m_Time; }
		inline float GetMilliseconds() const { return m_Time * 1000.0f; }

	private:
		float m_Time;
	};
}