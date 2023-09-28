#include "SmokeParticle.h"
#include"../KazLibrary/Easing/easing.h"

SmokeParticle::SmokeParticle() :m_initFlag(false)
{
	m_smokeRender.LoadZAllways("Resource/VFX/smoke/", "Slap_VFX.gltf");
	m_transform.scale = { 0.0f,0.0f,0.0f };
}

void SmokeParticle::Init(const KazMath::Vec3<float>& arg_emittPos, const KazMath::Vec3<float>& arg_vel, float arg_radius, float arg_scale)
{
	m_initFlag = true;
	m_emittPos = arg_emittPos;
	m_transform.pos = m_emittPos;
	m_vel = arg_vel;
	m_radius = arg_radius;
	m_scaleMax = arg_scale;
	m_transform.scale = { m_scaleMax,m_scaleMax ,m_scaleMax };
	m_velRate = 0.0f;
	m_scaleRate = 0.0f;
	m_disappearTimer = 0;

	m_disappearMaxTimer = KazMath::Rand<int>(s_disappearMaxTime + s_disappearMaxTime / 2, s_disappearMaxTime / 2);
}

void SmokeParticle::Update()
{
	if (!m_initFlag)
	{
		return;
	}

	//一定範囲まで広がる
	if (m_velRate < 1.0f)
	{
		float rateVel = static_cast<float>(1) / static_cast<float>(s_velRateTime);
		Rate(&m_velRate, rateVel, 1.0f);
		m_transform.pos = m_emittPos + m_vel * (m_radius * EasingMaker(Out, Exp, m_velRate));
	}
	//上に段々上がる
	if (1.0f <= m_velRate)
	{
		++m_disappearTimer;
		m_transform.pos.y += 0.01f;
	}
	//消える
	if (m_disappearMaxTimer <= m_disappearTimer)
	{
		float rateVel = static_cast<float>(1) / static_cast<float>(s_scaleRateTime);
		Rate(&m_scaleRate, rateVel, 1.0f);
		float scale = m_scaleMax * (1.0f - EasingMaker(In, Sine, m_scaleRate));
		m_transform.scale = { scale,scale,scale };
	}

	//スケールが0になったら終了
	if (m_transform.scale.x <= 0.0f)
	{
		m_transform.scale = { 0.0f,0.0f,0.0f };
		m_initFlag = false;
	}
}

void SmokeParticle::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{
	m_smokeRender.Draw(arg_rasterize, arg_blasVec, m_transform);
}
