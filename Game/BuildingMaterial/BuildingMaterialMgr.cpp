#include "BuildingMaterialMgr.h"
#include "BuildingMaterial.h"

void BuildingMaterialMgr::Setting()
{

	for (auto& index : m_materials) {

		index = std::make_shared<BuildingMaterial>();

	}

}

void BuildingMaterialMgr::Init()
{

	for (auto& index : m_materials) {

		index->Init();

	}

}

void BuildingMaterialMgr::Generate(KazMath::Vec3<float> arg_pos, KazMath::Vec3<float> arg_generateVec)
{

	for (auto& index : m_materials) {

		if (index->GetIsActive()) continue;

		index->Generate(arg_pos, arg_generateVec);

		break;

	}

}

void BuildingMaterialMgr::Update()
{

	for (auto& index : m_materials) {

		if (!index->GetIsActive()) continue;

		index->Update();

	}

}

void BuildingMaterialMgr::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{

	for (auto& index : m_materials) {

		index->Draw(arg_rasterize, arg_blasVec);

	}

}

int BuildingMaterialMgr::GetTargetMaterialIndex(KazMath::Vec3<float> arg_playerPos, float arg_targetRange, float& arg_targetDistance)
{
	//�������Ă����̂Ȃ�����^�[�Q�b�g�͈͓̔��̂��̂���������B
	std::vector<int> inTargetIndex;
	std::vector<float> inTargetLength;
	arg_targetDistance = 1000000.0f;
	for (auto& index : m_materials) {

		if (!index->GetIsActive()) continue;
		if (index->GetIsHeld()) continue;

		//�^�[�Q�b�g�̒��ɓ����Ă��邩���`�F�b�N����B
		float length = KazMath::Vec3<float>(arg_playerPos - index->GetPosZeroY()).Length();
		bool inTarget = length < arg_targetRange;
		if (!inTarget) continue;

		//�C���f�b�N�X���擾�B
		inTargetIndex.emplace_back(static_cast<int>(&index - &m_materials[0]));
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

KazMath::Vec3<float> BuildingMaterialMgr::GetMaterialScale(int arg_index)
{
	return m_materials[arg_index]->GetScale();
}

KazMath::Vec3<float> BuildingMaterialMgr::GetMaterialPosZeroY(int arg_index)
{
	return m_materials[arg_index]->GetPosZeroY();
}
