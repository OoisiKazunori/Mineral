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
		TOGEKURI,
	};

	struct EnemyWaveInfo {
		EnemyRoute::ROUTE_ID m_routeID;	//�G���N���ꏊ��ID
		ENEMY_ID m_enemyID;
		int m_spawnFrame; //�G���N���t���[��
		EnemyWaveInfo(EnemyRoute::ROUTE_ID arg_routeID, ENEMY_ID arg_enemyID, int arg_spawnFrame) : m_routeID(arg_routeID), m_enemyID(arg_enemyID), m_spawnFrame(arg_spawnFrame) {};
	};
	struct WheatherData
	{
		eWeather m_weather;
		int m_time;
	};
private:

	WheatherData m_dayTime;
	WheatherData m_nightTime;
	WheatherData m_nowTime;		//���݂̌o�߃t���[��
	std::vector<int> m_tree;
	std::vector<int> m_rock;
	std::vector<int> m_mineralRock;
	bool m_isNight;

	bool m_isActiveWave;
	std::vector<EnemyWaveInfo> m_enemyWaveInfo;	//�G��N������t���[����
	bool m_isPlayNiwatori;

public:



	struct InitWaveData
	{
		WheatherData m_dayTime;
		WheatherData m_nightTime;
		std::vector<int> m_tree;
		std::vector<int> m_rock;
		std::vector<int> m_mineralRock;
		std::vector<EnemyWaveInfo> m_enemyWaveInfo;
		std::weak_ptr<Core> m_core;
	};

	Wave(const InitWaveData& arg_initData);

	void Update(std::weak_ptr<EnemyMgr> arg_enemyMgr);

	//�E�F�[�u��L����
	void Active();
	//�E�F�[�u�𖳌���
	void Invalidate(std::weak_ptr<EnemyMgr> arg_enemyMgr);

	float WaveTimerRate();

	bool GetIsActiveWave() { return m_isActiveWave; }
	bool GetIsNight() { return m_isNight; }
	bool GetIsRain() { return m_nowTime.m_weather == RAIN; }
	void SetMNowTime(int set_time) { m_nowTime.m_time = set_time; }
};