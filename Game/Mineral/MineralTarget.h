#pragma once
#include "../Game/DrawCallSet.h"

class Rock;
class EnemyMgr;
class MineKuji;
class MineTsumuri;
class DestructibleTree;
class BuildingMaterialMgr;
class BuildingMaterial;

class MineralTarget {

private:

	//岩関連
	int m_oldTargetRockIndex;
	int m_targetRockIndex;	//ターゲットが岩の場合のインデックス。ターゲットじゃないときは-1を入れることで岩か敵のどれがターゲットされているのかを判断する。
	float m_targetRockDistance;
	//ミネクジ関連
	int m_oldTargetMineKujiIndex;
	int m_targetMinekujiIndex;	//ターゲットがミネクジの場合のインデックス。ターゲットじゃないときは-1を入れることでミネクジか敵のどれがターゲットされているのかを判断する。
	float m_targetMinekujiDistance;
	//ミネツムリ関連
	int m_oldTargetMineTsumuriIndex;
	int m_targetMineTsumuriIndex;	//ターゲットがミネクジの場合のインデックス。ターゲットじゃないときは-1を入れることでミネクジか敵のどれがターゲットされているのかを判断する。
	float m_targetMineTsumuriDistance;
	//破壊可能木関連
	int m_oldTargetTreeIndex;
	int m_targetTreeIndex;	//ターゲットがミネクジの場合のインデックス。ターゲットじゃないときは-1を入れることでミネクジか敵のどれがターゲットされているのかを判断する。
	float m_targetTreeDistance;
	//建材
	int m_oldTargetMaterialIndex;
	int m_targetMaterialIndex;	//ターゲットがミネクジの場合のインデックス。ターゲットじゃないときは-1を入れることでミネクジか敵のどれがターゲットされているのかを判断する。
	float m_targetMaterialDistance;
	//建築物
	int m_oldTargetBuildingIndex;
	int m_targetBuildingIndex;	//ターゲットがミネクジの場合のインデックス。ターゲットじゃないときは-1を入れることでミネクジか敵のどれがターゲットされているのかを判断する。
	float m_targetBuildingDistance;

	//ロックオン対象を描画する用のモデル
	DrawCallSet m_model;
	DrawCallSet m_targetModel;

	KazMath::Transform3D m_transform;
	KazMath::Transform3D m_targetTransform;

	//ロックオン対象に出す矢印の変数
	float m_targetSineWaveTimer;
	const float TARGET_SINE_WAVE_TIMER = 0.25f;
	const float TARGET_SINE_WAVE_LENGTH = 1.0f;

	std::weak_ptr<EnemyMgr> m_enemyMgr;

public:

	MineralTarget();

	void Init();

	void Update(KazMath::Vec3<float> arg_playerPos, float arg_targetRange, std::weak_ptr<EnemyMgr> arg_enemyMgr);

	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec, std::weak_ptr<EnemyMgr> arg_enemyMgr);

	//ターゲット可能かを返す。 0...無理 1...岩
	int GetCanTarget();

	//ターゲット可能な岩のインデックス
	int GetTargetRockIndex() { return m_targetRockIndex; };

	//ターゲット可能な岩の参照を返す。
	std::weak_ptr<Rock> GetTargetRock();
	std::weak_ptr<MineKuji> GetTargetMinekuji();
	std::weak_ptr<MineTsumuri> GetTargetMineTsumuri();
	std::weak_ptr<DestructibleTree> GetTargetTree();
	std::weak_ptr<BuildingMaterial> GetTargetMaterial();

	int GetTargetBuilidngIndex() { return m_targetBuildingIndex; }

};