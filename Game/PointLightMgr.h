#pragma once
#include "../Game/DrawCallSet.h"
#include "../KazLibrary/Helper/ISinglton.h"

class PointLightMgr : public ISingleton<PointLightMgr> {

public:

	struct PointLihgtData {

		float m_power;
		float m_facter;
		int m_changeTimer;
		int m_changeTime;

		PointLihgtData() {
			m_power = 0;
			m_facter = 0;
			m_changeTime = 0;
			m_changeTimer = 0;
		}

	};

	const float FACTER = 0.5f;
	const float POWER = 100.0f;

	std::array<PointLihgtData, 10> m_pointLightData;

public:

	void Update(bool arg_isNight);

};