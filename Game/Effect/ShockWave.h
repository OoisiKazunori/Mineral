#pragma once
#include"../KazLibrary/Helper/ISinglton.h"
#include"../KazLibrary/Math/KazMath.h"
#include <Helper/KazBufferHelper.h>

class ShockWave :public ISingleton<ShockWave>
{

public:

	struct ShockWaveData {

		KazMath::Vec3<float> m_pos;
		float m_radius;
		float m_power;
		int m_isActive;
		float m_facter;
		float m_pad;

	};

	std::array<ShockWaveData, 8> m_shockWave;
	std::array<float, 8> m_shockWaveEasingTimer;
	const float SHOCKWAVE_EASING_TIMER = 60.0f;
	const float SHOCK_WAVE_RAIDUS = 100.0f;
	KazBufferHelper::BufferData m_shockWaveParamData;

	void Setting();
	void Generate(KazMath::Vec3<float> arg_generatePos);
	void Init();
	void Update();

};