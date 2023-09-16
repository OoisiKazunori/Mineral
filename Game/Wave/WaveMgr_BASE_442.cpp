#include "WaveMgr.h"
#include "Wave.h"
#include "../Game/Enemy/EnemyMgr.h"

void WaveMgr::Setting()
{

	//現在のウェーブ数を初期化。
	m_nowWaveIndex = 0;

	//生成する敵の情報配列
	std::vector<Wave::EnemyWaveInfo> enemyInfo;

	//1ウェーブ目 -----------------------------------------------------------------------------------------
	int dayTime = 10;		//日中の時間 フレーム数
	int nightTime = 6000;	//夜の時間 フレーム数
	std::vector<int> tree = { 1, 3 };		//有効化時に生成される木のIndex 1スタート
	std::vector<int> rock = { 1 };		//有効化時に生成される岩のIndex 1スタート
	std::vector<int> mineralRock = { 2, 3 };		//有効化時に生成されるミネラル岩のIndex 1スタート
	//敵を追加していく。
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
	//ウェーブを追加。
	m_waves.emplace_back(std::make_shared<Wave>(dayTime, nightTime, tree, rock, mineralRock, enemyInfo));
	enemyInfo.clear();


	//2ウェーブ目 -----------------------------------------------------------------------------------------
	//dayTime = 6000;		//日中の時間 フレーム数
	//nightTime = 6000;	//夜の時間 フレーム数
	//treeCount = 3;		//有効化時に生成される木の数
	//rockCount = 3;		//有効化時に生成される岩の数
	//mineralRockCount = 3;		//有効化時に生成されるミネラル岩の数
	////敵を追加していく。
	////enemyInfo.emplace_back(EnemyRoute::ROUTE_ID::A, 0);	//第一引数 生成場所  第二引数 夜時間での生成フレーム(dayTimeを考慮する必要はなく、0からnightTimeの範囲でお願いします。)
	////enemyInfo.emplace_back(EnemyRoute::ROUTE_ID::B, 0);
	////enemyInfo.emplace_back(EnemyRoute::ROUTE_ID::C, 0);
	////enemyInfo.emplace_back(EnemyRoute::ROUTE_ID::A, 600);
	////enemyInfo.emplace_back(EnemyRoute::ROUTE_ID::B, 600);
	////enemyInfo.emplace_back(EnemyRoute::ROUTE_ID::C, 600);
	////enemyInfo.emplace_back(EnemyRoute::ROUTE_ID::A, 1200);
	////enemyInfo.emplace_back(EnemyRoute::ROUTE_ID::B, 1200);
	////enemyInfo.emplace_back(EnemyRoute::ROUTE_ID::C, 1200);
	////ウェーブを追加。
	//m_waves.emplace_back(std::make_shared<Wave>(dayTime, nightTime, treeCount, rockCount, mineralRockCount, enemyInfo));
	//enemyInfo.clear();






	//ウェーブ数を保存。
	m_waveCount = static_cast<int>(m_waves.size());

	//UIをロード
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

	//ウェーブを更新。
	m_waves[m_nowWaveIndex]->Update(arg_enemyMgr);

	//現在のウェーブが終わっていたら次のウェーブへ
	if (!m_waves[m_nowWaveIndex]->GetIsActiveWave()) {

		//次のウェーブが存在しているか？
		if (m_waveCount - 1 <= m_nowWaveIndex) {

			//次のウェーブがない時

		}
		else {

			++m_nowWaveIndex;
			m_waves[m_nowWaveIndex]->Active();

		}

	}

	//UIを更新。
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

	//UIを描画
	m_frameUI.Draw(arg_rasterize);
	m_timerUI.Draw(arg_rasterize);

}

void WaveMgr::GameStart()
{

	m_waves.front()->Active();

}
