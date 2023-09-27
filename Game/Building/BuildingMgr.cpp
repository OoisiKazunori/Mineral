#include "BuildingMgr.h"
#include "Wall.h"
#include "../Game/Enemy/EnemyMgr.h"

void BuildingMgr::Setting()
{

	for (auto& index : m_walls) {

		index = std::make_shared<Wall>();

	}

}

void BuildingMgr::Init()
{

	for (auto& index : m_walls) {

		index->Init();

	}

}

void BuildingMgr::Generate()
{

	//壁A
	m_walls[0]->Genrate(KazMath::Vec3<float>(46, 10, 57), 47, 0);
	//壁B
	m_walls[1]->Genrate(KazMath::Vec3<float>(-53, 10, 91), 163, 1);
	//壁C
	m_walls[2]->Genrate(KazMath::Vec3<float>(-130, 10, -160), 100, 2);

}

void BuildingMgr::Update(std::weak_ptr<Player> arg_player, BulidSmokeEmitter& arg_smokeEffect, int* arg_smokeIndex)
{

	for (auto& index : m_walls) {

		if (index->GetIsBuildTrigger())
		{
			*arg_smokeIndex += 1;
			arg_smokeEffect.Init(
				index->GetPosZeroY() + KazMath::Vec3<float>(0.0f, 5.0f, 0.0f),
				30.0f
			);
		}

		index->Update(arg_player);
	}

}

void BuildingMgr::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{

	for (auto& index : m_walls) {

		index->Draw(arg_rasterize, arg_blasVec);

	}

}

int BuildingMgr::GetTargetWallIndex(KazMath::Vec3<float> arg_playerPos, float arg_targetRange, float& arg_targetDistance)
{

	//生成している岩のなかからターゲットの範囲内のものを検索する。
	std::vector<int> inTargetIndex;
	std::vector<float> inTargetLength;
	arg_targetDistance = 1000000.0f;
	for (auto& index : m_walls) {

		if (!index->GetIsActive()) continue;
		if (index->GetNowLevel() == index->GetMAXLevel()) continue;
		if (!index->GetIsReady()) continue;

		//ターゲットの中に入っているかをチェックする。
		float length = KazMath::Vec3<float>(arg_playerPos - index->GetPosZeroY()).Length();
		bool inTarget = length < arg_targetRange;
		if (!inTarget) continue;

		//インデックスを取得。
		inTargetIndex.emplace_back(static_cast<int>(&index - &m_walls[0]));
		inTargetLength.emplace_back(length);

	}

	//検索結果が0だったら範囲内に岩がないということなので、-1を返す。
	const int INDEX_COUNT = static_cast<int>(inTargetIndex.size());
	if (INDEX_COUNT <= 0) {

		return -1;

	}

	//検索した結果から最短のものを選ぶ。
	int miniIndex = -1;
	float miniLength = arg_targetRange * 2.0f;
	for (int index = 0; index < INDEX_COUNT; ++index) {

		//保存されている値より大きかったら処理を飛ばす。
		if (miniLength < inTargetLength[index]) continue;

		//保存する。
		miniLength = inTargetLength[index];
		miniIndex = inTargetIndex[index];

	}

	arg_targetDistance = miniLength;
	return miniIndex;
}

KazMath::Vec3<float> BuildingMgr::GetWallPosZeroY(int arg_index)
{
	return m_walls[arg_index]->GetPosZeroY();
}

KazMath::Vec3<float> BuildingMgr::GetWallScale(int arg_index)
{
	return m_walls[arg_index]->GetScale();
}

int BuildingMgr::GetMaterialCounterWall(int arg_index)
{
	return m_walls[arg_index]->GetMaterialCounter();
}

void BuildingMgr::AddMaterialWall(int arg_index)
{
	m_walls[arg_index]->AddMaterial();
}

bool BuildingMgr::IsFullMaterialWall(int arg_index)
{
	return m_walls[arg_index]->IsFullMaterial();
}

bool BuildingMgr::GetIsBuildWall(int arg_index) {

	return m_walls[arg_index]->GetIsBuild();
}

bool BuildingMgr::GetIsKnockBackWall(int arg_index)
{
	return m_walls[arg_index]->GetIsKnockBackTrigger();
}

void BuildingMgr::CheckNockBack(std::weak_ptr<EnemyMgr> arg_enemyMgr)
{

	for (auto& wallIndex : m_walls) {

		if (!wallIndex->GetIsKnockBackTrigger()) continue;

		arg_enemyMgr.lock()->CheckNockBack(wallIndex->GetPosZeroY(), 20.0f);

	}

}

MeshCollision::CheckHitResult BuildingMgr::CheckHitWall(KazMath::Vec3<float> arg_rayPos, KazMath::Vec3<float> arg_rayDir, float arg_rayLength, int& arg_wallIndex) {

	MeshCollision::CheckHitResult checkHitResult;
	checkHitResult.m_isHit = false;

	const int WALL_COUNT = static_cast<int>(m_walls.size());
	for (int index = 0; index < WALL_COUNT; ++index) {

		if (!m_walls[index]->GetIsBuild()) continue;

		checkHitResult = m_walls[index]->CheckHitMesh(arg_rayPos, arg_rayDir);
		if (checkHitResult.m_isHit && 0 < checkHitResult.m_distance && checkHitResult.m_distance <= arg_rayLength) {

			arg_wallIndex = index;
			return checkHitResult;

		}

	}

	checkHitResult.m_isHit = false;
	return checkHitResult;

}

void BuildingMgr::DamageWall(int arg_index, int arg_damage) {
	m_walls[arg_index]->Damage(arg_damage);
}

int BuildingMgr::GetWallHP(int arg_index) {
	return m_walls[arg_index]->GetHP();
}