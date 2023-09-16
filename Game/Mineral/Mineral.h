#pragma once
#include "../Game/DrawCallSet.h"
#include "../KazLibrary/Sound/SoundManager.h"

class Player;
class Rock;
class MineKuji;
class MineTsumuri;
class DestructibleTree;
class BuildingMaterial;

class Mineral {

public:

	enum MINERAL_ID {
		SMALL,
		MEDIUM,
		BIG,
	};

private:
	SoundData rock_break01;
	SoundData rock_break02;
	SoundData have_material_se;
	SoundData walk;
	SoundData attack;

	int move_span_count;
	const int MOVE_SPAN_COUNT_MAX = 3;

	//基本情報
	std::array<DrawCallSet, 3> m_model;
	KazMath::Transform3D m_oldTransform;
	KazMath::Transform3D m_transform;
	KazMath::Vec3<float> m_forwardVec;			//正面ベクトル
	DrawCallSet m_surprisedModel;
	KazMath::Transform3D m_surpisedTransform;
	KazMath::Vec3<float> m_respawnVec;
	float m_gravity;
	const float GRAVITY = 0.2f;
	float m_gatheringTriggerEffect;
	const float GATHERING_TRIGGER_EFFECT = 2.0f;
	const float STRONG_DAIPAN_POWER = 2.0f;
	const float GATHERING_DAIPAN_POWER = 1.0f;	//すでに追従しているときの、反応用の力
	int m_hp;
	std::array<int, 3> HP = { 2, 6, 45 };
	std::weak_ptr<Mineral> m_thisMineral;	//自分自身

	//解散するときのベクトル
	KazMath::Vec3<float> m_breakUpVec;
	const float BREAK_UP_POWER = 1.5f;

	//移動関連
	KazMath::Vec3<float> m_moveVec;
	int m_moveSpan;
	int m_randomMoveSpan;
	const int MOVE_SPAN = 1;
	std::array<const float, 3> MOVE_SPEED = { 3.0f, 2.0f, 0.1f };
	std::array<const int, 3> ATTACK_DAMAGE = { 1, 3, 9 };
	KazMath::Vec3<float> m_wallJump;	//壁超えの大ジャンプ

	//ステータス
	bool m_isAlive;
	bool m_isOldGathering;
	bool m_isGathering;		//隊列を組んで居る状態か。
	bool m_canGathering;	//隊列不可能フラグ。リスポーンしてから接地するまでは隊列に入らない。
	MINERAL_ID m_mineralID;

	//建材を運んでいる時
	std::weak_ptr<BuildingMaterial> m_haveMaterial;
	bool m_isGoToGetMaterial;

	//サイズ関係 描画
	const float MINERAL_DRAW_SIZE_BIG = 7.0f;
	const float MINERAL_DRAW_SIZE_MEDIUM = 4.5f;
	const float MINERAL_DRAW_SIZE_SMALL = 3.0f;
	//当たり判定
	const float MINERAL_COLLISION_SIZE_BIG = 7.0f;
	const float MINERAL_COLLISION_SIZE_MEDIUM = 5.0f;
	const float MINERAL_COLLISION_SIZE_SMALL = 3.0f;
	//サイズごとに埋まりの対策のためにY軸を持ち上げる量。
	std::array<const float, 3> MINERAL_OFFSETY = { 1.0f, 6.0f, 15.0f };

	//攻撃関係
	std::weak_ptr<Rock> m_attackTargetRock;
	std::weak_ptr<MineKuji> m_attackTargetMineKuji;
	std::weak_ptr<MineTsumuri> m_attackTargetMineTsumuri;
	std::weak_ptr<DestructibleTree> m_attackDestrutibleTree;
	KazMath::Vec3<float> m_attackReactionVec;
	float m_attackMoveSpeed;
	int m_stayDelayTimer;
	int m_stayDelay;
	const int MIN_STAY_DELAY = 60;
	const int MAX_STAY_DELAY = 90;
	const float ADD_ATTACK_SPEED = 0.05f;
	const float MAX_ATTACK_SPEED = 1.5f;
	bool m_isAttack;
	enum ATTACK_ID {
		ATTACK,
		STAY,
	}m_attackID;

	//ビックリマーク関連
	bool m_isDrawSurpised;	//ビックリマークを描画するかの判断。
	float m_surpisedTimer;
	enum class SURPRISED_STATUS {
		APPEAR,
		STAY,
		EXIT,
	}m_surprisedStatus;
	const float SURPRISED_APPEAR_TIMER = 15.0f;
	const float SURPRISED_STAY_TIMER = 10.0f;
	const float SURPRISED_EXIT_TIMER = 15.0f;


public:

	Mineral();

	void Init();

	void Generate(std::weak_ptr<Mineral> arg_thisMineral, KazMath::Vec3<float> arg_pos, KazMath::Vec3<float> arg_respawnVec, int arg_mineralID = MEDIUM);

	void Update(std::weak_ptr<Player> arg_player, std::vector<std::pair<KazMath::Vec3<float>, int>>& arg_respawnVec, int& arg_moveSECount);

	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	void Damage(int arg_damage = 1);
	int GetHP() { return m_hp; }

	//重なり対策。
	void CounterOverlap(KazMath::Vec3<float> arg_centerPos, KazMath::Vec3<float>& arg_mineralPos, float arg_scale);

	//解散させる。
	void BreakUP(KazMath::Vec3<float> arg_forwardVec);

	//攻撃する。
	void Attack(std::weak_ptr<Rock> arg_attackTargetRock);
	void Attack(std::weak_ptr<MineKuji> arg_attackTargetMinekuji);
	void Attack(std::weak_ptr<MineTsumuri> arg_attackTargetMineTsumuri);
	void Attack(std::weak_ptr<DestructibleTree> arg_destructibleTree);

	//マテリアルを持つ。
	void HaveMaterial(std::weak_ptr<BuildingMaterial> arg_material);

	//マテリアルを落とす。
	void DropMaterial();
	void EraseTheMaterial();

	bool GetIsAlive() { return m_isAlive; }
	bool GetIsGathering() { return m_isGathering; }
	bool GetIsGatheringTrigger() { return !m_isOldGathering && m_isGathering; }
	bool GetIsAttack() { return m_isAttack; }
	bool GetHaveMaterial() { return !m_haveMaterial.expired(); }
	MINERAL_ID GetMineralID() { return m_mineralID; }
	KazMath::Vec3<float> GetPos() { return m_transform.pos; }
	KazMath::Vec3<float> GetPosZeroY() { return KazMath::Vec3<float>(m_transform.pos.x, 0.0f, m_transform.pos.z); }
	KazMath::Vec3<float> GetScale() { return m_transform.scale; }
	float GetCollisionScale();
	KazMath::Vec3<float>& GetPosRef() { return m_transform.pos; }

private:

	void UpdateAttack(std::weak_ptr<Player> arg_player, int& arg_moveSECount);
	void UpdateHaveMaterial(std::weak_ptr<Player> arg_player);

};