#include "ShockWave.h"
#include <Easing/easing.h>
#include <algorithm>

void ShockWave::Setting()
{
	for (auto& index : m_shockWave) {
		index.m_isActive = false;
	}
	m_shockWaveParamData = KazBufferHelper::SetConstBufferData(sizeof(ShockWaveData) * 8);
	m_shockWaveParamData.bufferWrapper->TransData(m_shockWave.data(), sizeof(ShockWaveData) * 8);
}

void ShockWave::Generate(KazMath::Vec3<float> arg_generatePos)
{

	for (int index = 0; index < 8; ++index) {

		if (m_shockWave[index].m_isActive) continue;

		m_shockWave[index].m_pos = arg_generatePos;
		m_shockWaveEasingTimer[index] = 0.0f;
		m_shockWave[index].m_isActive = true;

		break;

	}

}

void ShockWave::Init()
{
	for (auto& index : m_shockWave) {
		index.m_isActive = false;
	}
}

void ShockWave::Update()
{

	for (int index = 0; index < 8; ++index) {

		if (!m_shockWave[index].m_isActive) continue;

		//衝撃波の計算
		m_shockWaveEasingTimer[index] = std::clamp(m_shockWaveEasingTimer[index] + 1.0f, 0.0f, SHOCKWAVE_EASING_TIMER);
		//イージングを計算。
		float easing = EasingMaker(EasingType::Out, EaseInType::Cubic, m_shockWaveEasingTimer[index] / SHOCKWAVE_EASING_TIMER);
		m_shockWave[index].m_radius = easing * SHOCK_WAVE_RAIDUS;
		m_shockWave[index].m_power = (1.0f - easing);

		if (SHOCKWAVE_EASING_TIMER <= m_shockWaveEasingTimer[index]) {

			m_shockWave[index].m_isActive = false;

		}

	}

	m_shockWaveParamData.bufferWrapper->TransData(m_shockWave.data(), sizeof(ShockWaveData) * 8);
}
