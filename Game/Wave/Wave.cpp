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

Wave::Wave(const InitWaveData& arg_initData)
{
	m_dayTime = arg_initData.m_dayTime.m_time;
	m_nighTime = arg_initData.m_nightTime.m_time;
	m_tree = arg_initData.m_tree;
	m_rock = arg_initData.m_rock;
	m_mineralRock = arg_initData.m_mineralRock;
	m_enemyWaveInfo = arg_initData.m_enemyWaveInfo;
	m_nowTime = 0;
	m_weather = arg_initData.m_dayTime.m_weather;
	m_isActiveWave = false;

	m_core = arg_initData.m_core;
	m_core.lock()->SetHp(25);
	night_start = SoundManager::Instance()->SoundLoadWave("Resource/Sound/night_start.wav");
	night_start.volume = 0.1f;

	//BGM�p
	volume_up = false;
	volume_down = true;
	m_isPlayNiwatori = false;
}

void Wave::Update(std::weak_ptr<EnemyMgr> arg_enemyMgr)
{

	//�^�C�g����������^�C�}�[��1��Œ肷��B
	if (TitleFlag::Instance()->m_isTitle) {
		m_nowTime = 1;
	}

	//�`���[�g���A����������^�C�}�[��1�ɌŒ肷��
	if (Tutorial::Instance()->is_tutorial)
	{
		m_nowTime = 1;
		//���Ԃ̐ݒ�
		switch (Tutorial::Instance()->tutorial_num)
		{
		case 2:
			//�~�l�N�W���N��
			if (!Tutorial::Instance()->spawn_minekuzi)
			{
				m_nowTime = 1;
				Tutorial::Instance()->spawn_minekuzi = true;
				arg_enemyMgr.lock()->GenerateMinekuji(EnemyRoute::A, true);
			}
			break;

		case 3:
			//�~�l�����₪�N��
			if (!Tutorial::Instance()->spawn_m_rock)
			{
				Tutorial::Instance()->spawn_m_rock = true;
				RockMgr::Instance()->Generate(WallAndTreeGeneratePos::Instance()->m_rockPos[2], {}, true, 2);
			}
			break;

		case 5:
			//�₪�N��
			if (!Tutorial::Instance()->spawn_rock)
			{
				Tutorial::Instance()->spawn_rock = true;
				RockMgr::Instance()->Generate(WallAndTreeGeneratePos::Instance()->m_rockPos[2], {}, false, 2);
			}
			break;


		case 8:
			//�؂��N��
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

	//���U���g���͎��Ԃ�i�߂Ȃ��B
	if (!ResultFlag::Instance()->m_isResult) {

		//�鎞�Ԃ�������
		if (m_weather == NIGHT) {

			//���̎��ԂɗN���G��������N������B
			for (auto& enemy : m_enemyWaveInfo) {

				if (enemy.m_spawnFrame != m_nowTime) continue;

				//�G��N������B
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

			//���Ԃ�i�߂�B
			m_nowTime = std::clamp(m_nowTime + 1, 0, m_nighTime);

			bool isEnemyZero = arg_enemyMgr.lock()->GetAliveEnemyCount() <= 0;
			bool isFinalWave = WaveMgr::Instance()->IsFinalWave();

			if (isFinalWave) {

				if (isEnemyZero) {

					//���Ԃ��I�����Wave�𖳌�������B
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

			}
			else {

				//���Ԃ��I�����Wave�𖳌�������B
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

		}
		//�����Ԃ�������
		else {


			//���Ԃ�i�߂�B
			m_nowTime = std::clamp(m_nowTime + 1, 0, m_dayTime);

			//���Ԃ��I�����Wave�𖳌�������B
			if (m_dayTime <= m_nowTime) {

				//�鎞�Ԃ�
				m_nowTime = 0;
				m_weather = NIGHT;
				SoundManager::Instance()->SoundPlayerWave(night_start, 0);

				//BGM��炷
				volume_up = true;
				if (WaveMgr::Instance()->GetWaveCount() == 0)
				{
					WaveMgr::Instance()->start_bgm = true;
				}
			}
		}

		if (!TitleFlag::Instance()->m_isTitle) {

			//�f�B���N�V���i�����C�g�̕�����ς��Ē����\��
			switch (m_weather)
			{
			case SUNNY:
				GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir += (KazMath::Vec3<float>(0.0f, -0.894f, 0.4472f) - GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir) / 30.0f;
				break;
			case RAIN:
				GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir += (KazMath::Vec3<float>(0.0f, -0.648f, 0.894f) - GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir) / 30.0f;
				break;
			case NIGHT:
				GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir += (KazMath::Vec3<float>(0.0f, -0.4472f, 0.894f) - GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir) / 30.0f;
				break;
			default:
				break;
			}
			GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir.Normalize();

			//���C�g�̃A�b�v�f�[�g
			PointLightMgr::Instance()->Update(m_weather == NIGHT);

		}

	}

	//�G�̃X�|�[���̍ő厞��
	int enemyMaxSpawnTime = 0;
	for (auto& index : m_enemyWaveInfo) {

		if (index.m_spawnFrame < enemyMaxSpawnTime) continue;

		enemyMaxSpawnTime = index.m_spawnFrame;

	}

	if (m_weather == NIGHT) {

		//�Ō�̓G�������Ă�����B
		bool isEnemyEnd = enemyMaxSpawnTime < m_nowTime;
		bool isZeroEnemy = arg_enemyMgr.lock()->GetAliveEnemyCount() <= 0;
		if (isEnemyEnd && isZeroEnemy) {

			//���Ԍo�߂𑬂߂�B
			m_nowTime = std::clamp(m_nowTime + 20, 0, m_nighTime);

		}

	}

}

void Wave::Active()
{

	//�E�F�[�u��L�����B
	m_isActiveWave = true;
	m_isPlayNiwatori = false;
	m_nowTime = 0;


	//�؂Ɗ�������_���Ŕz�u�B
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

	//�S�Ă̓G�𖳌����B
	arg_enemyMgr.lock()->Init();

	//�S�Ă̔j��\�I�u�W�F�N�g���폜�B
	//DestructibleObjectMgr::Instance()->Init();

	//Wave���I����B
	m_isActiveWave = false;

}

float Wave::WaveTimerRate()
{

	if (m_weather == NIGHT) {

		return static_cast<float>(m_nowTime) / static_cast<float>(m_nighTime);

	}

	return static_cast<float>(m_nowTime) / static_cast<float>(m_dayTime);
}
