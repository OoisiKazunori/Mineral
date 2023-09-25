#pragma once
#include "../Game/DrawCallSet.h"
#include "../Game/DrawCallUISet.h"
#include "../KazLibrary/Sound/SoundManager.h"

class Core {

private:

	SoundData wall_break01;
	SoundData wall_break02;
	SoundData damage;

	DrawCallSet m_model;
	DrawCallSet m_crownModel;
	KazMath::Vec3<float> m_crownPos;
	KazMath::Transform3D m_transform;
	int m_hp;
	const int MAX_HP = 25;
	const float SCALE = 10.0f;

	//コアがダメージを受けた時関連の変数
	DrawCallUISet m_coreDamageUI;
	enum class UISTATE {
		APPEAR,
		EXIT,
	}m_uiState;
	int m_uiAppearCount;
	const int UI_APPEAR_COUNT = 2;
	float m_damageUITimer;
	const float APPEAR_TIME = 20.0f;
	const float EXIT_TIME = 40.0f;
	bool m_isActiveDamageUI;

	//再度ダメージのUIを出すまでのタイマー
	int m_damageUIAgainTimer;
	const int DAMAGEUI_AGAIN_TIMER = 60 * 2;

public:

	/*オカモトゾーン*/
	DrawCallSet m_hpBoxModel;
	KazMath::Transform3D m_hpBoxTransform;
	int m_hpBoxDrawTimer;
	const int HP_BOX_DRAW_TIME_MAX = 60;
	float hpBoxScaleStart;
	float hpBoxScaleEnd;
	bool isDrawHpBox;
	float hpBoxEaseTime;
	const float HP_BOX_EASE_TIME_MAX = 30.0f;
	float ease_scale;
	/*オカモトゾーン*/

	Core();

	void Init();

	void Update();

	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	void Damage(int arg_damage = 1);

	bool IsDead() { return m_hp <= 0; }

	void SetPos(KazMath::Vec3<float> arg_pos) { m_transform.pos = arg_pos; };
	float GetScale() { return SCALE; }
	KazMath::Vec3<float> GetPos() { return m_transform.pos; }
	KazMath::Vec3<float> GetPosZeroY() { return { m_transform.pos.x, 0.0f, m_transform.pos.z }; }
	void SetHp(int hp) { m_hp = hp; }
};