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

	//BGM�p
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

	//�^�C�g����������^�C�}�[��1��Œ肷��B
	if (TitleFlag::Instance()->m_isTitle)
	{
		m_nowTime.m_time = 1;
	}

	//�`���[�g���A����������^�C�}�[��1�ɌŒ肷��
	if (Tutorial::Instance()->is_tutorial)
	{
		m_nowTime.m_time = 1;
		//���Ԃ̐ݒ�
		switch (Tutorial::Instance()->tutorial_num)
		{
		case 2:
			//�~�l�N�W���N��
			if (!Tutorial::Instance()->spawn_minekuzi)
			{
				m_nowTime.m_time = 1;
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
	if (!ResultFlag::Instance()->m_isResult && !OptionUI::Instance()->m_optionDetails[OptionUI::DEBUG_NAME::TIMEZONE].m_selectID) {

		//�鎞�Ԃ�������
		if (m_isNight) {

			//���̎��ԂɗN���G��������N������B
			for (auto& enemy : m_enemyWaveInfo) {

				if (enemy.m_spawnFrame != m_nowTime.m_time) continue;

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
				else if (enemy.m_enemyID == ENEMY_ID::TOGEKURI) {

					arg_enemyMgr.lock()->GenerateMinekuji(enemy.m_routeID, false, true);

				}

			}

			//���Ԃ�i�߂�B
			m_nowTime.m_time = std::clamp(m_nowTime.m_time + 1, 0, m_nightTime.m_time);

			bool isEnemyZero = arg_enemyMgr.lock()->GetAliveEnemyCount() <= 0;
			bool isFinalWave = WaveMgr::Instance()->IsFinalWave();

			if (isFinalWave) {

				if (isEnemyZero) {

					//���Ԃ��I�����Wave�𖳌�������B
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

				//���Ԃ��I�����Wave�𖳌�������B
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
		//�����Ԃ�������
		else {


			//���Ԃ�i�߂�B
			m_nowTime.m_time = std::clamp(m_nowTime.m_time + 1, 0, m_dayTime.m_time);

			//���Ԃ��I�����Wave�𖳌�������B
			if (m_dayTime.m_time <= m_nowTime.m_time) {

				//�鎞�Ԃ�
				m_nowTime.m_time = 0;
				m_isNight = true;
				SoundManager::Instance()->SoundPlayerWave(night_start, 0);

				//BGM��炷
				volume_up = true;
				if (WaveMgr::Instance()->GetWaveCount() == 0)
				{
					WaveMgr::Instance()->start_bgm = true;
				}
			}
		}

		//�G�̃X�|�[���̍ő厞��
		int enemyMaxSpawnTime = 0;
		for (auto& index : m_enemyWaveInfo) {

			if (index.m_spawnFrame < enemyMaxSpawnTime) continue;

			enemyMaxSpawnTime = index.m_spawnFrame;
		}

		if (m_isNight) {


			//�Ō�̓G�������Ă�����B
			bool isEnemyEnd = enemyMaxSpawnTime < m_nowTime.m_time;
			bool isZeroEnemy = arg_enemyMgr.lock()->GetAliveEnemyCount() <= 0;
			if (isEnemyEnd && isZeroEnemy) {

				//���Ԍo�߂𑬂߂�B
				m_nowTime.m_time = std::clamp(m_nowTime.m_time + 20, 0, m_nightTime.m_time);

			}

		}

	}

	if (!TitleFlag::Instance()->m_isTitle &&
		OptionUI::Instance()->m_optionDetails[OptionUI::DEBUG_NAME::TIMEZONE].m_selectID == 0)
	{
		//�f�B���N�V���i�����C�g�̕�����ς��Ē����\��
		if (m_isNight)
		{
			//��
			GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir += (KazMath::Vec3<float>(0.0f, -0.4472f, 0.894f) - GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir) / 30.0f;
		}
		//�����̉J�̎��ԑ�
		else if (!m_isNight && m_nowTime.m_weather == RAIN && OptionUI::Instance()->m_optionDetails[OptionUI::DEBUG_NAME::RAIN].m_selectID != 1)
		{
			GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir += (KazMath::Vec3<float>(0.0f, -0.648f, 0.894f) - GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir) / 30.0f;
		}
		else
		{
			//��
			GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir += (KazMath::Vec3<float>(0.0f, -0.894f, 0.4472f) - GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir) / 30.0f;
		}


		GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir.Normalize();

		//���C�g�̃A�b�v�f�[�g
		PointLightMgr::Instance()->Update(m_isNight);
	}
	else if (OptionUI::Instance()->m_optionDetails[OptionUI::DEBUG_NAME::TIMEZONE].m_selectID)
	{
		//��
		if (OptionUI::Instance()->m_optionDetails[OptionUI::DEBUG_NAME::TIMEZONE].m_selectID == 2)
		{
			//��
			GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir += (KazMath::Vec3<float>(0.0f, -0.4472f, 0.894f) - GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir) / 30.0f;
		}
		//�����̉J�̎��ԑ�
		else if (OptionUI::Instance()->m_optionDetails[OptionUI::DEBUG_NAME::TIMEZONE].m_selectID == 1 &&
			OptionUI::Instance()->m_optionDetails[OptionUI::DEBUG_NAME::RAIN].m_selectID == 2)
		{
			GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir += (KazMath::Vec3<float>(0.0f, -0.648f, 0.894f) - GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir) / 30.0f;
		}
		//��
		else
		{
			GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir += (KazMath::Vec3<float>(0.0f, -0.894f, 0.4472f) - GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir) / 30.0f;
		}
		GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir.Normalize();
		//���C�g�̃A�b�v�f�[�g
		PointLightMgr::Instance()->Update(OptionUI::Instance()->m_optionDetails[OptionUI::DEBUG_NAME::TIMEZONE].m_selectID == 2);
	}
}

void Wave::Active()
{

	//�E�F�[�u��L�����B
	m_isActiveWave = true;
	m_isPlayNiwatori = false;
	m_isNight = false;
	m_nowTime.m_time = 0;


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

	if (m_isNight) {

		return static_cast<float>(m_nowTime.m_time) / static_cast<float>(m_nightTime.m_time);

	}

	return static_cast<float>(m_nowTime.m_time) / static_cast<float>(m_dayTime.m_time);
}
