#include "MineralTarget.h"
#include "../Game/Rock/RockMgr.h"
#include "../Game/Enemy/EnemyMgr.h"
#include "../Game/DestructibleObject/DestructibleObjectMgr.h"
#include "../Game/BuildingMaterial/BuildingMaterialMgr.h"
#include "../Game/BuildingMaterial/BuildingMaterial.h"
#include "../Game/Building/BuildingMgr.h"
#include "../Game/Rock/RockMgr.h"
#include "../Game/Enemy/MineTsumuri.h"

MineralTarget::MineralTarget()
{
	m_targetRockIndex = -1;
	m_targetMinekujiIndex = -1;
	m_oldTargetMineKujiIndex = -1;
	m_targetSineWaveTimer = 0.0f;

	Init();

	m_model.LoadNoLighting("Resource/Player/HandCircle/", "HandCircleStrong.gltf");
	m_targetModel.LoadNoLighting("Resource/Mineral/Target/", "Target.gltf");

}

void MineralTarget::Init()
{
	m_targetRockIndex = -1;
	m_targetMinekujiIndex = -1;
	m_targetTreeIndex = -1;
	m_targetMaterialIndex = -1;
	m_targetBuildingIndex = -1;
	m_targetMineTsumuriIndex = -1;

	m_oldTargetRockIndex = -1;
	m_oldTargetMineKujiIndex = -1;
	m_oldTargetTreeIndex = -1;
	m_oldTargetMaterialIndex = -1;
	m_oldTargetBuildingIndex = -1;
	m_oldTargetMineTsumuriIndex = -1;

	m_targetRockDistance = 1000000.0f;
	m_targetMinekujiDistance = 1000000.0f;
	m_targetTreeDistance = 1000000.0f;
	m_targetMaterialDistance = 1000000.0f;
	m_targetBuildingDistance = 1000000.0f;
	m_targetMineTsumuriDistance = 1000000.0f;

	m_targetSineWaveTimer = 0.0f;
}

void MineralTarget::Update(KazMath::Vec3<float> arg_playerPos, float arg_targetRange, std::weak_ptr<EnemyMgr> arg_enemyMgr)
{

	//岩のターゲットを計算。
	m_oldTargetRockIndex = m_targetRockIndex;
	m_targetRockIndex = RockMgr::Instance()->GetTargetRockIndex(arg_playerPos, arg_targetRange, m_targetRockDistance);

	//ミネクジのターゲットを計算
	m_oldTargetMineKujiIndex = m_targetMinekujiIndex;
	m_targetMinekujiIndex = arg_enemyMgr.lock()->GetTargetMineKujiIndex(arg_playerPos, arg_targetRange, m_targetMinekujiDistance);

	//ミネクジのターゲットを計算
	m_oldTargetTreeIndex = m_targetTreeIndex;
	m_targetTreeIndex = DestructibleObjectMgr::Instance()->GetTargetTreeIndex(arg_playerPos, arg_targetRange, m_targetTreeDistance);

	//マテリアルのターゲットを計算
	m_oldTargetMaterialIndex = m_targetMaterialIndex;
	m_targetMaterialIndex = BuildingMaterialMgr::Instance()->GetTargetMaterialIndex(arg_playerPos, arg_targetRange, m_targetMaterialDistance);

	//建築物を計算
	m_oldTargetBuildingIndex = m_targetBuildingIndex;
	m_targetBuildingIndex = BuildingMgr::Instance()->GetTargetWallIndex(arg_playerPos, arg_targetRange, m_targetBuildingDistance);

	//ミネクジを計算
	m_oldTargetMineTsumuriIndex = m_targetMineTsumuriIndex;
	m_targetMineTsumuriIndex = arg_enemyMgr.lock()->GetTargetMineTsumuriIndex(arg_playerPos, arg_targetRange, m_targetMineTsumuriDistance);

	//ターゲットが変わった瞬間にTargetとかのスケールを小さくする。
	if ((m_oldTargetRockIndex != m_targetRockIndex) || 
		(m_oldTargetMineKujiIndex != m_targetMinekujiIndex) || 
		(m_oldTargetTreeIndex != m_targetTreeIndex) || 
		(m_oldTargetMaterialIndex != m_targetMaterialIndex) ||
		(m_oldTargetBuildingIndex != m_targetBuildingIndex) ||
		(m_oldTargetMineTsumuriIndex != m_targetMineTsumuriIndex)) {

		m_targetTransform.scale = {};
		m_transform.scale = {};

	}

	//ターゲットの矢印をY軸回転させておく。
	m_targetTransform.rotation.y += 8.0f;

	//Y軸にサイン波の動きもさせる。
	m_targetSineWaveTimer += TARGET_SINE_WAVE_TIMER;

	//岩のクラスの参照を保存。
	m_enemyMgr = arg_enemyMgr;

}

void MineralTarget::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec, std::weak_ptr<EnemyMgr> arg_enemyMgr)
{

	//ターゲットが存在したら
	int targetIndex = GetCanTarget();
	if (targetIndex == 1) {

		//大きさを設定
		const float SCALE = 2.5f;
		m_transform.scale += ((RockMgr::Instance()->GetScale(m_targetRockIndex) + KazMath::Vec3<float>(SCALE, SCALE, SCALE)) - m_transform.scale) / 3.0f;
		const float TARGET_SCALE = 10.0f;
		m_targetTransform.scale += (KazMath::Vec3<float>(TARGET_SCALE, TARGET_SCALE, TARGET_SCALE) - m_targetTransform.scale) / 5.0f;

		//座標を設定。
		m_transform.pos = RockMgr::Instance()->GetPos(m_targetRockIndex);
		m_transform.pos.y = 0.0f;
		m_targetTransform.pos = RockMgr::Instance()->GetPos(m_targetRockIndex);
		const float TARGET_HEIGHT = 20.0f;
		m_targetTransform.pos.y += TARGET_HEIGHT + (sinf(m_targetSineWaveTimer) * TARGET_SINE_WAVE_LENGTH);

	}
	else if (targetIndex == 2) {

		//大きさを設定
		const float SCALE = 1.0f;
		m_transform.scale += ((arg_enemyMgr.lock()->GetMineKujiTargetScale(m_targetMinekujiIndex) + KazMath::Vec3<float>(SCALE, SCALE, SCALE)) - m_transform.scale) / 3.0f;
		const float TARGET_SCALE = 10.0f;
		m_targetTransform.scale += (KazMath::Vec3<float>(TARGET_SCALE, TARGET_SCALE, TARGET_SCALE) - m_targetTransform.scale) / 5.0f;

		//座標を設定。
		m_transform.pos = arg_enemyMgr.lock()->GetMineKujiPos(m_targetMinekujiIndex);
		m_transform.pos.y = 0.0f;
		m_targetTransform.pos = arg_enemyMgr.lock()->GetMineKujiPos(m_targetMinekujiIndex);
		const float TARGET_HEIGHT = 20.0f;
		m_targetTransform.pos.y += TARGET_HEIGHT + (sinf(m_targetSineWaveTimer) * TARGET_SINE_WAVE_LENGTH);

	}
	else if (targetIndex == 3) {

		//大きさを設定
		const float SCALE = 1.0f;
		m_transform.scale += ((DestructibleObjectMgr::Instance()->GetTreeTargetScale(m_targetTreeIndex) + KazMath::Vec3<float>(SCALE, SCALE, SCALE)) - m_transform.scale) / 3.0f;
		const float TARGET_SCALE = 10.0f;
		m_targetTransform.scale += (KazMath::Vec3<float>(TARGET_SCALE, TARGET_SCALE, TARGET_SCALE) - m_targetTransform.scale) / 5.0f;

		//座標を設定。
		m_transform.pos = DestructibleObjectMgr::Instance()->GetTreePosZeroY(m_targetTreeIndex);
		m_transform.pos.y = 0.0f;
		m_targetTransform.pos = DestructibleObjectMgr::Instance()->GetTreePosZeroY(m_targetTreeIndex);
		const float TARGET_HEIGHT = 50.0f;
		m_targetTransform.pos.y += TARGET_HEIGHT + (sinf(m_targetSineWaveTimer) * TARGET_SINE_WAVE_LENGTH);

	}
	else if (targetIndex == 4) {

		//大きさを設定
		const float SCALE = 3.0f;
		m_transform.scale += ((BuildingMaterialMgr::Instance()->GetMaterialScale(m_targetMaterialIndex) + KazMath::Vec3<float>(SCALE, SCALE, SCALE)) - m_transform.scale) / 3.0f;
		const float TARGET_SCALE = 10.0f;
		m_targetTransform.scale += (KazMath::Vec3<float>(TARGET_SCALE, TARGET_SCALE, TARGET_SCALE) - m_targetTransform.scale) / 5.0f;

		//座標を設定。
		m_transform.pos = BuildingMaterialMgr::Instance()->GetMaterialPosZeroY(m_targetMaterialIndex);
		m_transform.pos.y = 0.0f;
		m_targetTransform.pos = BuildingMaterialMgr::Instance()->GetMaterialPosZeroY(m_targetMaterialIndex);
		const float TARGET_HEIGHT = 10.0f;
		m_targetTransform.pos.y += TARGET_HEIGHT + (sinf(m_targetSineWaveTimer) * TARGET_SINE_WAVE_LENGTH);

	}
	else if (targetIndex == 5) {

		//大きさを設定
		const float SCALE = 3.0f;
		m_transform.scale += ((BuildingMgr::Instance()->GetWallScale(m_targetBuildingIndex) + KazMath::Vec3<float>(SCALE, SCALE, SCALE)) - m_transform.scale) / 3.0f;
		const float TARGET_SCALE = 10.0f;
		m_targetTransform.scale += (KazMath::Vec3<float>(TARGET_SCALE, TARGET_SCALE, TARGET_SCALE) - m_targetTransform.scale) / 5.0f;

		//座標を設定。
		m_transform.pos = BuildingMgr::Instance()->GetWallPosZeroY(m_targetBuildingIndex);
		m_transform.pos.y = 0.0f;
		m_targetTransform.pos = BuildingMgr::Instance()->GetWallPosZeroY(m_targetBuildingIndex);
		const float TARGET_HEIGHT = 25.0f;
		m_targetTransform.pos.y += TARGET_HEIGHT + (sinf(m_targetSineWaveTimer) * TARGET_SINE_WAVE_LENGTH);

	}
	else if (targetIndex == 6) {

		//大きさを設定
		float SCALE = 1.0f;
		if (arg_enemyMgr.lock()->GetIsMineking(m_targetMineTsumuriIndex)) {
			SCALE += 20.0f;
		}
		m_transform.scale += ((arg_enemyMgr.lock()->GetMineTsumuriTargetScale(m_targetMineTsumuriIndex) + KazMath::Vec3<float>(SCALE, SCALE, SCALE)) - m_transform.scale) / 3.0f;
		float TARGET_SCALE = 10.0f;
		if (arg_enemyMgr.lock()->GetIsMineking(m_targetMineTsumuriIndex)) {
			TARGET_SCALE += 10.0f;
		}
		m_targetTransform.scale += (KazMath::Vec3<float>(TARGET_SCALE, TARGET_SCALE, TARGET_SCALE) - m_targetTransform.scale) / 5.0f;

		//座標を設定。
		m_transform.pos = arg_enemyMgr.lock()->GetMineTsumuriPos(m_targetMineTsumuriIndex);
		m_transform.pos.y = 0.0f;
		m_targetTransform.pos = arg_enemyMgr.lock()->GetMineTsumuriPos(m_targetMineTsumuriIndex);
		float TARGET_HEIGHT = 20.0f;
		if (arg_enemyMgr.lock()->GetIsMineking(m_targetMineTsumuriIndex)) {
			TARGET_HEIGHT += 25.0f;
		}
		m_targetTransform.pos.y += TARGET_HEIGHT + (sinf(m_targetSineWaveTimer) * TARGET_SINE_WAVE_LENGTH);

	}
	else {
		m_transform.scale += (KazMath::Vec3<float>(0, 0, 0) - m_transform.scale) / 3.0f;
		m_targetTransform.scale += (KazMath::Vec3<float>(0, 0, 0) - m_targetTransform.scale) / 3.0f;
	}

	m_transform.pos.y = 2.0f;
	m_model.Draw(arg_rasterize, arg_blasVec, m_transform);
	m_targetModel.Draw(arg_rasterize, arg_blasVec, m_targetTransform);

}

int MineralTarget::GetCanTarget()
{

	//岩のインデックスに値が入っていたら。
	if (100000 <= m_targetMinekujiDistance && 
		100000 <= m_targetRockDistance && 
		100000 <= m_targetTreeDistance && 
		100000 <= m_targetMaterialDistance &&
		100000 <= m_targetBuildingDistance &&
		100000 <= m_targetMineTsumuriDistance) {
		return 0;
	}

	//距離の情報を保存しておく。
	std::vector<float> distanceVector;
	distanceVector.emplace_back(m_targetRockDistance);
	distanceVector.emplace_back(m_targetMinekujiDistance);
	distanceVector.emplace_back(m_targetTreeDistance);
	distanceVector.emplace_back(m_targetMaterialDistance);
	distanceVector.emplace_back(m_targetBuildingDistance);
	distanceVector.emplace_back(m_targetMineTsumuriDistance);

	//昇順にソート
	std::sort(distanceVector.begin(), distanceVector.end());

	if (distanceVector.front() == m_targetRockDistance) {
		return 1;
	}
	if (distanceVector.front() == m_targetMinekujiDistance) {
		return 2;
	}
	if (distanceVector.front() == m_targetTreeDistance) {
		return 3;
	}
	if (distanceVector.front() == m_targetMaterialDistance) {
		return 4;
	}
	if (distanceVector.front() == m_targetBuildingDistance) {
		return 5;
	}
	if (distanceVector.front() == m_targetMineTsumuriDistance) {
		return 6;
	}
	return 0;
}

std::weak_ptr<Rock> MineralTarget::GetTargetRock()
{
	return RockMgr::Instance()->GetRock(m_targetRockIndex);
}

std::weak_ptr<MineKuji> MineralTarget::GetTargetMinekuji()
{
	return m_enemyMgr.lock()->GetMineKuji(m_targetMinekujiIndex);
}

std::weak_ptr<MineTsumuri> MineralTarget::GetTargetMineTsumuri()
{
	return m_enemyMgr.lock()->GetMineTsumuri(m_targetMineTsumuriIndex);
}

std::weak_ptr<DestructibleTree> MineralTarget::GetTargetTree()
{
	return DestructibleObjectMgr::Instance()->GetTree(m_targetTreeIndex);
}

std::weak_ptr<BuildingMaterial> MineralTarget::GetTargetMaterial()
{
	return BuildingMaterialMgr::Instance()->GetMaterial(m_targetMaterialIndex);
}
