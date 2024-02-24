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
	//ƒWƒƒƒ“ƒv—Í
	m_jumpVel += 1.0f;
	//—‰º‚·‚é
	m_jumpVel += -m_gravity;
	//d—Í‰ÁZ
	m_gravity += 0.1f;
	return m_jumpVel;
}
