#pragma once
#include "../Game/DrawCallSet.h"
#include "../Enemy/EnemyRoute.h"
#include "../KazLibrary/Sound/SoundManager.h"
#include "../Game/Core/Core.h"

class EnemyMgr;

class Wave {

public:
	std::weak_ptr<Core> m_core;
	SoundData night_start;
	bool volume_up;
	bool volume_down;

	enum class ENEMY_ID {
		MINEKUJI,
		MINETSUMURI,
		MINEKING,
	};

	struct EnemyWaveInfo {
		EnemyRoute::ROUTE_ID m_routeID;	//�G���N���ꏊ��ID
		ENEMY_ID m_enemyID;
		int m_spawnFrame; //�G���N���t���[��
		EnemyWaveInfo(EnemyRoute::ROUTE_ID arg_routeID, ENEMY_ID arg_enemyID, int arg_spawnFrame) : m_routeID(arg_routeID), m_enemyID(arg_enemyID), m_spawnFrame(arg_spawnFrame) {};
	};

private:

	int m_dayTime;		//�����̃t���[���� 0����X�^�[�g
	int m_nighTime;		//��̃t���[���� 0����X�^�[�g
	int m_nowTime;		//���݂̌o�߃t���[��
	std::vector<int> m_tree;
	std::vector<int> m_rock;
	std::vector<int> m_mineralRock;
	bool m_isNight;
	bool m_isActiveWave;
	std::vector<EnemyWaveInfo> m_enemyWaveInfo;	//�G��N������t���[����

public:

	Wave(int arg_dayTime, int arg_nightTime, std::vector<int> arg_tree, std::vector<int> arg_rock, std::vector<int> arg_mineralRock, std::vector<EnemyWaveInfo> arg_enemyWaveInfo, std::weak_ptr<Core> m_core);

	void Update(std::weak_ptr<EnemyMgr> arg_enemyMgr);

	//�E�F�[�u��L����
	void Active();
	//�E�F�[�u�𖳌���
	void Invalidate(std::weak_ptr<EnemyMgr> arg_enemyMgr);

	float WaveTimerRate();

	bool GetIsActiveWave() { return m_isActiveWave; }
	bool GetIsNight() { return m_isNight; }
	void SetMNowTime(int set_time) { m_nowTime = set_time; }
};