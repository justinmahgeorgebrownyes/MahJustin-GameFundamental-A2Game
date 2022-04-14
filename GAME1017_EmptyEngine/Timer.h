#pragma once
#ifndef _TIMER_H_
#define _TIMER_H_
#include <string>

#endif // !#define _TIMER_H_


class Timer
{
private:
	bool m_started, m_running, m_hasChanged;
	int m_elapsed, m_elapsedTotal;
	time_t m_startTime, m_lastTime, m_currTime;

public:
	Timer();
	void Start();
	void Pause();
	void Resume();
	void Update();
	std::string GetTime();
	bool HasChanged();



};

