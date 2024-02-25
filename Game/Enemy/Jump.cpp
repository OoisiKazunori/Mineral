#include "Jump.h"

Jump::Jump() :m_gravity(0.0f), m_jumpVel(0.0f), m_activeFlag(false)
{
}

void Jump::Active()
{
	m_activeFlag = true;
	m_gravity = 0.0f;
	m_jumpVel = 0.0f;
}

void Jump::Finalize()
{
	m_activeFlag = false;
}

float Jump::Update()
{
	if (!m_activeFlag)
	{
		return 0.0f;
	}
	//ジャンプ力
	m_jumpVel += 1.1f;
	//落下する
	m_jumpVel += -m_gravity;
	//重力加算
	m_gravity += 0.1f;
	return m_jumpVel;
}
