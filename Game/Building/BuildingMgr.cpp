#include "BuildingMgr.h"
#include "Wall.h"

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

	//��A
	m_walls[0]->Genrate(KazMath::Vec3<float>(46, 10, 57), 47, 0);
	//��B
	m_walls[1]->Genrate(KazMath::Vec3<float>(-53, 10, 91), 163, 1);
	//��C
	m_walls[2]->Genrate(KazMath::Vec3<float>(-130, 10, -160), 100, 2);

}

void BuildingMgr::Update(std::weak_ptr<Player> arg_player)
{

	for (auto& index : m_walls) {

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

	//�������Ă����̂Ȃ�����^�[�Q�b�g�͈͓̔��̂��̂���������B
	std::vector<int> inTargetIndex;
	std::vector<float> inTargetLength;
	arg_targetDistance = 1000000.0f;
	for (auto& index : m_walls) {

		if (!index->GetIsActive()) continue;
		if (index->GetIsBuild()) continue;
		if (!index->GetIsReady()) continue;

		//�^�[�Q�b�g�̒��ɓ����Ă��邩���`�F�b�N����B
		float length = KazMath::Vec3<float>(arg_playerPos - index->GetPosZeroY()).Length();
		bool inTarget = length < arg_targetRange;
		if (!inTarget) continue;

		//�C���f�b�N�X���擾�B
		inTargetIndex.emplace_back(static_cast<int>(&index - &m_walls[0]));
		inTargetLength.emplace_back(length);

	}

	//�������ʂ�0��������͈͓��Ɋ₪�Ȃ��Ƃ������ƂȂ̂ŁA-1��Ԃ��B
	const int INDEX_COUNT = static_cast<int>(inTargetIndex.size());
	if (INDEX_COUNT <= 0) {

		return -1;

	}

	//�����������ʂ���ŒZ�̂��̂�I�ԁB
	int miniIndex = -1;
	float miniLength = arg_targetRange * 2.0f;
	for (int index = 0; index < INDEX_COUNT; ++index) {

		//�ۑ�����Ă���l���傫�������珈�����΂��B
		if (miniLength < inTargetLength[index]) continue;

		//�ۑ�����B
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