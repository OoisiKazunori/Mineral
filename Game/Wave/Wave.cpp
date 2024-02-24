#include "Wave.h"
#include "../Game/Enemy/EnemyMgr.h"
#include "../Game/DestructibleObject/DestructibleObjectMgr.h"
#include "../Game/WallAndTreeGeneratePos.h"
#include "../PointLightMgr.h"
#include "../Game/Rock/RockMgr.h"
#include "../Game/TitleFlag.h"
#include "../Game/ResultFlag.h"
#include "../Game/Tutorial.h"
#include "../Wave/WaveMgr.h"
#include "../Game/UI/OptionUI.h"

Wave::Wave(const InitWaveData& arg_initData)
{
	m_dayTime = arg_initData.m_dayTime;
	m_nightTime = arg_initData.m_nightTime;
	m_tree = arg_initData.m_tree;
	m_rock = arg_initData.m_rock;
	m_mineralRock = arg_initData.m_mineralRock;
	m_enemyWaveInfo = arg_initData.m_enemyWaveInfo;
	m_nowTime = { SUNNY,0 };
	m_isActiveWave = false;
	m_isNight = false;

	m_core = arg_initData.m_core;
	m_core.lock()->SetHp(25);
	night_start = SoundManager::Instance()->SoundLoadWave("Resource/Sound/night_start.wav");
	night_start.volume = 0.1f;

	//BGM用
	volume_up = false;
	volume_down = true;
	m_isPlayNiwatori = false;
}

void Wave::Update(std::weak_ptr<EnemyMgr> arg_enemyMgr)
{

	if (m_isNight)
	{
		m_nowTime.m_weather = m_nightTime.m_weather;
	}
	else
	{
		m_nowTime.m_weather = m_dayTime.m_weather;
	}

	//タイトルだったらタイマーを1二固定する。
	if (TitleFlag::Instance()->m_isTitle)
	{
		m_nowTime.m_time = 1;
	}

	//チュートリアルだったらタイマーを1に固定する
	if (Tutorial::Instance()->is_tutorial)
	{
		m_nowTime.m_time = 1;
		//時間の設定
		switch (Tutorial::Instance()->tutorial_num)
		{
		case 2:
			//ミネクジが湧く
			if (!Tutorial::Instance()->spawn_minekuzi)
			{
				m_nowTime.m_time = 1;
				Tutorial::Instance()->spawn_minekuzi = true;
				arg_enemyMgr.lock()->GenerateMinekuji(EnemyRoute::A, true);
			}
			break;

		case 3:
			//ミネラル岩が湧く
			if (!Tutorial::Instance()->spawn_m_rock)
			{
				Tutorial::Instance()->spawn_m_rock = true;
				RockMgr::Instance()->Generate(WallAndTreeGeneratePos::Instance()->m_rockPos[2], {}, true, 2);
			}
			break;

		case 5:
			//岩が湧く
			if (!Tutorial::Instance()->spawn_rock)
			{
				Tutorial::Instance()->spawn_rock = true;
				RockMgr::Instance()->Generate(WallAndTreeGeneratePos::Instance()->m_rockPos[2], {}, false, 2);
			}
			break;


		case 8:
			//木が湧く
			if (!Tutorial::Instance()->spawn_tree)
			{
				Tutorial::Instance()->spawn_tree = true;
				DestructibleObjectMgr::Instance()->GenerateTree(WallAndTreeGeneratePos::Instance()->m_treePos[2]);
			}
			break;
		default:

			break;
		}
	}

	//リザルト中は時間を進めない。
	if (!ResultFlag::Instance()->m_isResult && !OptionUI::Instance()->m_optionDetails[OptionUI::DEBUG_NAME::TIMEZONE].m_selectID) {

		//夜時間だったら
		if (m_isNight) {

			//この時間に湧く敵がいたら湧かせる。
			for (auto& enemy : m_enemyWaveInfo) {

				if (enemy.m_spawnFrame != m_nowTime.m_time) continue;

				//敵を湧かせる。
				if (enemy.m_enemyID == ENEMY_ID::MINEKUJI) {

					arg_enemyMgr.lock()->GenerateMinekuji(enemy.m_routeID);

				}
				else if (enemy.m_enemyID == ENEMY_ID::MINETSUMURI) {

					arg_enemyMgr.lock()->GenerateMinetsumuri(enemy.m_routeID, false);

				}
				else if (enemy.m_enemyID == ENEMY_ID::MINEKING) {

					arg_enemyMgr.lock()->GenerateMinetsumuri(enemy.m_routeID, true);

				}
				else if (enemy.m_enemyID == ENEMY_ID::TOGEKURI) {

					arg_enemyMgr.lock()->GenerateMinekuji(enemy.m_routeID, false, true);

				}

			}

			//時間を進める。
			m_nowTime.m_time = std::clamp(m_nowTime.m_time + 1, 0, m_nightTime.m_time);

			bool isEnemyZero = arg_enemyMgr.lock()->GetAliveEnemyCount() <= 0;
			bool isFinalWave = WaveMgr::Instance()->IsFinalWave();

			if (isFinalWave) {

				if (isEnemyZero) {

					//時間が終わったWaveを無効化する。
					if (m_nightTime.m_time <= m_nowTime.m_time) {
						if (!m_isPlayNiwatori && !WaveMgr::Instance()->IsNextWave()) {
							SoundManager::Instance()->SoundPlayerWave(WaveMgr::Instance()->start_morning, 0);
							m_isPlayNiwatori = true;
						}
						Invalidate(arg_enemyMgr);
						if (Tutorial::Instance()->is_tutorial)
						{
							Tutorial::Instance()->is_tutorial = false;
						}
					}

				}

			}
			else {

				//時間が終わったWaveを無効化する。
				if (m_nightTime.m_time <= m_nowTime.m_time) {
					if (!m_isPlayNiwatori && !WaveMgr::Instance()->IsNextWave()) {
						SoundManager::Instance()->SoundPlayerWave(WaveMgr::Instance()->start_morning, 0);
						m_isPlayNiwatori = true;
					}
					Invalidate(arg_enemyMgr);
					if (Tutorial::Instance()->is_tutorial)
					{
						Tutorial::Instance()->is_tutorial = false;
					}
				}

			}

		}
		//昼時間だったら
		else {


			//時間を進める。
			m_nowTime.m_time = std::clamp(m_nowTime.m_time + 1, 0, m_dayTime.m_time);

			//時間が終わったWaveを無効化する。
			if (m_dayTime.m_time <= m_nowTime.m_time) {

				//夜時間へ
				m_nowTime.m_time = 0;
				m_isNight = true;
				SoundManager::Instance()->SoundPlayerWave(night_start, 0);

				//BGMを鳴らす
				volume_up = true;
				if (WaveMgr::Instance()->GetWaveCount() == 0)
				{
					WaveMgr::Instance()->start_bgm = true;
				}
			}
		}

		//敵のスポーンの最大時間
		int enemyMaxSpawnTime = 0;
		for (auto& index : m_enemyWaveInfo) {

			if (index.m_spawnFrame < enemyMaxSpawnTime) continue;

			enemyMaxSpawnTime = index.m_spawnFrame;
		}

		if (m_isNight) {


			//最後の敵が沸いていたら。
			bool isEnemyEnd = enemyMaxSpawnTime < m_nowTime.m_time;
			bool isZeroEnemy = arg_enemyMgr.lock()->GetAliveEnemyCount() <= 0;
			if (isEnemyEnd && isZeroEnemy) {

				//時間経過を速める。
				m_nowTime.m_time = std::clamp(m_nowTime.m_time + 20, 0, m_nightTime.m_time);

			}

		}

	}

	if (!TitleFlag::Instance()->m_isTitle &&
		OptionUI::Instance()->m_optionDetails[OptionUI::DEBUG_NAME::TIMEZONE].m_selectID == 0)
	{
		//ディレクショナルライトの方向を変えて昼夜を表現
		if (m_isNight)
		{
			//夜
			GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir += (KazMath::Vec3<float>(0.0f, -0.4472f, 0.894f) - GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir) / 30.0f;
		}
		//日中の雨の時間帯
		else if (!m_isNight && m_nowTime.m_weather == RAIN && OptionUI::Instance()->m_optionDetails[OptionUI::DEBUG_NAME::RAIN].m_selectID != 1)
		{
			GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir += (KazMath::Vec3<float>(0.0f, -0.648f, 0.894f) - GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir) / 30.0f;
		}
		else
		{
			//昼
			GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir += (KazMath::Vec3<float>(0.0f, -0.894f, 0.4472f) - GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir) / 30.0f;
		}


		GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir.Normalize();

		//ライトのアップデート
		PointLightMgr::Instance()->Update(m_isNight);
	}
	else if (OptionUI::Instance()->m_optionDetails[OptionUI::DEBUG_NAME::TIMEZONE].m_selectID)
	{
		//夜
		if (OptionUI::Instance()->m_optionDetails[OptionUI::DEBUG_NAME::TIMEZONE].m_selectID == 2)
		{
			//夜
			GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir += (KazMath::Vec3<float>(0.0f, -0.4472f, 0.894f) - GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir) / 30.0f;
		}
		//日中の雨の時間帯
		else if (OptionUI::Instance()->m_optionDetails[OptionUI::DEBUG_NAME::TIMEZONE].m_selectID == 1 &&
			OptionUI::Instance()->m_optionDetails[OptionUI::DEBUG_NAME::RAIN].m_selectID == 2)
		{
			GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir += (KazMath::Vec3<float>(0.0f, -0.648f, 0.894f) - GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir) / 30.0f;
		}
		//昼
		else
		{
			GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir += (KazMath::Vec3<float>(0.0f, -0.894f, 0.4472f) - GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir) / 30.0f;
		}
		GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir.Normalize();
		//ライトのアップデート
		PointLightMgr::Instance()->Update(OptionUI::Instance()->m_optionDetails[OptionUI::DEBUG_NAME::TIMEZONE].m_selectID == 2);
	}
}

void Wave::Active()
{

	//ウェーブを有効化。
	m_isActiveWave = true;
	m_isPlayNiwatori = false;
	m_isNight = false;
	m_nowTime.m_time = 0;


	//木と岩をランダムで配置。
	for (auto& index : m_tree) {

		DestructibleObjectMgr::Instance()->GenerateTree(WallAndTreeGeneratePos::Instance()->m_treePos[index - 1]);

	}
	for (auto& index : m_rock) {

		RockMgr::Instance()->Generate(WallAndTreeGeneratePos::Instance()->m_rockPos[index - 1], {}, false, 2);

	}
	for (auto& index : m_mineralRock) {

		RockMgr::Instance()->Generate(WallAndTreeGeneratePos::Instance()->m_rockPos[index - 1], {}, true, 2);

	}

}

void Wave::Invalidate(std::weak_ptr<EnemyMgr> arg_enemyMgr)
{

	//全ての敵を無効化。
	arg_enemyMgr.lock()->Init();

	//全ての破壊可能オブジェクトを削除。
	//DestructibleObjectMgr::Instance()->Init();

	//Waveを終える。
	m_isActiveWave = false;

}

float Wave::WaveTimerRate()
{

	if (m_isNight) {

		return static_cast<float>(m_nowTime.m_time) / static_cast<float>(m_nightTime.m_time);

	}

	return static_cast<float>(m_nowTime.m_time) / static_cast<float>(m_dayTime.m_time);
}
