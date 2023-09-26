#include "DestructibleObjectMgr.h"
#include "DestructibleTree.h"
#include "../Game/Player.h"

void DestructibleObjectMgr::Setting()
{

	for (auto& index : m_trees) {

		index = std::make_shared<DestructibleTree>();

	}

}

void DestructibleObjectMgr::Init()
{

	for (auto& index : m_trees) {

		index->Init();

	}

}

void DestructibleObjectMgr::DebugGenerate()
{

	m_trees[0]->Generate(KazMath::Vec3<float>(-20, 40, 0));

}

void DestructibleObjectMgr::GenerateTree(KazMath::Vec3<float> arg_pos)
{

	for (auto& tree : m_trees) {

		if (tree->GetIsActive()) continue;

		tree->Generate(arg_pos);

		break;

	}

}

void DestructibleObjectMgr::Update(std::weak_ptr<Player> arg_player)
{

	for (auto& index : m_trees) {

		if (!index->GetIsActive()) continue;

		index->Update(arg_player);

	}

}

void DestructibleObjectMgr::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{

	for (auto& index : m_trees) {

		if (!index->GetIsActive()) continue;

		index->Draw(arg_rasterize, arg_blasVec);

	}

}

int DestructibleObjectMgr::GetTargetTreeIndex(KazMath::Vec3<float> arg_playerPos, float arg_targetRange, float& arg_targetDistance)
{

	//生成している岩のなかからターゲットの範囲内のものを検索する。
	std::vector<int> inTargetIndex;
	std::vector<float> inTargetLength;
	arg_targetDistance = 1000000.0f;
	for (auto& index : m_trees) {

		if (!index->GetIsActive()) continue;

		//ターゲットの中に入っているかをチェックする。
		float length = KazMath::Vec3<float>(arg_playerPos - index->GetPosZeroY()).Length();
		bool inTarget = length < arg_targetRange;
		if (!inTarget) continue;

		//インデックスを取得。
		inTargetIndex.emplace_back(static_cast<int>(&index - &m_trees[0]));
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

KazMath::Vec3<float> DestructibleObjectMgr::GetTreeScale(int arg_index)
{
	return m_trees[arg_index]->GetScale();
}

KazMath::Vec3<float> DestructibleObjectMgr::GetTreeTargetScale(int arg_index)
{
	return m_trees[arg_index]->GetTargetScale();
}

KazMath::Vec3<float> DestructibleObjectMgr::GetTreePosZeroY(int arg_index)
{
	return m_trees[arg_index]->GetPosZeroY();
}
