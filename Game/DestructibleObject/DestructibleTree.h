#pragma once
#include "../Game/DrawCallSet.h"
#include "../KazLibrary/Sound/SoundManager.h"

class Player;

class DestructibleTree {

private:
	SoundData break_se;

	DrawCallSet m_model;
	KazMath::Transform3D m_transform;
	bool m_isActive;

	int m_hp;
	const int HP = 10;

	//ターゲット表示するときのスケール
	const float TARGET_SCALE = 10.0f;

public:

	DestructibleTree();

	void Init();

	void Update(std::weak_ptr<Player> arg_player);

	void Generate(KazMath::Vec3<float> arg_generatePos);

	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	void Damage(int arg_damage);

	bool GetIsActive() { return m_isActive; }
	KazMath::Vec3<float> GetPos() { return m_transform.pos; }
	KazMath::Vec3<float> GetPosZeroY() { return KazMath::Vec3<float>(m_transform.pos.x, 0.0f, m_transform.pos.z); }
	KazMath::Vec3<float> GetScale() { return m_transform.scale; }
	KazMath::Vec3<float> GetTargetScale() { return KazMath::Vec3<float>(TARGET_SCALE, TARGET_SCALE, TARGET_SCALE); }
	int GetHP() { return m_hp; }

};