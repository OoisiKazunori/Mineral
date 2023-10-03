#pragma once
#include"../KazLibrary/Helper/ISinglton.h"
#include"../KazLibrary/Helper/KazBufferHelper.h"
#include"../KazLibrary/Math/KazMath.h"
#include<memory>
/// <summary>
/// ヒットストップ設定用構造体
/// </summary>
struct HitStopParam
{
	//適用する時間
	int m_time;
	//適用する速度
	float m_speed;

	//コンストラクタ
	HitStopParam(int time = 0, float speed = 1.0f);
};

/// <summary>
/// ゲームスピードを操るクラス
/// </summary>
class StopMgr :public ISingleton<StopMgr>
{

private:

	//ゲーム内で流れる速度の割合
	float m_gameSpeed;

	//速度が変化している時間
	int m_slowTimer;
public:

	void Init();

	void Update();

	/// <summary>
	/// 引数で渡された値に合わせてヒットストップを実行する
	/// </summary>
	/// <param name="param">ヒットストップを発生させる値</param>
	void HitStopStart(const HitStopParam& param);

	//ヒットストップ適用する際のゲームスピードを受け取る
	float GetGameSpeed();

	void HitStopEnemy()
	{

	}
	bool IsHitStop() {
		return 0 < m_slowTimer;
	}
};