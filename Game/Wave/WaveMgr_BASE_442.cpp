#include "WaveMgr.h"
#include "Wave.h"
#include "../Game/Enemy/EnemyMgr.h"

void WaveMgr::Setting()
{

	//���݂̃E�F�[�u�����������B
	m_nowWaveIndex = 0;

	//��������G�̏��z��
	std::vector<Wave::EnemyWaveInfo> enemyInfo;

	//1�E�F�[�u�� -----------------------------------------------------------------------------------------
	int dayTime = 10;		//�����̎��� �t���[����
	int nightTime = 6000;	//��̎��� �t���[����
	std::vector<int> tree = { 1, 3 };		//�L�������ɐ��������؂�Index 1�X�^�[�g
	std::vector<int> rock = { 1 };		//�L�������ɐ����������Index 1�X�^�[�g
	std::vector<int> mineralRock = { 2, 3 };		//�L�������ɐ��������~�l�������Index 1�X�^�[�g
	//�G��ǉ����Ă����B
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 120);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 120);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 120);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 120);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 120);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 120);
	/*
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 30);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 60);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 90);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 120);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINETSUMURI, 0);*/
	//enemyInfo.emplace_back(EnemyRoute::ROUTE_ID::B, 0);
	//enemyInfo.emplace_back(EnemyRoute::ROUTE_ID::C, 0);
	//enemyInfo.emplace_back(EnemyRoute::ROUTE_ID::A, 600);
	//enemyInfo.emplace_back(EnemyRoute::ROUTE_ID::B, 600);
	//enemyInfo.emplace_back(EnemyRoute::ROUTE_ID::C, 600);
	//enemyInfo.emplace_back(EnemyRoute::ROUTE_ID::A, 1200);
	//enemyInfo.emplace_back(EnemyRoute::ROUTE_ID::B, 1200);
	//enemyInfo.emplace_back(EnemyRoute::ROUTE_ID::C, 1200);
	//�E�F�[�u��ǉ��B
	m_waves.emplace_back(std::make_shared<Wave>(dayTime, nightTime, tree, rock, mineralRock, enemyInfo));
	enemyInfo.clear();


	//2�E�F�[�u�� -----------------------------------------------------------------------------------------
	//dayTime = 6000;		//�����̎��� �t���[����
	//nightTime = 6000;	//��̎��� �t���[����
	//treeCount = 3;		//�L�������ɐ��������؂̐�
	//rockCount = 3;		//�L�������ɐ���������̐�
	//mineralRockCount = 3;		//�L�������ɐ��������~�l������̐�
	////�G��ǉ����Ă����B
	////enemyInfo.emplace_back(EnemyRoute::ROUTE_ID::A, 0);	//������ �����ꏊ  ������ �鎞�Ԃł̐����t���[��(dayTime���l������K�v�͂Ȃ��A0����nightTime�͈̔͂ł��肢���܂��B)
	////enemyInfo.emplace_back(EnemyRoute::ROUTE_ID::B, 0);
	////enemyInfo.emplace_back(EnemyRoute::ROUTE_ID::C, 0);
	////enemyInfo.emplace_back(EnemyRoute::ROUTE_ID::A, 600);
	////enemyInfo.emplace_back(EnemyRoute::ROUTE_ID::B, 600);
	////enemyInfo.emplace_back(EnemyRoute::ROUTE_ID::C, 600);
	////enemyInfo.emplace_back(EnemyRoute::ROUTE_ID::A, 1200);
	////enemyInfo.emplace_back(EnemyRoute::ROUTE_ID::B, 1200);
	////enemyInfo.emplace_back(EnemyRoute::ROUTE_ID::C, 1200);
	////�E�F�[�u��ǉ��B
	//m_waves.emplace_back(std::make_shared<Wave>(dayTime, nightTime, treeCount, rockCount, mineralRockCount, enemyInfo));
	//enemyInfo.clear();






	//�E�F�[�u����ۑ��B
	m_waveCount = static_cast<int>(m_waves.size());

	//UI�����[�h
	m_timerUI.Load("Resource/UI/Timer/Timer.png");
	m_frameUI.Load("Resource/UI/Timer/Frame.png");

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

}

void WaveMgr::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{

	//UI��`��
	m_frameUI.Draw(arg_rasterize);
	m_timerUI.Draw(arg_rasterize);

}

void WaveMgr::GameStart()
{

	m_waves.front()->Active();

}
