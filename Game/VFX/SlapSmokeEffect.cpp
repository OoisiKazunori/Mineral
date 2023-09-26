#include "SlapSmokeEffect.h"
#include"../KazLibrary/Easing/easing.h"
#include"../KazLibrary/Imgui/MyImgui.h"

KazMath::Vec2<float> SlapSmokeEffect::s_smokeScale = { 3.0f,1.0f };
int SlapSmokeEffect::Particle::s_velRateTime = 60;
int SlapSmokeEffect::Particle::s_scaleRateTime = 60;
int SlapSmokeEffect::Particle::s_disappearMaxTime = 20;
KazMath::Vec2<float> SlapSmokeEffect::s_smokeSubRange = { 10.0f,0.0f };
KazMath::Vec2<int> SlapSmokeEffect::s_smokeInterval = { 30,-10 };


SlapSmokeEffect::SlapSmokeEffect()
{
}

void SlapSmokeEffect::Init(const KazMath::Vec3<float>& arg_emittPos, float arg_range, bool arg_isStrong)
{
	//�p�[�e�B�N���̔������ϓ��ɒu���A�c��̓����_���ɒu�����Ƃŉ~��ۂ����R�ȉ��Ɍ�����
	int circleParticleNum = (PARTICLE_MAX_NUM / 2);
	int angleVel = 360 / circleParticleNum;

	float addScale = 0.0f;
	float addRangeSub = 0.0f;
	if (arg_isStrong)
	{
		addScale = 3.0f;
		addRangeSub = s_smokeSubRange.x / 2;
	}

	//360�x�Ƀp�[�e�B�N�����΂�
	for (int i = 0; i < circleParticleNum; ++i)
	{
		//������x�p�[�e�B�N���̊Ԋu�����炷
		int angle = angleVel * i + KazMath::Rand<int>(s_smokeInterval.x, -s_smokeInterval.y);
		KazMath::Vec3<float>vel(cosf(KazMath::AngleToRadian(angle)), 0.0f, sinf(KazMath::AngleToRadian(angle)));
		//������
		m_particleArray[i].Init(arg_emittPos, vel, arg_range, KazMath::Rand<float>(4.0f + addScale, 2.0f + addScale));
	}

	for (int i = circleParticleNum; i < m_particleArray.size(); ++i)
	{
		//������x�p�[�e�B�N���̊Ԋu�����炷
		float angle = KazMath::Rand<float>(360.0f, 0.0f);
		KazMath::Vec3<float>vel(cosf(KazMath::AngleToRadian(angle)), 0.0f, sinf(KazMath::AngleToRadian(angle)));
		//������
		m_particleArray[i].Init(arg_emittPos, vel, arg_range - KazMath::Rand<float>(s_smokeSubRange.x + addRangeSub, s_smokeSubRange.y), KazMath::Rand<float>(2.0f + addScale / 2.0f, 1.0f + addScale / 2));
	}
}

void SlapSmokeEffect::Update()
{
	for (auto& obj : m_particleArray)
	{
		obj.Update();
	}
}

void SlapSmokeEffect::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{
	for (auto& obj : m_particleArray)
	{
		obj.Draw(arg_rasterize, arg_blasVec);
	}
}

void SlapSmokeEffect::DebugImGui()
{
	ImGui::Begin("Smoke");
	ImGui::DragFloat("SmokeScaleMax", &s_smokeScale.x);								//���̑傫���̍ő�l
	ImGui::DragFloat("SmokeScaleMin", &s_smokeScale.y);								//���̑傫���̍ŏ��l
	//���Ԃ̒P�ʂ�flame�œ���
	ImGui::DragInt("SmokeExpandingTime", &Particle::s_velRateTime);					//�����U���͈͂܂ōL���鎞��
	ImGui::DragInt("SmokeDisappearTime", &Particle::s_scaleRateTime);				//�����������鎞��
	ImGui::DragInt("SmokeReadyToDisappearTime", &Particle::s_disappearMaxTime);		//�����c�鎞��
	ImGui::DragFloat("RangeSubMax", &s_smokeSubRange.x);							//���͈̔͂�{�����ǂꂭ�炢���������邩�̍ő�l
	ImGui::DragFloat("RangeSubMin", &s_smokeSubRange.y);							//���͈̔͂�{�����ǂꂭ�炢���������邩�̍ŏ��l
	ImGui::DragInt("SmokeIntervalMax", &s_smokeInterval.x);
	ImGui::DragInt("SmokeIntervalMin", &s_smokeInterval.y);
	ImGui::End();
}

SlapSmokeEffect::Particle::Particle() :m_initFlag(false)
{
	m_smokeRender.LoadZAllways("Resource/VFX/smoke/", "Slap_VFX.gltf");
	m_transform.scale = { 0.0f,0.0f,0.0f };
}

void SlapSmokeEffect::Particle::Init(const KazMath::Vec3<float>& arg_emittPos, const KazMath::Vec3<float>& arg_vel, float arg_radius, float arg_scale)
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

void SlapSmokeEffect::Particle::Update()
{
	if (!m_initFlag)
	{
		return;
	}

	//���͈͂܂ōL����
	if (m_velRate < 1.0f)
	{
		float rateVel = static_cast<float>(1) / static_cast<float>(s_velRateTime);
		Rate(&m_velRate, rateVel, 1.0f);
		m_transform.pos = m_emittPos + m_vel * (m_radius * EasingMaker(Out, Exp, m_velRate));
	}
	//��ɒi�X�オ��
	if (1.0f <= m_velRate)
	{
		++m_disappearTimer;
		m_transform.pos.y += 0.01f;
	}
	//������
	if (m_disappearMaxTimer <= m_disappearTimer)
	{
		float rateVel = static_cast<float>(1) / static_cast<float>(s_scaleRateTime);
		Rate(&m_scaleRate, rateVel, 1.0f);
		float scale = m_scaleMax * (1.0f - EasingMaker(In, Sine, m_scaleRate));
		m_transform.scale = { scale,scale,scale };
	}

	//�X�P�[����0�ɂȂ�����I��
	if (m_transform.scale.x <= 0.0f)
	{
		m_transform.scale = { 0.0f,0.0f,0.0f };
		m_initFlag = false;
	}
}

void SlapSmokeEffect::Particle::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{
	if (0 < m_transform.scale.x) {
		m_smokeRender.Draw(arg_rasterize, arg_blasVec, m_transform);
	}
}
