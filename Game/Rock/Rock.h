#pragma once
#include "../Game/DrawCallSet.h"
#include "../KazLibrary/Sound/SoundManager.h"

class Player;

class Rock {

public:

	enum ROCK_ID {
		SMALL,
		MEDIUM,
		BIG,
		MINERAL,
	};

private:
	SoundData rock_break01;
	SoundData rock_break02;

	//基本情報
	std::array<DrawCallSet, 4> m_model;
	int m_modelIndex;
	KazMath::Transform3D m_transform;
	KazMath::Vec3<float> m_respawnVec;
	float m_gravity;
	bool m_canCollision;
	bool m_canOldCollision;
	const float GRAVITY = 0.1f;
	const float STRONG_DAIPAN_POWER = 1.0f;
	int m_hp;
	std::array<int, 4> MAX_HP = { 1, 3, 6, 1 };
	KazMath::Vec3<float> m_reactionVec;

	//ステータス
	bool m_isAlive;
	ROCK_ID m_rockID;
	bool m_isMineralRock;

	//サイズ関係 描画
	const float DRAW_SIZE_BIG = 12.0f;
	const float DRAW_SIZE_MEDIUM = 5.5f;
	const float DRAW_SIZE_SMALL = 6.0f;
	//当たり判定
	const float COLLISION_SIZE_BIG = 12.0f;
	const float COLLISION_SIZE_MEDIUM = 4.6f;
	const float COLLISION_SIZE_SMALL = 6.0f;

public:


	Rock();

	void Init();

	void Generate(KazMath::Vec3<float> arg_pos, KazMath::Vec3<float> arg_respawnVec, bool arg_isMineralRock, int arg_rockID);

	void Update(std::weak_ptr<Player> arg_player, std::vector<std::pair<KazMath::Vec3<float>, int>>& arg_respawnVec);

	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	int GetHP() { return m_hp; }
	void Damage(KazMath::Vec3<float> arg_reactionVec, int arg_damage = 1);

	bool GetIsAlive() { return m_isAlive; }
	bool GetIsMineralRock() { return m_isMineralRock; }
	bool IsSmall() { return m_rockID == SMALL; }
	ROCK_ID GetRockID() { return m_rockID; }
	KazMath::Vec3<float> GetPos() { return m_transform.pos; }
	KazMath::Vec3<float> GetScale() { return m_transform.scale; }

};