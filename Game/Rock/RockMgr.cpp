#include "../Game/Rock/Rock.h"
#include "RockMgr.h"
#include "../Player.h"
#include "../Game/Mineral/MineralMgr.h"

void RockMgr::Setting()
{

	for (auto& index : m_rocks) {

		index = std::make_shared<Rock>();

	}
}

void RockMgr::DebugGenerate() {

	using namespace KazMath;

	std::vector<Vec3<float>> debugInfo;
	debugInfo.emplace_back(Vec3<float>(40, 0, -40));

	for (auto& param : debugInfo) {

		for (auto& index : m_rocks) {

			if (index->GetIsAlive()) continue;

			index->Generate(param, {}, true, Rock::MEDIUM);

			break;

		}

	}

}

void RockMgr::Init() {

	for (auto& index : m_rocks) {

		index->Init();

	}

}

void RockMgr::Generate(KazMath::Vec3<float> arg_pos, KazMath::Vec3<float> arg_respawnDir, bool m_isMineralRock, int arg_mineralID) {

	for (auto& index : m_rocks) {

		if (index->GetIsAlive()) continue;

		index->Generate(arg_pos, arg_respawnDir, m_isMineralRock, arg_mineralID);

		break;

	}

}

void RockMgr::Update(std::weak_ptr<Player> arg_player) {

	for (auto& index : m_rocks) {

		if (!index->GetIsAlive()) continue;

		//���􂵂��ꍇ�̃x�N�g��
		std::vector<std::pair<KazMath::Vec3<float>, int>> breakUpPos;

		//�X�V����
		index->Update(arg_player, breakUpPos);

		//���􂵂��ꍇ�A��������B
		if (index->GetIsMineralRock()) {
			for (auto& breakUp : breakUpPos) {

				MineralMgr::Instance()->Generate(index->GetPos(), breakUp.first, 0);

			}
		}
		else {
			for (auto& breakUp : breakUpPos) {

				Generate(index->GetPos(), breakUp.first, false, breakUp.second);

			}
		}

	}

}

void RockMgr::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec) {


	for (auto& index : m_rocks) {

		//if (!index->GetIsAlive()) continue;

		index->Draw(arg_rasterize, arg_blasVec);

	}

}

int RockMgr::GetTargetRockIndex(KazMath::Vec3<float> arg_playerPos, float arg_targetRange, float& arg_targetDistance)
{

	//�������Ă����̂Ȃ�����^�[�Q�b�g�͈͓̔��̂��̂���������B
	std::vector<int> inTargetIndex;
	std::vector<float> inTargetLength;
	arg_targetDistance = 1000000.0f;
	for (auto& index : m_rocks) {

		if (!index->GetIsAlive()) continue;

		//�^�[�Q�b�g�̒��ɓ����Ă��邩���`�F�b�N����B
		float length = KazMath::Vec3<float>(arg_playerPos - index->GetPos()).Length();
		bool inTarget = length < arg_targetRange;
		if (!inTarget) continue;

		//�C���f�b�N�X���擾�B
		inTargetIndex.emplace_back(static_cast<int>(&index - &m_rocks[0]));
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

KazMath::Vec3<float> RockMgr::GetPos(int arg_index)
{
	return m_rocks[arg_index]->GetPos();
}

KazMath::Vec3<float> RockMgr::GetScale(int arg_index)
{
	return m_rocks[arg_index]->GetScale();
}
