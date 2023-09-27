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

Wave::Wave(int arg_dayTime, int arg_nightTime, std::vector<int> arg_tree, std::vector<int> arg_rock, std::vector<int> arg_mineralRock, std::vector<EnemyWaveInfo> arg_enemyWaveInfo, std::weak_ptr<Core> core)
{

	m_dayTime = arg_dayTime;
	m_nighTime = arg_nightTime;
	m_tree = arg_tree;
	m_rock = arg_rock;
	m_mineralRock = arg_mineralRock;
	m_enemyWaveInfo = arg_enemyWaveInfo;
	m_nowTime = 0;
	m_isNight = false;
	m_isActiveWave = false;

	m_core = core;
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

	//タイトルだったらタイマーを1二固定する。
	if (TitleFlag::Instance()->m_isTitle) {
		m_nowTime = 1;
	}

	//チュートリアルだったらタイマーを1に固定する
	if (Tutorial::Instance()->is_tutorial)
	{
		m_nowTime = 1;
		//時間の設定
		switch (Tutorial::Instance()->tutorial_num)
		{
		case 2:
			//ミネクジが湧く
			if (!Tutorial::Instance()->spawn_minekuzi)
			{
				m_nowTime = 1;
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
	if (!ResultFlag::Instance()->m_isResult) {

		//夜時間だったら
		if (m_isNight) {

			//この時間に湧く敵がいたら湧かせる。
			for (auto& enemy : m_enemyWaveInfo) {

				if (enemy.m_spawnFrame != m_nowTime) continue;

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

			}

			//時間を進める。
			m_nowTime = std::clamp(m_nowTime + 1, 0, m_nighTime);

			//時間が終わったWaveを無効化する。
			if (m_nighTime <= m_nowTime) {
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
		//昼時間だったら
		else {


			//時間を進める。
			m_nowTime = std::clamp(m_nowTime + 1, 0, m_dayTime);

			//時間が終わったWaveを無効化する。
			if (m_dayTime <= m_nowTime) {

				//夜時間へ
				m_nowTime = 0;
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

		if (!TitleFlag::Instance()->m_isTitle) {

			//ディレクショナルライトの方向を変えて昼夜を表現
			if (!m_isNight) {
				GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir += (KazMath::Vec3<float>(0.0f, -0.894f, 0.4472f) - GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir) / 30.0f;
			}
			else if (m_isNight) {
				GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir += (KazMath::Vec3<float>(0.0f, -0.4472f, 0.894f) - GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir) / 30.0f;
			}
			GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir.Normalize();

			//ライトのアップデート
			PointLightMgr::Instance()->Update(m_isNight);

		}

	}

	//敵のスポーンの最大時間
	int enemyMaxSpawnTime = 0;
	for (auto& index : m_enemyWaveInfo) {

		if (index.m_spawnFrame < enemyMaxSpawnTime) continue;

		enemyMaxSpawnTime = index.m_spawnFrame;

	}

	//最後の敵が沸いていたら。
	bool isEnemyEnd = enemyMaxSpawnTime < m_nowTime;
	bool isZeroEnemy = arg_enemyMgr.lock()->GetAliveEnemyCount() <= 0;
	if (isEnemyEnd && isZeroEnemy) {

		//時間経過を速める。
		m_nowTime = std::clamp(m_nowTime + 20, 0, m_nighTime);

	}

}

void Wave::Active()
{

	//ウェーブを有効化。
	m_isActiveWave = true;
	m_isNight = false;
	m_isPlayNiwatori = false;
	m_nowTime = 0;


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
	DestructibleObjectMgr::Instance()->Init();

	//Waveを終える。
	m_isActiveWave = false;

}

float Wave::WaveTimerRate()
{

	if (m_isNight) {

		return static_cast<float>(m_nowTime) / static_cast<float>(m_nighTime);

	}

	return static_cast<float>(m_nowTime) / static_cast<float>(m_dayTime);
}
