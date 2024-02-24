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

		//���􂵂��ꍇ�̃x�N�g��
		std::vector<std::pair<KazMath::Vec3<float>, int>> breakUpPos;

		//�X�V����
		index->Update(arg_player, breakUpPos, moveSECount);

		//���􂵂��ꍇ�A��������B
		for (auto& breakUp : breakUpPos) {

			Generate(index->GetPos(), breakUp.first, breakUp.second);

		}

	}

	//�d�Ȃ�Ȃ��悤�ɂ���B
	for (auto& targetMineral : m_minerals) {

		if (!targetMineral->GetIsAlive()) continue;

		for (auto& otherMineral : m_minerals) {

			if (!otherMineral->GetIsAlive()) continue;

			//�����I�u�W�F�N�g���������΂��B
			if (targetMineral == otherMineral) continue;

			targetMineral->CounterOverlap(arg_player.lock()->GetMineralCenterPos(), otherMineral->GetPosRef(), otherMineral->GetCollisionScale());

		}

	}

	//�v���C���[����p����Ԃ�������
	if (arg_player.lock()->GetIsDaipanTrigger() && arg_mineralTarget.lock()->GetCanTarget()) {

		//�U��������B
		for (auto& index : m_minerals) {

			if (!index->GetIsAlive()) continue;
			if (index->GetIsAttack()) continue;
			if (!index->GetIsGathering()) continue;
			if (index->GetIsGatheringTrigger()) continue;
			if (index->GetIsGoToGetMaterial()) continue;


			int targetIndex = arg_mineralTarget.lock()->GetCanTarget();
			//�^�[�Q�b�g���������̏ꍇ�̓T�C�Y�͊֌W�Ȃ��B
			if (targetIndex == 5) {
				//���z�ޗ��������Ă�����
				if (index->GetHaveMaterial()) {

					//���z�ޗ�������Ă��Ȃ�������B
					//if (BuildingMgr::Instance()->IsFullMaterialWall(arg_mineralTarget.lock()->GetTargetBuilidngIndex())) {

						//�ޗ��𑫂��B
						//BuildingMgr::Instance()->AddMaterialWall(arg_mineralTarget.lock()->GetTargetBuilidngIndex());
						//index->EraseTheMaterial();
					index->Build(BuildingMgr::Instance()->GetWall(arg_mineralTarget.lock()->GetTargetBuilidngIndex()));

					//}
					//else {

						//�n�ʂɒu���B
						//index->DropMaterial();

					//}

				}
			}

			//�U��������B
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


	//�Ǐ]���Ă��Ȃ��~�l�����������s�������鏈��
	for (auto& mineralIndex : m_minerals) {

		if (!mineralIndex->GetIsAlive()) continue;
		if (mineralIndex->GetIsGathering()) continue;
		if (mineralIndex->GetIsAttack()) continue;

		//�����܂Œʂ�̂͐�������Ă��āA�Ǐ]���Ă��Ȃ��āA�U�����Ă��Ȃ��~�l�����B

		//�T���͈�
		const float SEARCH_RANGE = 30.0f;

		//�G�������玩���U���̏���
		{

			//�~�l�N�W�̍ŒZ����
			float mineKujiDistance = 0.0f;
			int minekujiIndex = arg_enemyMgr.lock()->GetTargetMineKujiIndex(mineralIndex->GetPosZeroY(), SEARCH_RANGE, mineKujiDistance);

			//�~�l�N�W�̍ŒZ����
			float mineTsumuriDistance = 0.0f;
			int mineTsumuriIndex = arg_enemyMgr.lock()->GetTargetMineTsumuriIndex(mineralIndex->GetPosZeroY(), SEARCH_RANGE, mineTsumuriDistance);

			//�T���͈͓��ɓG�͋��邩�H
			bool isNearEnemy = minekujiIndex != -1 || mineTsumuriIndex != -1;

			//�T���͈͂ɓG������̂Ȃ�A�߂��ɂ���̂̓~�l�N�W���H�~�l�c�������H
			if (isNearEnemy) {

				bool isMineKuji = false;

				//�ǂ��炩����̃C���f�b�N�X��-1��������
				if (minekujiIndex == -1) {
					isMineKuji = false;
				}
				else if (mineTsumuriIndex == -1) {
					isMineKuji = true;
				}
				//�ǂ���ɂ��l�������Ă�����B
				else {

					//�l�̏������ق���D�悷��B
					isMineKuji = mineKujiDistance < mineTsumuriDistance;

				}

				//�G���U������B
				if (isMineKuji) {
					mineralIndex->Attack(arg_enemyMgr.lock()->GetMineKuji(minekujiIndex));
				}
				else {
					mineralIndex->Attack(arg_enemyMgr.lock()->GetMineTsumuri(mineTsumuriIndex));
				}

				continue;

			}

		}

		//�߂��ɖ؂���������̏���
		{

			float treeDistance = 0.0f;
			int treeIndex = DestructibleObjectMgr::Instance()->GetTargetTreeIndex(mineralIndex->GetPosZeroY(), SEARCH_RANGE, treeDistance);

			//�߂��ɖ؂���������
			if (treeIndex != -1) {

				mineralIndex->Attack(DestructibleObjectMgr::Instance()->GetTree(treeIndex));

				continue;

			}

		}

		//�߂��Ɋ₪�������ꍇ�̏���
		{

			float rockDistance = 0.0f;
			int rockIndex = RockMgr::Instance()->GetTargetRockIndex(mineralIndex->GetPosZeroY(), SEARCH_RANGE, rockDistance);

			//�߂��ɖ؂���������
			if (rockIndex != -1) {

				mineralIndex->Attack(RockMgr::Instance()->GetRock(rockIndex));

				continue;

			}

		}

		//�߂��Ɍ��ނ��������ꍇ�̏���
		if (!mineralIndex->GetHaveMaterial()) {

			float materialDistance = 0.0f;
			int materialIndex = BuildingMaterialMgr::Instance()->GetTargetMaterialIndex(mineralIndex->GetPosZeroY(), SEARCH_RANGE, materialDistance);

			//�߂��ɖ؂���������
			if (materialIndex != -1) {

				mineralIndex->HaveMaterial(BuildingMaterialMgr::Instance()->GetMaterial(materialIndex));

				continue;

			}

		}


	}


	//�v���C���[���X�^��������~�l�����̒Ǐ]����������B
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

		//����������
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

	//�C���f�b�N�X��-1�������瓖�����ĂȂ����Ƃɂ���B
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
