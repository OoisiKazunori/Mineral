#include "Transition.h"
#include "../KazLibrary/Easing/easing.h"

void Transition::Setting()
{

	m_transitionUI.Load("Resource/Title/Transition.png");
	m_transitionUI.m_transform.pos = { 1280.0f / 2.0f, 720.0f / 2.0f };
	m_transitionUI.m_transform.scale = { 1280.0f, 720.0f };

}

void Transition::Init()
{
	m_isActive = false;
	m_isFinish = false;
	m_status = STATUS::APPEAR;
	m_easingTimer = 0.0f;
}

void Transition::Update()
{

	if (!m_isActive) return;

	switch (m_status)
	{
	case Transition::STATUS::APPEAR:
	{

		m_easingTimer = std::clamp(m_easingTimer + 1.0f, 0.0f, APPEAR_TIME);

		//イージングをかけてタイマーを減らす。
		float easingAmount = EasingMaker(Out, Sine, m_easingTimer / APPEAR_TIME);

		//アルファを減らす。
		m_transitionUI.m_color.color.a = static_cast<int>(easingAmount * 255);

		if (APPEAR_TIME <= m_easingTimer) {

			m_easingTimer = 0.0f;
			m_status = STATUS::STAY;

		}
	}
		break;
	case Transition::STATUS::STAY:
	{

		m_easingTimer = std::clamp(m_easingTimer + 1.0f, 0.0f, STAY_TIME);

		if (STAY_TIME <= m_easingTimer) {

			m_easingTimer = 0.0f;
			m_status = STATUS::EXIT;

		}
	}
		break;
	case Transition::STATUS::EXIT:
	{

		m_easingTimer = std::clamp(m_easingTimer + 1.0f, 0.0f, EXIT_TIME);

		//イージングをかけてタイマーを減らす。
		float easingAmount = EasingMaker(In, Sine, m_easingTimer / EXIT_TIME);

		//アルファを減らす。
		m_transitionUI.m_color.color.a = static_cast<int>((1.0f - easingAmount) * 255);

		if (EXIT_TIME <= m_easingTimer) {

			m_isActive = false;
			m_isFinish = true;

		}
	}
		break;
	default:
		break;
	}
}

void Transition::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{
	if (!m_isActive)return;
	m_transitionUI.Draw(arg_rasterize);
}

void Transition::Activate()
{
	m_isActive = true;
	m_status = STATUS::APPEAR;
	m_easingTimer = 0.0f;
}
