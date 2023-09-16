#include "Mineral.h"
#include "../Game/Player.h"
#include "../Game/Rock/Rock.h"
#include "../Effect/ShakeMgr.h"
#include "../Game/Enemy/MineKuji.h"
#include "../Game/DestructibleObject/DestructibleTree.h"
#include "../KazLibrary/Easing/easing.h"
#include "../Game/BuildingMaterial/BuildingMaterial.h"
#include "../Game/Collision/StageCollision.h"
#include "../Game/Enemy/MineTsumuri.h"
#include "../Game/Building/BuildingMgr.h"
#include "../Game/Tutorial.h"

Mineral::Mineral()
{

	m_isAlive = false;
	m_isOldGathering = false;
	m_isGathering = false;
	m_canGathering = false;
	m_isGoToGetMaterial = false;
	m_gravity = 0;
	m_mineralID = MEDIUM;

	m_transform.pos = { 10000,-10000,10000 };
	m_transform.scale = { 0,0,0 };

	m_model[0].LoadOutline("Resource/Mineral/Small/", "Mineral_S.gltf");
	m_model[1].LoadOutline("Resource/Mineral/Medium/", "Mineral_M.gltf");
	m_model[2].LoadOutline("Resource/Mineral/Lerge/", "Mineral_L.gltf");

	m_surprisedModel.LoadNoLighting("Resource/Mineral/Surprised/", "Surprised.gltf");
	m_surpisedTransform.pos = m_transform.pos;
	m_surpisedTransform.scale = { 1,1,1 };

	m_haveMaterial.reset();

	//SE
	rock_break01 = SoundManager::Instance()->SoundLoadWave("Resource/Sound/rock.wav");
	rock_break02 = SoundManager::Instance()->SoundLoadWave("Resource/Sound/break.wav");
	rock_break01.volume = 0.05f;
	rock_break02.volume = 0.05f;
	have_material_se = SoundManager::Instance()->SoundLoadWave("Resource/Sound/Take_Wood.wav");
	have_material_se.volume = 0.05f;
	walk = SoundManager::Instance()->SoundLoadWave("Resource/Sound/Walk.wav");
	walk.volume = 0.07f;
	attack = SoundManager::Instance()->SoundLoadWave("Resource/Sound/Attack.wav");
	attack.volume = 0.1f;

}

void Mineral::Init() {

	m_isAlive = false;
	m_isOldGathering = false;
	m_isGathering = false;
	m_canGathering = false;
	m_isGoToGetMaterial = false;
	m_isAttack = false;
	m_gravity = 0;
	m_mineralID = MEDIUM;
	m_attackReactionVec = {};
	m_haveMaterial.reset();

}

void Mineral::Generate(std::weak_ptr<Mineral> arg_thisMineral, KazMath::Vec3<float> arg_pos, KazMath::Vec3<float> arg_respawnVec, int arg_mineralID) {

	m_mineralID = static_cast<MINERAL_ID>(arg_mineralID);
	m_transform.pos = arg_pos;
	m_respawnVec = arg_respawnVec;
	m_isAlive = true;
	m_isOldGathering = false;
	m_isAttack = false;
	m_isDrawSurpised = false;
	m_canGathering = false;
	m_isGoToGetMaterial = false;
	m_gravity = 0;
	m_breakUpVec = {};
	m_attackReactionVec = {};
	m_thisMineral = arg_thisMineral;
	m_oldTransform = m_transform;
	m_haveMaterial.reset();
	m_moveVec = {};
	m_moveSpan = 0;
	m_randomMoveSpan = MOVE_SPAN + KazMath::Rand(0, 1);
	m_wallJump = {};

	m_hp = HP[static_cast<int>(arg_mineralID)];

	//�����̂ӂ��Ƃ΂�������ꍇ�́A����ɓ����Ă����Ԃɂ���B
	m_isGathering = 0 < arg_respawnVec.Length();
	if (m_isGathering) {
		m_isDrawSurpised = true;
		m_surpisedTimer = 0.0f;
		m_surprisedStatus = SURPRISED_STATUS::APPEAR;
	}

}

void Mineral::Update(std::weak_ptr<Player> arg_player, std::vector<std::pair<KazMath::Vec3<float>, int>>& arg_respawnVec, int& arg_moveSECount) {

	using namespace KazMath;

	//���W��ۑ����Ă����B
	m_oldTransform = m_transform;

	//�^����ꂽ���������W�ɑ����B
	if (0.01f < m_respawnVec.Length()) {

		m_transform.pos += m_respawnVec;
		m_respawnVec -= m_respawnVec / 10.0f;
	}
	else {

		m_respawnVec = {};

	}

	//���U���̈ړ����X�V�B
	if (0.01f < m_breakUpVec.Length()) {

		m_transform.pos += m_breakUpVec;
		m_breakUpVec -= m_breakUpVec / 10.0f;

	}
	else {

		m_breakUpVec = {};

	}

	//�U���̍X�V����
	if (m_isAttack) {

		UpdateAttack(arg_player, arg_moveSECount);

	}
	//�}�e���A�������ɍs����Ԃ�������
	else if (m_isGoToGetMaterial) {

	}
	//����ɂȂ��Ă�����ǔ�����B
	else if (m_isGathering && m_canGathering) {

		//�ǔ�������W
		Vec3<float> playerPos = arg_player.lock()->GetMineralCenterPos();
		Vec3<float> mineralPos = GetPosZeroY();

		++m_moveSpan;
		if (m_randomMoveSpan <= m_moveSpan) {

			//�ǔ�����B
			m_moveVec = (playerPos - mineralPos).GetNormal();
			m_moveVec.y = 1.0f;
			m_moveVec.Normalize();
			m_moveVec *= MOVE_SPEED[static_cast<int>(m_mineralID)] + KazMath::Rand(0.0f, MOVE_SPEED[static_cast<int>(m_mineralID)]);

			m_moveSpan = 0;

			if (arg_moveSECount < 1) {
				++arg_moveSECount;
				SoundManager::Instance()->SoundPlayerWave(walk, 0);
			}

		}

	}

	//�}�e���A���������Ă����Ԃ�������
	if (!m_haveMaterial.expired()) {
		UpdateHaveMaterial(arg_player);
	}

	//�󒆂ɕ����Ă�����d�͂��v�Z����B
	if (MINERAL_OFFSETY[static_cast<int>(m_mineralID)] < m_transform.pos.y) {

		m_gravity += GRAVITY;
		m_transform.pos.y -= m_gravity;
		m_canGathering = false;

	}
	else {

		m_gravity = 0.0f;
		m_transform.pos.y = MINERAL_OFFSETY[static_cast<int>(m_mineralID)];
		m_canGathering = true;

	}

	//����ɂȂ����g���K�[��������
	if (!m_isOldGathering && m_isGathering) {

		m_gatheringTriggerEffect = GATHERING_TRIGGER_EFFECT;

	}

	//����̃g���K�[�ɂȂ�����u�Ԃ̃G�t�F�N�g������B
	m_transform.pos.y += m_gatheringTriggerEffect;
	if (0 < m_gatheringTriggerEffect) {
		m_gatheringTriggerEffect -= 0.05f;
	}
	else {
		m_gatheringTriggerEffect = 0.0f;
	}

	//�v���C���[�Ƃ̓����蔻����s���B
	m_isOldGathering = m_isGathering;
	if (arg_player.lock()->GetIsDaipanTrigger()) {

		//�����蔻��`�F�b�N �����g��ł����Ԃ�������L�����Z���B
		bool isHit = Vec3<float>(arg_player.lock()->GetPosZeroY() - GetPosZeroY()).Length() <= arg_player.lock()->GetMineralAffectRange();
		if (isHit) {

			if (!m_isGathering) {

				m_isGathering = true;

				//�r�b�N���}�[�N���o���B
				m_isDrawSurpised = true;
				m_surpisedTimer = 0.0f;
				m_surprisedStatus = SURPRISED_STATUS::APPEAR;


			}
			//���łɑ����g��ł����Ԃ�������A������ƃW�����v������B
			else {

				m_respawnVec = { Vec3<float>(KazMath::Rand(-0.1f, 0.1f) ,KazMath::Rand(0.5f, 2.0f) ,KazMath::Rand(-0.1f, 0.1f)) };
				m_respawnVec.Normalize();
				m_respawnVec *= GATHERING_DAIPAN_POWER;

			}

		}

		//����p����������
		if (m_mineralID != SMALL && isHit && arg_player.lock()->GetIsStrongDaipan()) {

			int id = 0;
			switch (m_mineralID)
			{
			case Mineral::SMALL:
				break;
			case Mineral::MEDIUM:
				id = SMALL;
				break;
			default:
				break;
			}

			//ID��Small�ȊO�������珬�������āA���􂳂���B
			if (m_mineralID != SMALL) {
				SoundManager::Instance()->SoundPlayerWave(rock_break01, 0);
				SoundManager::Instance()->SoundPlayerWave(rock_break02, 0);
				for (int index = 0; index < 3; ++index) {
					std::pair<KazMath::Vec3<float>, int> respawnData;
					respawnData.first = { Vec3<float>(KazMath::Rand(-1.0f, 1.0f) ,KazMath::Rand(-0.0f, 2.0f) ,KazMath::Rand(-1.0f, 1.0f)) };
					respawnData.first.GetNormal();
					respawnData.first *= STRONG_DAIPAN_POWER;
					respawnData.second = id;
					arg_respawnVec.emplace_back(respawnData);
				}
			}

			//��������Ă����痎�Ƃ��B
			if (!m_haveMaterial.expired()) {

				m_haveMaterial.lock()->Release();
				m_haveMaterial.reset();

			}

			//�����{�̂͏����B
			Init();

			//���̃`���[�g���A���ɑ���
			if (Tutorial::Instance()->tutorial_num == 6 || Tutorial::Instance()->tutorial_num == 7)
			{
				Tutorial::Instance()->break_mineral = true;
			}
		}
	}

	//�~�l�����̃T�C�Y���X�e�[�^�X�ɂ���ĕύX������B
	switch (m_mineralID)
	{
	case Mineral::SMALL:
		m_transform.scale = { MINERAL_DRAW_SIZE_SMALL,MINERAL_DRAW_SIZE_SMALL ,MINERAL_DRAW_SIZE_SMALL };
		break;
	case Mineral::MEDIUM:
		m_transform.scale = { MINERAL_DRAW_SIZE_MEDIUM,MINERAL_DRAW_SIZE_MEDIUM ,MINERAL_DRAW_SIZE_MEDIUM };
		break;
	default:
		break;
	}

	//�U���̔�����^����B
	if (0.01f < m_attackReactionVec.Length()) {

		m_transform.pos += m_attackReactionVec;
		m_attackReactionVec -= m_attackReactionVec / 10.0f;

	}
	else {

		m_attackReactionVec = {};

	}
	//�ړ�����
	if (0.01f < m_moveVec.Length()) {

		m_transform.pos += m_moveVec;
		m_moveVec -= m_moveVec / 10.0f;

	}
	else {

		m_moveVec = {};

	}

	//�X����B
	KazMath::Vec3<float> nowPos = GetPosZeroY();
	KazMath::Vec3<float> viewTargetPos;
	nowPos.y = 0.0f;
	//�U�����̓^�[�Q�b�g�̕���������B
	if (m_isAttack) {

		if (!m_attackTargetRock.expired()) {
			nowPos = m_attackTargetRock.lock()->GetPos();
		}
		else if (!m_attackTargetMineKuji.expired()) {
			nowPos = m_attackTargetMineKuji.lock()->GetPos();
		}
		else if (!m_attackTargetMineTsumuri.expired()) {
			nowPos = m_attackTargetMineTsumuri.lock()->GetPos();
		}
		else if (!m_attackDestrutibleTree.expired()) {
			nowPos = m_attackDestrutibleTree.lock()->GetPos();
		}
		nowPos.y = 0.0f;
		viewTargetPos = GetPosZeroY();
		viewTargetPos.y = 0.0f;

	}
	else {

		//�����������ɌX����B
		viewTargetPos = m_oldTransform.pos;
		viewTargetPos.y = 0.0f;

	}
	if (0.2f < KazMath::Vec3<float>(nowPos - viewTargetPos).Length()) {

		KazMath::Vec3<float> forwardVec = KazMath::Vec3<float>(nowPos - viewTargetPos).GetNormal();

		//�f�t�H���g�̐��ʃx�N�g������̉�]�ʂ����߂�B
		float angle = std::acosf(KazMath::Vec3<float>(0, 0, 1).Dot(forwardVec));

		//�N�H�[�^�j�I�������߂�B
		KazMath::Vec3<float> cross = KazMath::Vec3<float>(0, 0, 1).Cross(forwardVec);
		if (cross.Length() <= 0) {
			cross = { 0,1,0 };
		}
		DirectX::XMVECTOR rotateQ = DirectX::XMQuaternionRotationAxis({ cross.x, cross.y, cross.z }, angle);

		//��]��K��
		m_transform.quaternion = rotateQ;

	}

	//�r�b�N���}�[�N�̃X�P�[���𒲐��B
	if (m_isDrawSurpised) {

		switch (m_surprisedStatus)
		{
		case Mineral::SURPRISED_STATUS::APPEAR:
		{

			//�^�C�}�[���X�V
			m_surpisedTimer = std::clamp(m_surpisedTimer + 1.0f, 0.0f, SURPRISED_APPEAR_TIMER);

			//�C�[�W���O�ŃX�P�[����ς���B
			float easingTimer = EasingMaker(InOut, Exp, m_surpisedTimer / SURPRISED_APPEAR_TIMER);
			m_surpisedTransform.scale.x = easingTimer;
			m_surpisedTransform.scale.z = easingTimer;

			//�^�C�}�[���ő�l�ɒB�����玟��
			if (SURPRISED_APPEAR_TIMER <= m_surpisedTimer) {

				m_surpisedTimer = 0;
				m_surprisedStatus = SURPRISED_STATUS::STAY;

			}

		}
		break;
		case Mineral::SURPRISED_STATUS::STAY:
		{

			//�^�C�}�[���X�V
			m_surpisedTimer = std::clamp(m_surpisedTimer + 1.0f, 0.0f, SURPRISED_STAY_TIMER);

			//�^�C�}�[���ő�l�ɒB�����玟��
			if (SURPRISED_STAY_TIMER <= m_surpisedTimer) {

				m_surpisedTimer = 0;
				m_surprisedStatus = SURPRISED_STATUS::EXIT;

			}
		}
		break;
		case Mineral::SURPRISED_STATUS::EXIT:
		{

			//�^�C�}�[���X�V
			m_surpisedTimer = std::clamp(m_surpisedTimer + 1.0f, 0.0f, SURPRISED_EXIT_TIMER);

			//�C�[�W���O�ŃX�P�[����ς���B
			float easingTimer = EasingMaker(InOut, Exp, m_surpisedTimer / SURPRISED_APPEAR_TIMER);
			m_surpisedTransform.scale.x = 1.0f - easingTimer;
			m_surpisedTransform.scale.z = 1.0f - easingTimer;

			//�^�C�}�[���ő�l�ɒB�����玟��
			if (SURPRISED_EXIT_TIMER <= m_surpisedTimer) {

				m_surpisedTimer = 0;
				m_surprisedStatus = SURPRISED_STATUS::APPEAR;
				m_isDrawSurpised = false;

			}
		}
		break;
		default:
			break;
		}

	}


	KazMath::Vec3<float> oldPos = m_oldTransform.pos;
	if (0 < KazMath::Vec3<float>(nowPos - oldPos).Length()) {

		m_forwardVec = KazMath::Vec3<float>(nowPos - oldPos).GetNormal();

	}


	//�����蔻����v�Z�B
	const float RAY_LENGTH = 5.0f;
	MeshCollision::CheckHitResult rayResult = StageCollision::Instance()->m_stageCollision.CheckHitRay(m_transform.pos, m_forwardVec);
	if (rayResult.m_isHit && 0.0f < rayResult.m_distance && rayResult.m_distance <= RAY_LENGTH) {

		//�����߂��B
		m_transform.pos += rayResult.m_normal * (RAY_LENGTH - rayResult.m_distance);

	}

	//�ǂƂ̓����蔻��B
	int wallIndex = 0;
	KazMath::Vec3<float> movedVecZeroY = KazMath::Vec3<float>(m_transform.pos - m_oldTransform.pos).GetNormal();
	movedVecZeroY.y = 0.0f;
	movedVecZeroY.Normalize();
	MeshCollision::CheckHitResult collisionResult = BuildingMgr::Instance()->CheckHitWall(m_transform.pos, movedVecZeroY, 20.0f, wallIndex);
	if (collisionResult.m_isHit) {

		//���������ɑ�W�����v�B
		if (m_wallJump.Length() <= 0.01f) {

			m_wallJump = movedVecZeroY;
			m_wallJump.y = 3.0f;
			m_wallJump.Normalize();
			m_wallJump *= 10.0f;

		}

	}

	//�ǒ���������B
	if (0.01f < m_wallJump.Length()) {

		m_transform.pos += m_wallJump;
		m_wallJump -= m_wallJump / 10.0f;

	}
	else {

		m_wallJump = {};

	}


	//�~�l���������񂾂�
	if (m_hp <= 0) {
		SoundManager::Instance()->SoundPlayerWave(rock_break01, 0);
		SoundManager::Instance()->SoundPlayerWave(rock_break02, 0);
		if (m_mineralID == MEDIUM) {

			//ID��Small�ȊO�������珬�������āA���􂳂���B
			if (m_mineralID != SMALL) {
				for (int index = 0; index < 3; ++index) {
					std::pair<KazMath::Vec3<float>, int> respawnData;
					respawnData.first = { Vec3<float>(KazMath::Rand(-1.0f, 1.0f) ,KazMath::Rand(-0.0f, 2.0f) ,KazMath::Rand(-1.0f, 1.0f)) };
					respawnData.first.GetNormal();
					respawnData.first *= STRONG_DAIPAN_POWER;
					respawnData.second = SMALL;
					arg_respawnVec.emplace_back(respawnData);
				}
			}

		}

		//��������Ă����痎�Ƃ��B
		if (!m_haveMaterial.expired()) {

			m_haveMaterial.lock()->Release();
			m_haveMaterial.reset();

		}

		//�����{�̂͏����B
		Init();

	}

}

void Mineral::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec) {

	if (!m_isAlive) {
		m_transform.scale = {};
	}

	if (m_isDrawSurpised) {
		m_surpisedTransform.pos = m_transform.pos + KazMath::Vec3<float>(0, 10, 0);
	}
	else {
		m_surpisedTransform.pos = { 0,-10000,0 };
	}


	DessolveOutline outline;
	if (m_isAttack) {
		outline.m_outline = KazMath::Vec4<float>(0.4f, 0, 0, 1);
	}
	else if (m_isGathering) {
		outline.m_outline = KazMath::Vec4<float>(0.1f, 0, 0, 1);
	}
	else {
		outline.m_outline = KazMath::Vec4<float>(0.3f, 0, 0, 1);
	}
	m_model[static_cast<int>(m_mineralID)].m_model.extraBufferArray[4].bufferWrapper->TransData(&outline, sizeof(DessolveOutline));

	m_model[static_cast<int>(m_mineralID)].m_model.extraBufferArray.back() = GBufferMgr::Instance()->m_outlineBuffer;
	m_model[static_cast<int>(m_mineralID)].m_model.extraBufferArray.back().rangeType = GRAPHICS_RANGE_TYPE_UAV_DESC;
	m_model[static_cast<int>(m_mineralID)].m_model.extraBufferArray.back().rootParamType = GRAPHICS_PRAMTYPE_TEX;

	m_model[static_cast<int>(m_mineralID)].Draw(arg_rasterize, arg_blasVec, m_transform);
	m_surprisedModel.Draw(arg_rasterize, arg_blasVec, m_surpisedTransform);

}

void Mineral::Damage(int arg_damage)
{

	m_hp = std::clamp(m_hp - arg_damage, 0, 1000);

}

void Mineral::CounterOverlap(KazMath::Vec3<float> arg_centerPos, KazMath::Vec3<float>& arg_mineralPos, float arg_scale)
{

	//�d�Ȃ��Ă��邩���`�F�b�N����B
	float mineralLength = KazMath::Vec3<float>(arg_mineralPos - m_transform.pos).Length();
	bool isHit = mineralLength <= GetCollisionScale() + arg_scale;

	//�d�Ȃ��Ă��Ȃ���������Ȃ��B
	if (!isHit) return;

	//�������Ă�����ǂ������O���ɋ��邩�𔻒f���āA�O���ɋ�����������߂��B
	float thisMineralLength = KazMath::Vec3<float>(arg_centerPos - m_transform.pos).Length();
	float argMineralLength = KazMath::Vec3<float>(arg_centerPos - arg_mineralPos).Length();
	bool isOutsideThisMineral = argMineralLength < thisMineralLength;

	//�����߂��ʂ��v�Z�B
	float pushBackLength = (arg_scale + GetCollisionScale()) - mineralLength;

	//���̃~�l�������O����������
	if (isOutsideThisMineral) {

		//�����߂��x�N�g�����v�Z�B
		KazMath::Vec3<float> pushBackVec = KazMath::Vec3<float>(m_transform.pos - arg_mineralPos);
		pushBackVec.y = 0.0f;
		pushBackVec.Normalize();

		//�����߂��B
		m_transform.pos += pushBackVec * pushBackLength;


	}
	else {

		//�����߂��x�N�g�����v�Z�B
		KazMath::Vec3<float> pushBackVec = KazMath::Vec3<float>(arg_mineralPos - m_transform.pos);
		pushBackVec.y = 0.0f;
		pushBackVec.Normalize();

		//�����߂��B
		arg_mineralPos += pushBackVec * pushBackLength;

	}

}

void Mineral::BreakUP(KazMath::Vec3<float> arg_forwardVec) {

	m_isGathering = false;

	//���U�Ŕ�΂��B
	KazMath::Vec3<float> baseVec = arg_forwardVec;
	float rotateAngle = 0.0f;
	switch (m_mineralID)
	{
	case Mineral::SMALL:
		rotateAngle = DirectX::XM_PI - (DirectX::XM_PI / 4.0f);
		break;
	case Mineral::MEDIUM:
		rotateAngle = DirectX::XM_PI;
		break;
	default:
		break;
	}

	//�x�N�g������]������B
	DirectX::XMVECTOR rotateQ = DirectX::XMQuaternionRotationAxis({ 0,1,0,0 }, rotateAngle);
	auto baseVector = DirectX::XMVector3Transform(baseVec.ConvertXMVECTOR(), DirectX::XMMatrixRotationQuaternion(rotateQ));
	baseVec = { baseVector.m128_f32[0],0.0f ,baseVector.m128_f32[2] };

	m_breakUpVec = baseVec.GetNormal() * BREAK_UP_POWER;

}

void Mineral::Attack(std::weak_ptr<Rock> arg_attackTargetRock)
{
	m_isAttack = true;
	m_attackTargetRock.reset();
	m_attackTargetMineKuji.reset();
	m_attackDestrutibleTree.reset();
	m_attackTargetMineTsumuri.reset();
	m_attackTargetRock = arg_attackTargetRock;
	m_attackID = ATTACK;
	m_attackMoveSpeed = 0.0f;
	m_attackReactionVec = {};

	m_stayDelayTimer = 0;
	m_stayDelay = KazMath::Rand(MIN_STAY_DELAY, MAX_STAY_DELAY);

	//if (!m_haveMaterial.expired()) {

	//	m_haveMaterial.lock()->Release();
	//	m_haveMaterial.reset();

	//}

}
void Mineral::Attack(std::weak_ptr<MineKuji> arg_attackTargetMinekuji)
{
	m_isAttack = true;
	m_attackTargetRock.reset();
	m_attackTargetMineKuji.reset();
	m_attackDestrutibleTree.reset();
	m_attackTargetMineTsumuri.reset();
	m_attackTargetMineKuji = arg_attackTargetMinekuji;
	m_attackID = ATTACK;
	m_attackMoveSpeed = 0.0f;
	m_attackReactionVec = {};

	m_stayDelayTimer = 0;
	m_stayDelay = KazMath::Rand(MIN_STAY_DELAY, MAX_STAY_DELAY);

	//if (!m_haveMaterial.expired()) {

	//	m_haveMaterial.lock()->Release();
	//	m_haveMaterial.reset();

	//}

}
void Mineral::Attack(std::weak_ptr<MineTsumuri> arg_attackTargetMinetsumuri)
{
	m_isAttack = true;
	m_attackTargetRock.reset();
	m_attackTargetMineKuji.reset();
	m_attackDestrutibleTree.reset();
	m_attackTargetMineTsumuri.reset();
	m_attackTargetMineTsumuri = arg_attackTargetMinetsumuri;
	m_attackID = ATTACK;
	m_attackMoveSpeed = 0.0f;
	m_attackReactionVec = {};

	m_stayDelayTimer = 0;
	m_stayDelay = KazMath::Rand(MIN_STAY_DELAY, MAX_STAY_DELAY);

	//if (!m_haveMaterial.expired()) {

	//	m_haveMaterial.lock()->Release();
	//	m_haveMaterial.reset();

	//}

}
void Mineral::Attack(std::weak_ptr<DestructibleTree> arg_destructibleTree) {
	m_isAttack = true;
	m_attackTargetRock.reset();
	m_attackTargetMineKuji.reset();
	m_attackDestrutibleTree.reset();
	m_attackTargetMineTsumuri.reset();
	m_attackDestrutibleTree = arg_destructibleTree;
	m_attackID = ATTACK;
	m_attackMoveSpeed = 0.0f;
	m_attackReactionVec = {};

	m_stayDelayTimer = 0;
	m_stayDelay = KazMath::Rand(MIN_STAY_DELAY, MAX_STAY_DELAY);

	//if (!m_haveMaterial.expired()) {

	//	m_haveMaterial.lock()->Release();
	//	m_haveMaterial.reset();

	//}

}

void Mineral::HaveMaterial(std::weak_ptr<BuildingMaterial> arg_material)
{
	SoundManager::Instance()->SoundPlayerWave(have_material_se, 0);
	m_haveMaterial = arg_material;
	m_haveMaterial.lock()->Held();

	if (Tutorial::Instance()->tutorial_num == 9)
	{
		Tutorial::Instance()->tree_carry_count++;
	}
}

void Mineral::DropMaterial()
{

	m_haveMaterial.lock()->Release();
	m_haveMaterial.reset();

}

void Mineral::EraseTheMaterial()
{
	m_haveMaterial.lock()->Init();
	m_haveMaterial.reset();
}

float Mineral::GetCollisionScale() {

	switch (m_mineralID)
	{
	case Mineral::SMALL:
		return MINERAL_COLLISION_SIZE_SMALL;
		break;
	case Mineral::MEDIUM:
		return MINERAL_COLLISION_SIZE_MEDIUM;
		break;
	default:
		break;
	}

	return MINERAL_COLLISION_SIZE_BIG;

}

void Mineral::UpdateAttack(std::weak_ptr<Player> arg_player, int& arg_moveSECount)
{

	switch (m_attackID)
	{
	case Mineral::ATTACK:
	{

		//�^�[�Q�b�g�̍��W�Ȃǂ��擾����B
		KazMath::Vec3<float> targetPos;
		KazMath::Vec3<float> targetScale;
		if (!m_attackTargetRock.expired()) {
			targetPos = m_attackTargetRock.lock()->GetPos();
			targetScale = m_attackTargetRock.lock()->GetScale();
		}
		else if (!m_attackTargetMineKuji.expired()) {
			targetPos = m_attackTargetMineKuji.lock()->GetPos();
			targetScale = m_attackTargetMineKuji.lock()->GetScale();
		}
		else if (!m_attackTargetMineTsumuri.expired()) {
			targetPos = m_attackTargetMineTsumuri.lock()->GetPos();
			targetScale = m_attackTargetMineTsumuri.lock()->GetScale();
		}
		else if (!m_attackDestrutibleTree.expired()) {
			targetPos = m_attackDestrutibleTree.lock()->GetPosZeroY();
			targetScale = m_attackDestrutibleTree.lock()->GetScale();
		}

		//�ړ����x���グ��B
		//m_attackMoveSpeed = std::clamp(m_attackMoveSpeed + ADD_ATTACK_SPEED, 0.0f, MAX_ATTACK_SPEED);


		++m_moveSpan;
		if (m_randomMoveSpan <= m_moveSpan) {

			//�ǔ�����B
			m_moveVec = (targetPos - GetPosZeroY()).GetNormal();
			m_moveVec.y = 1.0f;
			m_moveVec.Normalize();
			m_moveVec *= MOVE_SPEED[static_cast<int>(m_mineralID)];

			m_moveSpan = 0;
			if (move_span_count < MOVE_SPAN_COUNT_MAX)
			{
				move_span_count++;
			}
			else
			{
				move_span_count = 0;

				if (arg_moveSECount < 1) {
					SoundManager::Instance()->SoundPlayerWave(walk, 0);
				}
			}
		}

		//�ړ�����x�N�g�������߂�B
		KazMath::Vec3<float> moveDir = KazMath::Vec3<float>(targetPos - GetPosZeroY());
		float distance = moveDir.Length();
		moveDir.Normalize();

		//�ړ����x�������𒴂��Ă�����͈͂Ɏ��߂�B
		//m_attackMoveSpeed = std::clamp(m_attackMoveSpeed, 0.0f, distance);

		//�ړ�������B
		//m_transform.pos += moveDir * m_attackMoveSpeed;

		//���������ȉ��ɂȂ�����ҋ@��Ԃ�
		if (KazMath::Vec3<float>(targetPos - m_transform.pos).Length() <= targetScale.x + m_transform.scale.x) {

			m_attackID = STAY;

			//�����Ő�����΂��B
			SoundManager::Instance()->SoundPlayerWave(attack, 0);
			KazMath::Vec3<float> reactionDir = moveDir;
			reactionDir *= -1.0f;
			reactionDir.y = 1.0f;
			reactionDir.Normalize();
			m_attackReactionVec = reactionDir * (4.0f + KazMath::Rand(0.0f, 4.0f));

			//�₾������
			if (!m_attackTargetRock.expired()) {

				//��Ƀ_���[�W��^����B
				int rockHP = m_attackTargetRock.lock()->GetHP();
				m_attackTargetRock.lock()->Damage(moveDir * m_attackMoveSpeed / 8.0f);

				//�|�����u�Ԃ�������B
				if (0 < rockHP && m_attackTargetRock.lock()->GetHP() <= 0) {

					if (!m_attackTargetRock.lock()->GetIsMineralRock() && m_attackTargetRock.lock()->IsSmall()) {

						//��ł����~�l�����ɂ���B
						switch (m_mineralID)
						{
						case Mineral::SMALL:
							m_mineralID = MEDIUM;
							break;
						case Mineral::MEDIUM:
							//m_mineralID = BIG;
							break;
						default:
							break;
						}

					}

					m_isAttack = false;

					//�v���C���[�̉e���͈͊O������������Ԃ������B
					float playerDistance = KazMath::Vec3<float>(arg_player.lock()->GetPosZeroY() - GetPosZeroY()).Length();
					if (arg_player.lock()->GetMineralBreakUpRange() < playerDistance) {

						m_isGathering = false;

					}

				}

			}
			else if (!m_attackTargetMineKuji.expired()) {

				m_attackTargetMineKuji.lock()->Damage(m_thisMineral, ATTACK_DAMAGE[static_cast<int>(m_mineralID)]);

			}
			else if (!m_attackTargetMineTsumuri.expired()) {

				m_attackTargetMineTsumuri.lock()->Damage(m_thisMineral, ATTACK_DAMAGE[static_cast<int>(m_mineralID)]);

			}
			else if (!m_attackDestrutibleTree.expired()) {

				m_attackDestrutibleTree.lock()->Damage(ATTACK_DAMAGE[static_cast<int>(m_mineralID)]);

			}

			ShakeMgr::Instance()->m_shakeAmount = 1.0f;

		}

	}
	break;
	case Mineral::STAY:

		m_stayDelayTimer = std::clamp(m_stayDelayTimer + 1, 0, m_stayDelay);
		if (m_stayDelay <= m_stayDelayTimer) {

			m_attackID = ATTACK;
			m_attackMoveSpeed = 0.0f;
			m_stayDelayTimer = 0;

		}

		break;
	default:
		break;
	}

	//���HP��0�ɂȂ��Ă�����U�����I����B
	if (!m_attackTargetRock.expired() && m_attackTargetRock.lock()->GetHP() <= 0) {

		m_isAttack = false;

		//�v���C���[�̉e���͈͊O������������Ԃ������B
		float playerDistance = KazMath::Vec3<float>(arg_player.lock()->GetPosZeroY() - GetPosZeroY()).Length();
		if (arg_player.lock()->GetMineralBreakUpRange() < playerDistance) {

			m_isGathering = false;

		}

	}
	//�~�l�N�W��HP��0�ɂȂ��Ă�����U�����I����B
	if (!m_attackTargetMineKuji.expired() && m_attackTargetMineKuji.lock()->GetHP() <= 0) {

		m_isAttack = false;

		//�v���C���[�̉e���͈͊O������������Ԃ������B
		float playerDistance = KazMath::Vec3<float>(arg_player.lock()->GetPosZeroY() - GetPosZeroY()).Length();
		if (arg_player.lock()->GetMineralBreakUpRange() < playerDistance) {

			m_isGathering = false;

		}

	}
	//�~�l�N�W��HP��0�ɂȂ��Ă�����U�����I����B
	if (!m_attackTargetMineTsumuri.expired() && m_attackTargetMineTsumuri.lock()->GetHP() <= 0) {

		m_isAttack = false;

		//�v���C���[�̉e���͈͊O������������Ԃ������B
		float playerDistance = KazMath::Vec3<float>(arg_player.lock()->GetPosZeroY() - GetPosZeroY()).Length();
		if (arg_player.lock()->GetMineralBreakUpRange() < playerDistance) {

			m_isGathering = false;

		}

	}
	//�؂�HP��0�ɂȂ��Ă�����U�����I����B
	if (!m_attackDestrutibleTree.expired() && m_attackDestrutibleTree.lock()->GetHP() <= 0) {

		m_isAttack = false;

		//�v���C���[�̉e���͈͊O������������Ԃ������B
		float playerDistance = KazMath::Vec3<float>(arg_player.lock()->GetPosZeroY() - GetPosZeroY()).Length();
		if (arg_player.lock()->GetMineralBreakUpRange() < playerDistance) {

			m_isGathering = false;

		}

	}

}

void Mineral::UpdateHaveMaterial(std::weak_ptr<Player> arg_player)
{

	m_haveMaterial.lock()->SetPos(m_transform.pos + KazMath::Vec3<float>(0, 20, 0));

}
