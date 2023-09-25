#pragma once
#include "DrawCallSet.h"
#include "DrawCallUISet.h"
#include "../KazLibrary/Sound/SoundManager.h"

class Player {

private:
	SoundData slap_se;
	SoundData slap_strong_se;
	SoundData damage;
	SoundData down;
	//プレイヤー描画関連
	DrawCallSet m_model;
	DrawCallSet m_attackModel;

	KazMath::Transform3D m_oldTransform;
	KazMath::Transform3D m_transform;
	KazMath::Transform3D m_drawTransform;
	KazMath::Vec3<float> m_daipanPos;	//台パン中にカメラを固定するための座標
	KazMath::Vec3<float> m_forwardVec;			//正面ベクトル
	DirectX::XMVECTOR m_forwardQ;
	DirectX::XMVECTOR m_baseQ;
	DirectX::XMVECTOR m_daipanQ;

	//隊列関連
	KazMath::Vec3<float> m_mineralCenterBasePos;	//ミネラルの中心座標。隊列を移動させる時に使う。
	KazMath::Vec3<float> m_mineralCenterPos;		//ミネラルの中心座標。隊列を移動させる時に使う。
	const float MINERAL_CENTER_DISTANCE = 10.0f;
	const float MINERAL_MOVE_SPEED = 0.8f;

	//台パンの範囲のモデル関連
	DrawCallSet m_daipanModel;
	DrawCallSet m_daipanStrongModel;

	//ダメージを受けたときのシェイク
	float m_damageShake;
	KazMath::Vec3<float> m_damageShakePos;
	const float DAMAGE_SHAKE = 3.0f;

	//HPが0になってスタン状態になった際にそれが回復するまでのタイマー
	float m_hpRecoveryTimer;
	const float HP_RECOVERY_TIMER = 300.0f;

	//台パン関連
	bool m_isInputDaipan;	//台パンのキーが入力されているか
	bool m_isOldInputDaipan;//台パンのキーの入力の前フレーム
	enum DAIPAN_STATE {
		NONE,
		CHARGE,			//入力をチャージしている間。
		DAIPAN_NOW,		//台パン中
		RETURN,

	}m_daipanStatus;
	float m_daipanXAngle;

	//強台パンの変数
	bool m_isDaipanStrong;
	float m_daipanStrongTimer;
	const float DAIPAN_STRONG_TIMER = 40.0f;
	KazMath::Vec3<float> m_daipanShakePos;
	const float DAIPAN_SHAKE_POS = 0.5f;

	//台パン中の変数
	float m_daipanStartPosY;
	float m_daipanTimer;
	const float DAIPAN_NOW_TIMER = 7.0f;
	const float DAIPAN_NOW_STRONG_TIMER = 4.0f;
	const float DAIPAN_SHAKE = 1.5f;
	const float DAIPAN_STRONG_SHAKE = 3.8f;

	//台パンから帰っている途中の変数。
	float m_daipanReturnTimer;
	const float DAIPAN_RETURN_TIMER = 8.0f;

	//Y軸関連
	const float DEFAULT_Y = 30.0f;
	const float DAIPAN_Y = DEFAULT_Y + 10.0f;
	float m_stanGravity;
	const float STAN_GRAVITY = 0.3f;

	//ミネラルを集合させる範囲
	const float MINERAL_AFFECT_RANGE = 60.0f;
	const float MINERAL_AFFECT_STRONG_RANGE = 15.0f;

	//HP関連
	float m_hp;
	bool m_isStun;
	const float HP = 10;
	const float HIT_SCALE = 5.0f;
	float m_autoRecoveryStartTimer;
	const float AUTO_RECOVERY_START_TIMER = 300.0f;
	float m_autoRecoveryDelayTimer;
	const float AUTO_RECOVERY_DELAY_TIMER = 10.0f;

	//ミネラル解散関連。
	bool m_isBreakUp;
	bool m_isOldBreakUp;
	bool m_isWaveHand;
	float m_waveHandTimer;

	//立ち絵をかえるタイマー
	int m_damageChangeDadanUITimer;
	const int DAMAGE_CHANGE_DADAN_UI_TIMER = 60;


	//UI関連
	DrawCallUISet m_hpFrameUI;
	DrawCallUISet m_hpUI;
	DrawCallUISet m_hpBackGroundUI;
	DrawCallUISet m_sousaUI;
	std::array<DrawCallUISet, 3> m_dadanUI;
	DrawCallUISet m_dadanBackGroundUI;



	KazMath::Vec2<float> UI_BASE_POS = { 218.0f, 592.0f };
	KazMath::Vec2<float> UI_BASE_SCALE = { 430.0f, 250.0f };
	KazMath::Vec2<float> UI_HPBAR_POS = { 183.0f, 666.0f };
	KazMath::Vec2<float> UI_HPBAR_SCALE = { 282.0f, 32.0f };


public:

	const int DAIPAN_DAMAGE = 2;
	const int STRONG_DAIPAN_DAMAGE = 10;

	Player();

	void Init();

	void Update();

	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	//座標を取得する。
	KazMath::Transform3D GetTransform() { return m_transform; }
	//Y軸を0にした座標を取得する。
	KazMath::Vec3<float> GetPosZeroY() { return KazMath::Vec3<float>(m_transform.pos.x, 0.0f, m_transform.pos.z); }
	//座標を取得する。
	KazMath::Vec3<float> GetMineralCenterPos() { return m_mineralCenterBasePos + m_mineralCenterPos; }
	//正面ベクトルを持ってくる、。
	KazMath::Vec3<float> GetForwardVec() { return m_forwardVec; }
	//Y軸をデフォルトの高さにした現在座標を取得する。
	KazMath::Vec3<float> GetTransformDefaultY() { return KazMath::Vec3<float>(m_daipanPos.x, DEFAULT_Y, m_daipanPos.z); }
	//ミネラルに影響を与える範囲。
	float GetMineralAffectRange() { return MINERAL_AFFECT_RANGE; }
	float GetMineralAffectStrongRange() { return MINERAL_AFFECT_STRONG_RANGE; }
	float GetMineralBreakUpRange() { return MINERAL_AFFECT_RANGE * 1.5f; }
	//台パンのタイミングか
	bool GetIsDaipanTrigger() { return (m_isDaipanStrong ? DAIPAN_NOW_STRONG_TIMER : DAIPAN_NOW_TIMER) - 1.0f == m_daipanTimer; }
	//強台パンか？
	bool GetIsStrongDaipan() { return m_isDaipanStrong; }
	//解散状態か？
	bool GetIsBreakUP() { return m_isBreakUp; }

	void Damage(int arg_damage);
	bool GetIsStun() { return m_isStun; }
	float GetHitScale() { return HIT_SCALE; }

	int GetDaipanDamage() {
		return m_isDaipanStrong ? STRONG_DAIPAN_DAMAGE : DAIPAN_DAMAGE;
	}

	KazMath::Vec3<float> TransformVec3(KazMath::Vec3<float> arg_value, DirectX::XMVECTOR arg_quaternion);

};