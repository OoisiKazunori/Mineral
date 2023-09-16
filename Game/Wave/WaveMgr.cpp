#include "WaveMgr.h"
#include "Wave.h"
#include "../Game/Enemy/EnemyMgr.h"
#include "../Game/TitleFlag.h"
#include "../Game/ResultFlag.h"
#include "../Game/Tutorial.h"
#include "../Game/UI/NumberFont.h"
#include <Imgui/imgui.h>

void WaveMgr::Setting(std::weak_ptr<Core> m_core)
{

	//���݂̃E�F�[�u�����������B
	m_nowWaveIndex = 0;

	//��������G�̏��z��
	std::vector<Wave::EnemyWaveInfo> enemyInfo;
	//�E�F�[�u�ɕK�v�ȏ��
	int dayTime;		//�����̎��� �t���[����
	int nightTime;	//��̎��� �t���[����
	std::vector<int> tree = {  };		//�L�������ɐ��������؂�Index 1�X�^�[�g
	std::vector<int> rock = {  };		//�L�������ɐ����������Index 1�X�^�[�g
	std::vector<int> mineralRock = {  };		//�L�������ɐ��������~�l�������Index 1�X�^�[�g


	//�`���[�g���A���̃E�F�[�u-----------------------------------------------------------------------------------------
	dayTime = 60;
	nightTime = 1800;
	//�G��ǉ����Ă����B
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 180);
	//enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINETSUMURI, 0);
	//�E�F�[�u��ǉ��B
	m_tutorialWave = std::make_shared<Wave>(dayTime, nightTime, tree, rock, mineralRock, enemyInfo, m_core);
	enemyInfo.clear();


	//1�E�F�[�u�� -----------------------------------------------------------------------------------------
	dayTime = 600;		//�����̎��� �t���[����
	nightTime = 1800;	//��̎��� �t���[����
	tree = {  };		//�L�������ɐ��������؂�Index 1�X�^�[�g
	rock = {  };		//�L�������ɐ����������Index 1�X�^�[�g
	mineralRock = { 1 };		//�L�������ɐ��������~�l�������Index 1�X�^�[�g
	//�G��ǉ����Ă����B
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 360);
	//enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINETSUMURI, 180);
	//enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 360);
	//enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINETSUMURI, 0);
	//�E�F�[�u��ǉ��B
	m_wave1 = std::make_shared<Wave>(dayTime, nightTime, tree, rock, mineralRock, enemyInfo, m_core);
	enemyInfo.clear();

	m_waves.emplace_back(m_wave1);


	//2�E�F�[�u�� -----------------------------------------------------------------------------------------
	dayTime = 1800;		//�����̎��� �t���[����
	nightTime = 3200;	//��̎��� �t���[����
	tree = { 4 };		//�L�������ɐ��������؂̐�
	rock = {  };		//�L�������ɐ���������̐�
	mineralRock = { 3 };		//�L�������ɐ��������~�l������̐�
	//�G��ǉ����Ă����B
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 360);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 1440);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 1620);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 1800);

	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 360);
	//�E�F�[�u��ǉ��B
	m_waves.emplace_back(std::make_shared<Wave>(dayTime, nightTime, tree, rock, mineralRock, enemyInfo, m_core));
	enemyInfo.clear();


	//3�E�F�[�u�� -----------------------------------------------------------------------------------------
	dayTime = 1800;		//�����̎��� �t���[����
	nightTime = 3200;	//��̎��� �t���[����
	tree = { 2 };		//�L�������ɐ��������؂̐�
	rock = {  };		//�L�������ɐ���������̐�
	mineralRock = { 1 };		//�L�������ɐ��������~�l������̐�
	//�G��ǉ����Ă����B
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 360);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 720);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 900);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 1080);

	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 360);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 720);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 900);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 1080);

	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINETSUMURI, 900);
	//�E�F�[�u��ǉ��B
	m_waves.emplace_back(std::make_shared<Wave>(dayTime, nightTime, tree, rock, mineralRock, enemyInfo, m_core));
	enemyInfo.clear();



	//4�E�F�[�u�� -----------------------------------------------------------------------------------------
	dayTime = 1800;		//�����̎��� �t���[����
	nightTime = 3200;	//��̎��� �t���[����
	tree = { 3,8 };		//�L�������ɐ��������؂̐�
	rock = { };		//�L�������ɐ���������̐�
	mineralRock = { 2,6 };		//�L�������ɐ��������~�l������̐�
	//�G��ǉ����Ă����B
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 360);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 360);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 720);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 900);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 1080);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 720);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 900);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 1080);

	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 360);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 360);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 720);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 900);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 1080);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 720);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 900);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 1080);

	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINEKUJI, 360);
	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINEKUJI, 360);
	//�E�F�[�u��ǉ��B
	m_waves.emplace_back(std::make_shared<Wave>(dayTime, nightTime, tree, rock, mineralRock, enemyInfo, m_core));
	enemyInfo.clear();


	//5�E�F�[�u�� -----------------------------------------------------------------------------------------
	dayTime = 1800;		//�����̎��� �t���[����
	nightTime = 3200;	//��̎��� �t���[����
	tree = { 6 };		//�L�������ɐ��������؂̐�
	rock = { 4 };		//�L�������ɐ���������̐�
	mineralRock = { 2 };		//�L�������ɐ��������~�l������̐�
	//�G��ǉ����Ă����B
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINETSUMURI, 0);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINETSUMURI, 180);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINETSUMURI, 360);


	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINETSUMURI, 720);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINETSUMURI, 900);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINETSUMURI, 1080);

	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINETSUMURI, 0);
	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINETSUMURI, 180);
	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINETSUMURI, 360);
	//�E�F�[�u��ǉ��B
	m_waves.emplace_back(std::make_shared<Wave>(dayTime, nightTime, tree, rock, mineralRock, enemyInfo, m_core));
	enemyInfo.clear();


	//6�E�F�[�u�� -----------------------------------------------------------------------------------------
	dayTime = 1800;		//�����̎��� �t���[����
	nightTime = 3200;	//��̎��� �t���[����
	tree = { 4 };		//�L�������ɐ��������؂̐�
	rock = { 2 };		//�L�������ɐ���������̐�
	mineralRock = { 3 };		//�L�������ɐ��������~�l������̐�
	//�G��ǉ����Ă����B
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 360);


	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 720);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 900);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 1080);

	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINEKING, 0);
	//�E�F�[�u��ǉ��B
	m_waves.emplace_back(std::make_shared<Wave>(dayTime, nightTime, tree, rock, mineralRock, enemyInfo, m_core));
	enemyInfo.clear();


	//7�E�F�[�u�� -----------------------------------------------------------------------------------------
	dayTime = 1800;		//�����̎��� �t���[����
	nightTime = 4800;	//��̎��� �t���[����
	tree = { 4,6 };		//�L�������ɐ��������؂̐�
	rock = { 4,3 };		//�L�������ɐ���������̐�
	mineralRock = { 1 };		//�L�������ɐ��������~�l������̐�
	//�G��ǉ����Ă����B
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 360);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 360);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINETSUMURI, 720);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINETSUMURI, 900);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINETSUMURI, 1080);

	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 360);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 360);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINETSUMURI, 720);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINETSUMURI, 900);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINETSUMURI, 1080);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKING, 0);

	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINEKUJI, 360);
	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINEKUJI, 360);
	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINETSUMURI, 720);
	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINETSUMURI, 900);
	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINETSUMURI, 1080);
	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINEKING, 900);

	//�E�F�[�u��ǉ��B
	m_waves.emplace_back(std::make_shared<Wave>(dayTime, nightTime, tree, rock, mineralRock, enemyInfo, m_core));
	enemyInfo.clear();


	//�E�F�[�u����ۑ��B
	m_waveCount = static_cast<int>(m_waves.size());

	m_isFinishAllWave = false;

	//UI�����[�h
	m_timerUI.Load("Resource/UI/Timer/Timer.png");
	m_frameUI.Load("Resource/UI/Timer/Frame.png");

	//�����\���pUI
	m_daysUI.Load("Resource/UI/NumFont/Day.png");
	m_daysUI.m_transform.pos = { 108,230 };
	m_daysUI.m_transform.scale = { 100,50 };

	//�����e�N�X�`�����[�h
	m_dayNumerUI.Load("Resource/UI/NumFont/0.png");

	//BGM
	m_BGM = SoundManager::Instance()->SoundLoadWave("Resource/Sound/bgm.wav");
	SoundManager::Instance()->SoundPlayerWave(m_BGM, 100);
	m_BGM.source->SetVolume(0.0f);
	m_BGM.source->Stop();
	volume = 0.0f;
	start_bgm = false;
	start_morning = SoundManager::Instance()->SoundLoadWave("Resource/Sound/Kokekoko.wav");
	start_morning.volume = 0.07f;
}

void WaveMgr::Init(std::weak_ptr<EnemyMgr> arg_enemyMgr)
{

	m_nowWaveIndex = 0;
	for (auto& wave : m_waves) {

		wave->Invalidate(arg_enemyMgr);

	}

}

void WaveMgr::Update(std::weak_ptr<EnemyMgr> arg_enemyMgr)
{

	//�E�F�[�u���X�V�B
	m_waves[m_nowWaveIndex]->Update(arg_enemyMgr);

	//���݂̃E�F�[�u���I����Ă����玟�̃E�F�[�u��
	if (!m_waves[m_nowWaveIndex]->GetIsActiveWave()) {

		//���̃E�F�[�u�����݂��Ă��邩�H
		if (m_waveCount - 1 <= m_nowWaveIndex) {

			//���̃E�F�[�u���Ȃ���
			m_isFinishAllWave = true;

		}
		else {

			++m_nowWaveIndex;
			m_waves[m_nowWaveIndex]->Active();

		}

	}

	//UI���X�V�B
	m_timerUI.m_transform.scale = { 168,168 };
	m_timerUI.m_transform.pos = { 128,128 };
	if (m_waves[m_nowWaveIndex]->GetIsNight()) {
		m_timerUI.m_transform.rotation = 180.0f + m_waves[m_nowWaveIndex]->WaveTimerRate() * 180.0f - 90.0f;
	}
	else {
		m_timerUI.m_transform.rotation = m_waves[m_nowWaveIndex]->WaveTimerRate() * 180.0f - 90.0f;
	}
	m_frameUI.m_transform.scale = { 168,168 };
	m_frameUI.m_transform.pos = { 128,128 };

	//������\���B
	m_dayNumerUI.m_texture = NumberFont::Instance()->m_font[std::clamp(static_cast<int>(m_nowWaveIndex + 1), 0, 9)];

}

void WaveMgr::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{


	if (0 < ResultFlag::Instance()->m_uiDeleteTime) {


		m_frameUI.m_color.color.a += static_cast<int>((0.0f - m_frameUI.m_color.color.a) / 3.0f);
		m_timerUI.m_color.color.a += static_cast<int>((0.0f - m_timerUI.m_color.color.a) / 3.0f);
		m_dayNumerUI.m_color.color.a += static_cast<int>((0.0f - m_dayNumerUI.m_color.color.a) / 3.0f);
		m_daysUI.m_color.color.a += static_cast<int>((0.0f - m_daysUI.m_color.color.a) / 3.0f);

		if (m_frameUI.m_color.color.a <= 50.0f) {

			m_frameUI.m_color.color.a = 0;
			m_timerUI.m_color.color.a = 0;
			m_dayNumerUI.m_color.color.a = 0;
			m_daysUI.m_color.color.a = 0;

		}
		else {

			//UI��`��B
			m_dayNumerUI.Draw(arg_rasterize);
			m_timerUI.Draw(arg_rasterize);
			m_timerUI.Draw(arg_rasterize);
			m_frameUI.Draw(arg_rasterize);
		}
	}
	else if ((!TitleFlag::Instance()->m_isTitle && !ResultFlag::Instance()->m_isResult)) {

		//UI��`��B
		m_dayNumerUI.Draw(arg_rasterize);
		m_timerUI.Draw(arg_rasterize);
		m_frameUI.Draw(arg_rasterize);
		m_daysUI.Draw(arg_rasterize);

		m_frameUI.m_color.color.a += static_cast<int>((255.0f - m_frameUI.m_color.color.a) / 15.0f);
		m_timerUI.m_color.color.a += static_cast<int>((255.0f - m_timerUI.m_color.color.a) / 15.0f);
		m_dayNumerUI.m_color.color.a += static_cast<int>((255.0f - m_dayNumerUI.m_color.color.a) / 15.0f);
		m_daysUI.m_color.color.a += static_cast<int>((255.0f - m_daysUI.m_color.color.a) / 15.0f);

	}
	else {

		m_frameUI.m_color.color.a = 0;
		m_timerUI.m_color.color.a = 0;
		m_dayNumerUI.m_color.color.a = 0;
		m_daysUI.m_color.color.a = 0;

	}

	m_dayNumerUI.m_transform.pos = { 170.0f, 230.0f };
	m_dayNumerUI.m_transform.scale = { 30.0f, 30.0f };

}

void WaveMgr::GameStart()
{

	//�`���[�g���A����������ŏ��̃E�F�[�u�������ւ��B
	if (Tutorial::Instance()->is_tutorial) {
		m_waves.front() = m_tutorialWave;
	}
	else {
		m_waves.front() = m_wave1;
	}

	m_waves.front()->Active();

	m_isFinishAllWave = false;

}

bool WaveMgr::GetIsNight()
{
	return m_waves[m_nowWaveIndex]->GetIsNight();
}

void WaveMgr::SetTime(int wave_index, int wave_time)
{
	m_waves[wave_index]->SetMNowTime(wave_time);
}
