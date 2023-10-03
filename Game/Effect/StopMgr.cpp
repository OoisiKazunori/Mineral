#include "StopMgr.h"
#include <algorithm>

HitStopParam::HitStopParam(int time, float speed)
{
	m_time = time;
	m_speed = speed;
}

void StopMgr::Init()
{
	m_slowTimer = 0;
	m_gameSpeed = 1.0f;

}

void StopMgr::Update()
{
	m_slowTimer = std::clamp(m_slowTimer - 1, 0, 10000);
}

void StopMgr::HitStopStart(const HitStopParam& param)
{
	m_slowTimer = param.m_time;
	m_gameSpeed = param.m_speed;
}

float StopMgr::GetGameSpeed()
{
	if (0 < m_slowTimer)
	{
		return m_gameSpeed;
	}

	return 1.0f;
}

