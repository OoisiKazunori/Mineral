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
	//生成している岩のなかからターゲットの範囲内のものを検索する。
	std::vector<int> inTargetIndex;
	std::vector<float> inTargetLength;
	arg_targetDistance = 1000000.0f;
	for (auto& index : m_materials) {

		if (!index->GetIsActive()) continue;
		if (index->GetIsHeld()) continue;

		//ターゲットの中に入っているかをチェックする。
		float length = KazMath::Vec3<float>(arg_playerPos - index->GetPosZeroY()).Length();
		bool inTarget = length < arg_targetRange;
		if (!inTarget) continue;

		//インデックスを取得。
		inTargetIndex.emplace_back(static_cast<int>(&index - &m_materials[0]));
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

KazMath::Vec3<float> BuildingMaterialMgr::GetMaterialScale(int arg_index)
{
	return m_materials[arg_index]->GetScale();
}

KazMath::Vec3<float> BuildingMaterialMgr::GetMaterialPosZeroY(int arg_index)
{
	return m_materials[arg_index]->GetPosZeroY();
}
