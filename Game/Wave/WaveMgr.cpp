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

	//現在のウェーブ数を初期化。
	m_nowWaveIndex = 0;

	//生成する敵の情報配列
	std::vector<Wave::EnemyWaveInfo> enemyInfo;
	Wave::InitWaveData initData;

	//チュートリアルのウェーブ-----------------------------------------------------------------------------------------

	initData.m_dayTime.m_time = 60;
	initData.m_nightTime.m_time = 1800;
	initData.m_dayTime.m_weather = SUNNY;
	initData.m_nightTime.m_weather = SUNNY;
	//敵を追加していく。

	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 180);
	//enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINETSUMURI, 0);
	//ウェーブを追加。
	initData.m_enemyWaveInfo = enemyInfo;

	initData.m_tree = {};
	initData.m_rock = {};
	initData.m_mineralRock = {};
	initData.m_core = m_core;
	m_tutorialWave = std::make_shared<Wave>(initData);

	enemyInfo.clear();


	//1ウェーブ目 -----------------------------------------------------------------------------------------
	initData.m_dayTime.m_time = 60;
	initData.m_nightTime.m_time = 1800;
	initData.m_dayTime.m_weather = SUNNY;
	initData.m_nightTime.m_weather = SUNNY;

	initData.m_tree = { 1 };		//有効化時に生成される木のIndex 1スタート
	initData.m_rock = {  };		//有効化時に生成される岩のIndex 1スタート
	initData.m_mineralRock = { 1 };		//有効化時に生成されるミネラル岩のIndex 1スタート
	//敵を追加していく。
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 360);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 360);
	//enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINETSUMURI, 180);
	//enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 360);
	//enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINETSUMURI, 0);
	initData.m_enemyWaveInfo = enemyInfo;
	initData.m_core = m_core;
	//ウェーブを追加。
	m_wave1 = std::make_shared<Wave>(initData);
	enemyInfo.clear();

	m_waves.emplace_back(m_wave1);


	//2ウェーブ目 -----------------------------------------------------------------------------------------
	initData.m_dayTime.m_time = 1800;		//日中の時間 フレーム数
	initData.m_nightTime.m_time = 3200;	//夜の時間 フレーム数
	initData.m_dayTime.m_weather = RAIN;
	initData.m_nightTime.m_weather = SUNNY;
	initData.m_tree = { 4 };
	initData.m_rock = {  };		//有効化時に生成される岩の数
	initData.m_mineralRock = { 3 };		//有効化時に生成されるミネラル岩の数
	//敵を追加していく。
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 360);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 360);

	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 360);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 360);
	//ウェーブを追加。
	m_waves.emplace_back(std::make_shared<Wave>(initData));
	enemyInfo.clear();




	//3ウェーブ目 -----------------------------------------------------------------------------------------
	initData.m_dayTime.m_time = 1800;		//日中の時間 フレーム数
	initData.m_nightTime.m_time = 3200;	//夜の時間 フレーム数
	initData.m_dayTime.m_weather = SUNNY;
	initData.m_nightTime.m_weather = SUNNY;
	initData.m_tree = { 2,1 };		//有効化時に生成される木の数
	initData.m_rock = {  };		//有効化時に生成される岩の数
	initData.m_mineralRock = { 1,4 };		//有効化時に生成されるミネラル岩の数
	//敵を追加していく。
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
	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINEKUJI, 360);
	//ウェーブを追加。
	m_waves.emplace_back(std::make_shared<Wave>(initData));
	enemyInfo.clear();



	//4ウェーブ目 -----------------------------------------------------------------------------------------
	initData.m_dayTime.m_time = 1800;		//日中の時間 フレーム数
	initData.m_nightTime.m_time = 3200;	//夜の時間 フレーム数
	initData.m_dayTime.m_weather = RAIN;
	initData.m_nightTime.m_weather = RAIN;
	initData.m_tree = { 3,8 };		//有効化時に生成される木の数
	initData.m_rock = { };		//有効化時に生成される岩の数
	initData.m_mineralRock = { 2,6 };		//有効化時に生成されるミネラル岩の数
	//敵を追加していく。
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
	//ウェーブを追加。
	m_waves.emplace_back(std::make_shared<Wave>(initData));
	enemyInfo.clear();


	//5ウェーブ目 -----------------------------------------------------------------------------------------
	initData.m_dayTime.m_time = 1800;		//日中の時間 フレーム数
	initData.m_nightTime.m_time = 3200;	//夜の時間 フレーム数
	initData.m_dayTime.m_weather = SUNNY;
	initData.m_nightTime.m_weather = SUNNY;
	initData.m_tree = { 6 };		//有効化時に生成される木の数
	initData.m_rock = { 4 };		//有効化時に生成される岩の数
	initData.m_mineralRock = { 2 };		//有効化時に生成されるミネラル岩の数
	//敵を追加していく。
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINETSUMURI, 0);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINETSUMURI, 180);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINETSUMURI, 360);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 360);

	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINETSUMURI, 720);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINETSUMURI, 900);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINETSUMURI, 1080);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 360);

	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINETSUMURI, 0);
	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINETSUMURI, 180);
	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINETSUMURI, 360);
	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINEKUJI, 360);
	//ウェーブを追加。
	m_waves.emplace_back(std::make_shared<Wave>(initData));
	enemyInfo.clear();


	//6ウェーブ目 -----------------------------------------------------------------------------------------
	initData.m_dayTime.m_time = 1800;		//日中の時間 フレーム数
	initData.m_nightTime.m_time = 3200;	//夜の時間 フレーム数
	initData.m_dayTime.m_weather = RAIN;
	initData.m_nightTime.m_weather = RAIN;
	initData.m_tree = { 4 };		//有効化時に生成される木の数
	initData.m_rock = { 2 };		//有効化時に生成される岩の数
	initData.m_mineralRock = { 3 };		//有効化時に生成されるミネラル岩の数
	//敵を追加していく。
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 360);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 360);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 720);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 900);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 1080);

	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 720);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 900);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 1080);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 720);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 900);
	enemyInfo.emplace_back(EnemyRoute::B, Wave::ENEMY_ID::MINEKUJI, 1080);

	enemyInfo.emplace_back(EnemyRoute::C, Wave::ENEMY_ID::MINEKING, 0);
	//ウェーブを追加。
	m_waves.emplace_back(std::make_shared<Wave>(initData));
	enemyInfo.clear();


	//7ウェーブ目 -----------------------------------------------------------------------------------------
	initData.m_dayTime.m_time = 1800;		//日中の時間 フレーム数
	initData.m_nightTime.m_time = 4800;	//夜の時間 フレーム数
	initData.m_dayTime.m_weather = SUNNY;
	initData.m_nightTime.m_weather = SUNNY;
	initData.m_tree = { 4,6 };		//有効化時に生成される木の数
	initData.m_rock = { 4,3 };		//有効化時に生成される岩の数
	initData.m_mineralRock = { 1 };		//有効化時に生成されるミネラル岩の数
	//敵を追加していく。
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 360);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 0);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 180);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 360);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 720);
	enemyInfo.emplace_back(EnemyRoute::A, Wave::ENEMY_ID::MINEKUJI, 900);
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

	//ウェーブを追加。
	m_waves.emplace_back(std::make_shared<Wave>(initData));
	enemyInfo.clear();


	//ウェーブ数を保存。
	m_waveCount = static_cast<int>(m_waves.size());

	m_isFinishAllWave = false;

	//UIをロード
	m_timerUI.Load("Resource/UI/Timer/Timer.png");
	m_frameUI.Load("Resource/UI/Timer/Frame.png");

	//日数表示用UI
	m_daysUI.Load("Resource/UI/NumFont/Day.png");
	m_daysUI.m_transform.pos = { 108,230 };
	m_daysUI.m_transform.scale = { 100,50 };

	//数字テクスチャロード
	m_dayNumerUI.Load("Resource/UI/NumFont/0.png");

	//BGM
	m_BGM = SoundManager::Instance()->SoundLoadWave("Resource/Sound/bgm.wav");
	SoundManager::Instance()->SoundPlayerWave(m_BGM, 100);
	m_BGM.source->SetVolume(0.0f);
	//m_BGM.source->Stop();
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

	//BGM関連
	{
		//ボリューム調整
		WaveMgr::Instance()->m_BGM.source->SetVolume(WaveMgr::Instance()->volume);

		//ボリュームを上げるフラグ
		bool isNight = m_waves[m_nowWaveIndex]->GetIsNight();
		bool isResult = ResultFlag::Instance()->m_isResult;
		bool isActiveWave = m_waves[m_nowWaveIndex]->GetIsActiveWave();

		//ボリュームを下げるフラグ

		if ((isNight && isActiveWave) || isResult || GetIsFinishAllWave())
		{
			if (WaveMgr::Instance()->volume < 0.05f)
			{
				WaveMgr::Instance()->volume += 0.0005f;
			}
		}
		else
		{

			if (WaveMgr::Instance()->volume > 0.0001f)
			{
				WaveMgr::Instance()->volume -= 0.0005f;
			}
		}
	}


	//ウェーブを更新。
	m_waves[m_nowWaveIndex]->Update(arg_enemyMgr);

	//現在のウェーブが終わっていたら次のウェーブへ
	if (!m_waves[m_nowWaveIndex]->GetIsActiveWave()) {

		//次のウェーブが存在しているか？
		if (m_waveCount - 1 <= m_nowWaveIndex) {

			//敵がすべていなかったら
			if (arg_enemyMgr.lock()->GetAliveEnemyCount() <= 0) {

				//次のウェーブがない時
				m_isFinishAllWave = true;

			}

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

	//日数を表示。
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

			//UIを描画。
			m_dayNumerUI.Draw(arg_rasterize);
			m_timerUI.Draw(arg_rasterize);
			m_timerUI.Draw(arg_rasterize);
			m_frameUI.Draw(arg_rasterize);
		}
	}
	else if ((!TitleFlag::Instance()->m_isTitle && !ResultFlag::Instance()->m_isResult)) {

		//UIを描画。
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

	//チュートリアルだったら最初のウェーブを差し替え。
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

bool WaveMgr::GetIsRain()
{
	return m_waves[m_nowWaveIndex]->GetIsRain();
}

void WaveMgr::SetTime(int wave_index, int wave_time)
{
	m_waves[wave_index]->SetMNowTime(wave_time);
}

bool WaveMgr::IsFinalWave()
{
	return m_waves.back()->GetIsActiveWave();
}

bool WaveMgr::IsNextWave()
{
	return m_waveCount - 1 <= m_nowWaveIndex;
}
