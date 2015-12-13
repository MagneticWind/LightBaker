#include <Windows.h>
#include "Timer.h"

//------------------------------------------------------------------
Timer::Timer() : m_bIsRunning(false), m_i64StartTicks(0), m_i64StopTicks(0), m_i64CurrentTicks(0)
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_i64TicksPerSecond);
}

//------------------------------------------------------------------
void Timer::Start()
{
	m_bIsRunning = true;
	QueryPerformanceCounter((LARGE_INTEGER*)&m_i64StartTicks);
}

//------------------------------------------------------------------
void Timer::Stop()
{
	m_bIsRunning = false;
	QueryPerformanceCounter((LARGE_INTEGER*)&m_i64StopTicks);
}

//------------------------------------------------------------------
void Timer::Reset()
{
	m_bIsRunning = false;
	m_i64TicksPerSecond = 0;
	m_i64StartTicks = 0;
	m_i64StopTicks = 0;
	m_i64CurrentTicks = 0;
}

//------------------------------------------------------------------
float Timer::GetElapsedSeconds()
{
	//assert(m_bIsRunning);
	QueryPerformanceCounter((LARGE_INTEGER*)&m_i64CurrentTicks);
	__int64 i64Delta = m_i64CurrentTicks - m_i64StartTicks;
	double dDelta = static_cast<double>(i64Delta) / static_cast<double>(m_i64TicksPerSecond);
	return static_cast<float>(dDelta);
}