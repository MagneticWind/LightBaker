#ifndef TIMER_H
#define TIMER_H

class Timer
{
public:
	Timer();

	void Start();
	void Stop();
	void Reset();
	bool IsRunning();
	float GetElapsedSeconds();
	float FrameRate();

private:
	bool m_bIsRunning;
	__int64 m_i64TicksPerSecond;
	__int64 m_i64StartTicks;
	__int64 m_i64StopTicks;
	__int64 m_i64CurrentTicks;
};

#endif