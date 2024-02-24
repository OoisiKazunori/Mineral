#include "Mineral.h"
#include "MineralMgr.h"
#include "MineralTarget.h"
#include "../Game/DestructibleObject/DestructibleObjectMgr.h"
#include "../Player.h"
#include "../Building/BuildingMgr.h"
#include "../Enemy/EnemyMgr.h"
#include "../DestructibleObject/DestructibleTree.h"
#include "../DestructibleObject/DestructibleObjectMgr.h"
#include "../BuildingMaterial/BuildingMaterial.h"
#include "../BuildingMaterial/BuildingMaterialMgr.h"
#include "../Rock/Rock.h"
#include "../Rock/RockMgr.h"

void MineralMgr::Setting() {

	for (auto& index : m_minerals) {

		index = std::make_shared<Mineral>();

	}

}

void MineralMgr::DebugGenerate() {

	using namespace KazMath;

	std::vector<Vec3<float>> debugInfo;
	debugInfo.emplace_back(KazMath::Vec3<float>(169.0f, 10.0f, 105.0f) - KazMath::Vec3<float>(50.0f, 0.0f, 0.0f));

	for (auto& param : debugInfo) {

		for (auto& index : m_minerals) {

			if (index->GetIsAlive()) continue;

			index->Generate(index, param, {});

			break;

		}

	}

}

void MineralMgr::Init() {

	for (auto& index : m_minerals) {

		index->Init();

	}

}

void MineralMgr::Generate(KazMath::Vec3<float>& arg_pos, KazMath::Vec3<float>& arg_respawnDir, int arg_mineralID) {

	for (auto& index : m_minerals) {

		if (index->GetIsAlive()) continue;

		index->Generate(index, arg_pos, arg_respawnDir, arg_mineralID);

		break;

	}

}

void MineralMgr::Update(std::weak_ptr<Player> arg_player, std::weak_ptr<MineralTarget> arg_mineralTarget, std::weak_ptr<EnemyMgr> arg_enemyMgr) {

	moveSECount = 0;

	for (auto& index : m_minerals) {

		if (!index->GetIsAlive()) continue;

		//分裂した場合のベクトル
		std::vector<std::pair<KazMath::Vec3<float>, int>> breakUpPos;

		//更新処理
		index->Update(arg_player, breakUpPos, moveSECount);

		//分裂した場合、生成する。
		for (auto& breakUp : breakUpPos) {

			Generate(index->GetPos(), breakUp.first, breakUp.second);

		}

	}

	//重ならないようにする。
	for (auto& targetMineral : m_minerals) {

		if (!targetMineral->GetIsAlive()) continue;

		for (auto& otherMineral : m_minerals) {

			if (!otherMineral->GetIsAlive()) continue;

			//同じオブジェクトだったら飛ばす。
			if (targetMineral == otherMineral) continue;

			targetMineral->CounterOverlap(arg_player.lock()->GetMineralCenterPos(), otherMineral->GetPosRef(), otherMineral->GetCollisionScale());

		}

	}

	//プレイヤーが台パン状態だったら
	if (arg_player.lock()->GetIsDaipanTrigger() && arg_mineralTarget.lock()->GetCanTarget()) {

		//攻撃させる。
		for (auto& index : m_minerals) {

			if (!index->GetIsAlive()) continue;
			if (index->GetIsAttack()) continue;
			if (!index->GetIsGathering()) continue;
			if (index->GetIsGatheringTrigger()) continue;
			if (index->GetIsGoToGetMaterial()) continue;


			int targetIndex = arg_mineralTarget.lock()->GetCanTarget();
			//ターゲットが建造物の場合はサイズは関係ない。
			if (targetIndex == 5) {
				//建築材料を持っていたら
				if (index->GetHaveMaterial()) {

					//建築材料が足りていなかったら。
					//if (BuildingMgr::Instance()->IsFullMaterialWall(arg_mineralTarget.lock()->GetTargetBuilidngIndex())) {

						//材料を足す。
						//BuildingMgr::Instance()->AddMaterialWall(arg_mineralTarget.lock()->GetTargetBuilidngIndex());
						//index->EraseTheMaterial();
					index->Build(BuildingMgr::Instance()->GetWall(arg_mineralTarget.lock()->GetTargetBuilidngIndex()));

					//}
					//else {

						//地面に置く。
						//index->DropMaterial();

					//}

				}
			}

			//攻撃させる。
			if (targetIndex == 1) {
				index->Attack(arg_mineralTarget.lock()->GetTargetRock());
			}
			else if (targetIndex == 2) {
				index->Attack(arg_mineralTarget.lock()->GetTargetMinekuji());
			}
			else if (targetIndex == 3) {
				index->Attack(arg_mineralTarget.lock()->GetTargetTree());
			}
			else if (targetIndex == 4) {
				if (index->GetHaveMaterial()) continue;
				index->GoToGetMaterial(arg_mineralTarget.lock()->GetTargetMaterial());
				break;
			}
			else if (targetIndex == 6) {
				index->Attack(arg_mineralTarget.lock()->GetTargetMineTsumuri());
			}

		}

	}


	//追従していないミネラルを自動行動させる処理
	for (auto& mineralIndex : m_minerals) {

		if (!mineralIndex->GetIsAlive()) continue;
		if (mineralIndex->GetIsGathering()) continue;
		if (mineralIndex->GetIsAttack()) continue;

		//ここまで通るのは生成されていて、追従していなくて、攻撃していないミネラル。

		//探索範囲
		const float SEARCH_RANGE = 30.0f;

		//敵がいたら自動攻撃の処理
		{

			//ミネクジの最短距離
			float mineKujiDistance = 0.0f;
			int minekujiIndex = arg_enemyMgr.lock()->GetTargetMineKujiIndex(mineralIndex->GetPosZeroY(), SEARCH_RANGE, mineKujiDistance);

			//ミネクジの最短距離
			float mineTsumuriDistance = 0.0f;
			int mineTsumuriIndex = arg_enemyMgr.lock()->GetTargetMineTsumuriIndex(mineralIndex->GetPosZeroY(), SEARCH_RANGE, mineTsumuriDistance);

			//探索範囲内に敵は居るか？
			bool isNearEnemy = minekujiIndex != -1 || mineTsumuriIndex != -1;

			//探索範囲に敵がいるのなら、近くにいるのはミネクジか？ミネツムリか？
			if (isNearEnemy) {

				bool isMineKuji = false;

				//どちらか一方のインデックスが-1だったら
				if (minekujiIndex == -1) {
					isMineKuji = false;
				}
				else if (mineTsumuriIndex == -1) {
					isMineKuji = true;
				}
				//どちらにも値が入っていたら。
				else {

					//値の小さいほうを優先する。
					isMineKuji = mineKujiDistance < mineTsumuriDistance;

				}

				//敵を攻撃する。
				if (isMineKuji) {
					mineralIndex->Attack(arg_enemyMgr.lock()->GetMineKuji(minekujiIndex));
				}
				else {
					mineralIndex->Attack(arg_enemyMgr.lock()->GetMineTsumuri(mineTsumuriIndex));
				}

				continue;

			}

		}

		//近くに木があったらの処理
		{

			float treeDistance = 0.0f;
			int treeIndex = DestructibleObjectMgr::Instance()->GetTargetTreeIndex(mineralIndex->GetPosZeroY(), SEARCH_RANGE, treeDistance);

			//近くに木が合ったら
			if (treeIndex != -1) {

				mineralIndex->Attack(DestructibleObjectMgr::Instance()->GetTree(treeIndex));

				continue;

			}

		}

		//近くに岩があった場合の処理
		{

			float rockDistance = 0.0f;
			int rockIndex = RockMgr::Instance()->GetTargetRockIndex(mineralIndex->GetPosZeroY(), SEARCH_RANGE, rockDistance);

			//近くに木が合ったら
			if (rockIndex != -1) {

				mineralIndex->Attack(RockMgr::Instance()->GetRock(rockIndex));

				continue;

			}

		}

		//近くに建材があった場合の処理
		if (!mineralIndex->GetHaveMaterial()) {

			float materialDistance = 0.0f;
			int materialIndex = BuildingMaterialMgr::Instance()->GetTargetMaterialIndex(mineralIndex->GetPosZeroY(), SEARCH_RANGE, materialDistance);

			//近くに木が合ったら
			if (materialIndex != -1) {

				mineralIndex->HaveMaterial(BuildingMaterialMgr::Instance()->GetMaterial(materialIndex));

				continue;

			}

		}


	}


	//プレイヤーがスタンしたらミネラルの追従を解除する。
	if (arg_player.lock()->GetIsStun()) {

		for (auto& mineral : m_minerals) {

			if (!mineral->GetIsAlive()) continue;
			if (mineral->GetIsAttack()) continue;
			if (!mineral->GetIsGathering()) continue;

			mineral->BreakUP({ 0,1,0 });

		}

	}

}

void MineralMgr::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec) {


	for (auto& index : m_minerals) {

		if (!index->GetIsAlive()) continue;

		index->Draw(arg_rasterize, arg_blasVec);

	}

}

bool MineralMgr::SearchNearMineral(KazMath::Vec3<float> arg_pos, float arg_searchRange, int& arg_mineralIndex, float& arg_distance) {

	bool isHit = false;
	float miniDistance = 1000000;
	int mineralIndex = -1;

	for (auto& index : m_minerals) {

		if (!index->GetIsAlive()) continue;

		//距離を検索
		float distance = KazMath::Vec3<float>(index->GetPosZeroY() - arg_pos).Length();

		if (arg_searchRange <= distance) continue;

		isHit = true;
		if (distance < miniDistance) {
			distance = miniDistance;
			mineralIndex = static_cast<int>(&index - &m_minerals[0]);
		}


	}

	if (isHit) {
		arg_mineralIndex = mineralIndex;
		arg_distance = miniDistance;
	}

	//インデックスが-1だったら当たってないことにする。
	if (arg_mineralIndex < 0) {
		isHit = false;
	}

	return isHit;
}

int MineralMgr::GetMineralScore()
{

	int score = 0;

	for (auto& index : m_minerals) {

		if (!index->GetIsAlive()) continue;

		if (index->GetMineralID() == 0) {
			score += 50;
		}
		else if (index->GetMineralID() == 1) {
			score += 150;
		}

	}

	return score;
}
