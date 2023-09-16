#include "PointLightMgr.h"
#include "../KazLibrary/Buffer/GBufferMgr.h"

void PointLightMgr::Update(bool arg_isNight)
{

	//夜だったら
	if (arg_isNight) {


		for (auto& index : m_pointLightData) {

			++index.m_changeTimer;

			if (index.m_changeTime <= index.m_changeTimer) {

				index.m_facter = FACTER + KazMath::Rand(-0.1f, 0.1f);
				index.m_power = POWER + KazMath::Rand(-10.0f, 10.0f);

				index.m_changeTime = KazMath::Rand(5, 10);
				index.m_changeTimer = 0;

			}

		}

	}
	//昼だったらライトを消す。
	else {

		for (auto& index : m_pointLightData) {

			if (index.m_facter <= 0.0f) continue;

			index.m_facter -= index.m_facter / 5.0f;

			if (index.m_facter <= 0.01f) {
				index.m_facter = 0.0f;
			}

		}

	}

	//更新処理
	const int SIZE = static_cast<int>(m_pointLightData.size());
	for (int index = 0; index < SIZE; ++index) {

		GBufferMgr::Instance()->m_lightConstData.m_pointLight[index].m_pad.x += (m_pointLightData[index].m_facter - GBufferMgr::Instance()->m_lightConstData.m_pointLight[index].m_pad.x) / 5.0f;
		GBufferMgr::Instance()->m_lightConstData.m_pointLight[index].m_power += (m_pointLightData[index].m_power - GBufferMgr::Instance()->m_lightConstData.m_pointLight[index].m_power) / 5.0f;

	}

}
