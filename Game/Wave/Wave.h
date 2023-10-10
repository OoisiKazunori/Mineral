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
		EnemyRoute::ROUTE_ID m_routeID;	//敵が湧く場所のID
		ENEMY_ID m_enemyID;
		int m_spawnFrame; //敵が湧くフレーム
		EnemyWaveInfo(EnemyRoute::ROUTE_ID arg_routeID, ENEMY_ID arg_enemyID, int arg_spawnFrame) : m_routeID(arg_routeID), m_enemyID(arg_enemyID), m_spawnFrame(arg_spawnFrame) {};
	};

private:

	int m_dayTime;		//日中のフレーム数 0からスタート
	int m_nighTime;		//夜のフレーム数 0からスタート
	int m_nowTime;		//現在の経過フレーム
	std::vector<int> m_tree;
	std::vector<int> m_rock;
	std::vector<int> m_mineralRock;
	eWeather m_weather;
	bool m_isActiveWave;
	std::vector<EnemyWaveInfo> m_enemyWaveInfo;	//敵を湧かせるフレーム数
	bool m_isPlayNiwatori;

public:

	struct WheatherData
	{
		eWeather m_weather;
		int m_time;
	};

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

	//ウェーブを有効化
	void Active();
	//ウェーブを無効化
	void Invalidate(std::weak_ptr<EnemyMgr> arg_enemyMgr);

	float WaveTimerRate();

	bool GetIsActiveWave() { return m_isActiveWave; }
	bool GetIsNight() { return m_weather == NIGHT; }
	void SetMNowTime(int set_time) { m_nowTime = set_time; }
};