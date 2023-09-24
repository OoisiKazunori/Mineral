#include "Mineral.h"
#include "MineralMgr.h"
#include "MineralTarget.h"
#include "../Game/DestructibleObject/DestructibleObjectMgr.h"
#include "../Player.h"
#include "../Building/BuildingMgr.h"

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

void MineralMgr::Update(std::weak_ptr<Player> arg_player, std::weak_ptr<MineralTarget> arg_mineralTarget) {

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

	//解散状態だったら。
	if (arg_player.lock()->GetIsBreakUP()) {

		for (auto& index : m_minerals) {

			//同じオブジェクトだったら飛ばす。
			if (!index->GetIsAlive()) continue;
			if (!index->GetIsGathering()) continue;

			index->BreakUP(arg_player.lock()->GetForwardVec());

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
					if (BuildingMgr::Instance()->IsFullMaterialWall(arg_mineralTarget.lock()->GetTargetBuilidngIndex())) {

						//材料を足す。
						BuildingMgr::Instance()->AddMaterialWall(arg_mineralTarget.lock()->GetTargetBuilidngIndex());
						index->EraseTheMaterial();

					}
					else {

						//地面に置く。
						//index->DropMaterial();

					}

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

}

void MineralMgr::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec) {


	for (auto& index : m_minerals) {

		//if (!index->GetIsAlive()) continue;

		index->Draw(arg_rasterize, arg_blasVec);

	}

}

bool MineralMgr::SearchNearMineral(KazMath::Vec3<float> arg_pos, float arg_searchRange, int& arg_mineralIndex) {

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
