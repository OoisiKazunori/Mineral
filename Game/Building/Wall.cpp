#include "Wall.h"
#include "../KazLibrary/Input/KeyBoradInputManager.h"
#include "../Game/Player.h"
#include "../KazLibrary/Easing/easing.h"
#include "../Game/Effect/ShakeMgr.h"
#include "../Game/UI/NumberFont.h"
#include "../Game/Wave/WaveMgr.h"
#include "../Game/Tutorial.h"

Wall::Wall()
{

	m_model[0].LoadOutline("Resource/Building/Wall/", "WallA.gltf");
	m_model[1].LoadOutline("Resource/Building/Wall/", "WallB.gltf");
	m_model[2].LoadOutline("Resource/Building/Wall/", "WallC.gltf");
	m_buildingBoxModel.LoadOutline("Resource/Building/", "BuildingBox.gltf");
	m_numberModel.LoadNoLighting("Resource/UI/NumFont/", "number.gltf");
	/*�I�J���g�]�[��*/
	m_hpBoxModel.LoadNoLighting("Resource/HpBox/", "Hp_Box.gltf");
	/*�I�J���g�]�[��*/

	m_wallTransform.pos.y = -10000.0f;
	m_meshCollisionModel[0].Load("Resource/Building/Wall/", "WallACollision.gltf");
	m_meshCollisionModel[1].Load("Resource/Building/Wall/", "WallBCollision.gltf");
	m_meshCollisionModel[2].Load("Resource/Building/Wall/", "WallCCollision.gltf");
	m_meshCollider[0].Setting(m_meshCollisionModel[0].m_modelData->modelData[0].vertexData, m_wallTransform);
	m_meshCollider[1].Setting(m_meshCollisionModel[1].m_modelData->modelData[0].vertexData, m_wallTransform);
	m_meshCollider[2].Setting(m_meshCollisionModel[2].m_modelData->modelData[0].vertexData, m_wallTransform);

	//SE
	wall_drop = SoundManager::Instance()->SoundLoadWave("Resource/Sound/drop.wav");
	wall_drop.volume = 0.07f;
	wall_build = SoundManager::Instance()->SoundLoadWave("Resource/Sound/build.wav");
	wall_build.volume = 0.1f;
	wall_break01 = SoundManager::Instance()->SoundLoadWave("Resource/Sound/break.wav");
	wall_break01.volume = 0.1f;
	wall_break02 = SoundManager::Instance()->SoundLoadWave("Resource/Sound/rock.wav");
	wall_break02.volume = 0.1f;
	damage = SoundManager::Instance()->SoundLoadWave("Resource/Sound/Object_Damage.wav");
	damage.volume = 0.05f;
	m_modelIndex = 0;
	Init();

}

void Wall::Init()
{

	m_isActive = false;
	m_isBuild = false;
	m_isReady = false;
	m_isOldReady = false;
	m_materialCounter = 0;
	m_sineWaveTimer = 0.0f;
	m_easingTimer = EXIT_EASING_TIMER;
	m_noreadyStatus = NOREADY_STATUS::EXIT;
	m_wallTransform.pos.y = -1000.0f;
	m_meshCollider[m_modelIndex].Transform(m_wallTransform);
	m_hp = HP;

	/*�I�J���g�]�[��*/
	isDrawHpBox = false;
	hpBoxScaleStart = 0.0f;
	hpBoxScaleEnd = 0.0f;
	m_hpBoxDrawTimer = HP_BOX_DRAW_TIME_MAX;
	hpBoxEaseTime = HP_BOX_EASE_TIME_MAX;
	ease_scale = 0.0f;
	m_hpBoxTransform.scale.y = 2.0f;
	m_hpBoxTransform.scale.z = 2.0f;
	m_hpBoxTransform.scale.x = m_hp * 1.5f;
	/*�I�J���g�]�[��*/
}

void Wall::Genrate(KazMath::Vec3<float> arg_generatePos, float arg_rotateY, int arg_modelIndex)
{

	m_transform.pos = arg_generatePos;
	m_initPos = arg_generatePos;
	Init();

	m_isActive = true;
	m_rotateY = arg_rotateY;
	m_initRotateY = arg_rotateY;
	m_boxTransform.rotation.y = arg_rotateY;
	m_modelIndex = arg_modelIndex;

}

void Wall::Update(std::weak_ptr<Player> arg_player)
{



	/*�I�J���g�]�[��*/
	if (isDrawHpBox)
	{
		if (m_hpBoxDrawTimer >= 0)
		{
			m_hpBoxDrawTimer--;
			//�X�P�[���̃C�[�W���O
			if (hpBoxEaseTime > 0)
			{
				ease_scale = hpBoxScaleStart - (damageAmount - EasingMaker(In, Circ, hpBoxEaseTime / HP_BOX_EASE_TIME_MAX) * damageAmount);
				ease_scale *= 1.5f;
				m_hpBoxTransform.scale.x = ease_scale;
				hpBoxEaseTime--;
			}

			else
			{
				m_hpBoxTransform.scale.x = hpBoxScaleEnd * 1.5f;
			}
		}

		else
		{
			isDrawHpBox = false;
		}
	}

	//���ɂȂ�����HP��S��
	if (!WaveMgr::Instance()->GetIsNight())
	{
		m_hp = HP;
	}
	/*�I�J���g�]�[��*/

	if (!m_isActive) return;

	//���z������
	if (m_isBuild) {

		switch (m_buildStatus)
		{
		case Wall::BUILD_STATUS::UPPER:
		{
			m_easingTimer = std::clamp(m_easingTimer + 1.0f, 0.0f, UPPER_EASING_TIMER);
			float easingAmount = EasingMaker(Out, Cubic, m_easingTimer / UPPER_EASING_TIMER);
			m_transform.pos.y = m_initPos.y + UPPER_DISTANCE * easingAmount;
			m_transform.scale = { 2.0f + easingAmount * 2.0f,2.0f + easingAmount * 2.0f ,2.0f + easingAmount * 2.0f };
			if (UPPER_EASING_TIMER <= m_easingTimer) {
				m_easingTimer = 0.0f;
				m_buildStatus = BUILD_STATUS::STAY;

			}
		}
		break;
		case Wall::BUILD_STATUS::STAY:
		{
			m_easingTimer = std::clamp(m_easingTimer + 1.0f, 0.0f, BUILD_STAY_EASING_TIMER);
			if (BUILD_STAY_EASING_TIMER <= m_easingTimer) {

				m_easingTimer = 0.0f;
				m_buildStatus = BUILD_STATUS::DOWN;

			}
		}
		break;
		case Wall::BUILD_STATUS::DOWN:
		{
			m_easingTimer = std::clamp(m_easingTimer + 1.0f, 0.0f, DOWN_EASING_TIMER);
			float easingAmount = EasingMaker(In, Exp, m_easingTimer / DOWN_EASING_TIMER);
			m_transform.pos.y = m_initPos.y + UPPER_DISTANCE - (UPPER_DISTANCE * easingAmount);
			if (DOWN_EASING_TIMER <= m_easingTimer) {
				SoundManager::Instance()->SoundPlayerWave(wall_drop, 0);

				m_easingTimer = 0.0f;
				m_buildStatus = BUILD_STATUS::COMPLETE;

				ShakeMgr::Instance()->m_shakeAmount = 5.0f;

				m_wallTransform = m_transform;
				m_meshCollider[m_modelIndex].Setting(m_meshCollisionModel[m_modelIndex].m_modelData->modelData[0].vertexData, m_wallTransform);

				//�`���[�g���A�������ɔ�΂�
				//���̃`���[�g���A���ɑ���
				if (Tutorial::Instance()->tutorial_num == 10)
				{
					Tutorial::Instance()->is_next = true;
				}
			}
		}
		break;
		case Wall::BUILD_STATUS::COMPLETE:
		{
		}
		break;
		default:
			break;
		}

		//Y����]�����ɖ߂��B
		m_transform.rotation.y += (m_rotateY - m_transform.rotation.y) / 5.0f;

	}
	//�܂����z���Ă��Ȃ�������
	else {

		//�ꏊ��ݒ�B
		m_transform.pos = m_initPos;
		m_boxTransform.pos = m_initPos;

		//�߂Â�����Ready��Ԃɂ���B
		float distance = KazMath::Vec3<float>(GetPosZeroY() - arg_player.lock()->GetPosZeroY()).Length();
		m_isOldReady = m_isReady;
		if (distance < arg_player.lock()->GetMineralAffectRange()) {
			m_isReady = true;
		}
		else {
			m_isReady = false;
		}

		//Ready�ɂȂ����u�Ԃ�������
		if (!m_isOldReady && m_isReady) {
			m_noreadyStatus = NOREADY_STATUS::APPEAR;
			m_easingTimer = 0.0f;
			m_transform.rotation.y = m_rotateY + 360.0f * 2.0f;
		}
		//Ready���������ꂽ�u�Ԃ�����r��
		if (m_isOldReady && !m_isReady) {
			m_noreadyStatus = NOREADY_STATUS::EXIT;
			m_easingTimer = 0.0f;
		}

		//�X�e�[�^�X�ɂ���đ傫����ς����肷��B
		switch (m_noreadyStatus)
		{
		case Wall::NOREADY_STATUS::APPEAR:
		{
			m_easingTimer = std::clamp(m_easingTimer + 1.0f, 0.0f, APPEAR_EASING_TIMER);
			float easingAmount = EasingMaker(Out, Back, m_easingTimer / APPEAR_EASING_TIMER);
			float scale = 2.0f * easingAmount;
			m_transform.scale = { scale ,scale ,scale };
			m_boxTransform.scale = { 2.0f - scale ,2.0f - scale ,2.0f - scale };

			if (APPEAR_EASING_TIMER <= m_easingTimer) {

				m_easingTimer = 0.0f;
				m_noreadyStatus = NOREADY_STATUS::STAY;

			}

		}
		break;
		case Wall::NOREADY_STATUS::STAY:
		{


		}
		break;
		case Wall::NOREADY_STATUS::EXIT:
		{
			m_easingTimer = std::clamp(m_easingTimer + 1.0f, 0.0f, EXIT_EASING_TIMER);
			float easingAmount = EasingMaker(In, Back, m_easingTimer / EXIT_EASING_TIMER);
			float scale = 2.0f - 2.0f * easingAmount;
			m_transform.scale = { scale ,scale ,scale };
			m_boxTransform.scale = { 2.0f - scale ,2.0f - scale ,2.0f - scale };

		}
		break;
		default:
			break;
		}

		//�T�C���g�̓������X�V�B
		m_sineWaveTimer += ADD_SINE_WAVE_TIMER;

		if (MATERIAL_COUNT <= m_materialCounter) {
			SoundManager::Instance()->SoundPlayerWave(wall_build, 0);
			m_isBuild = true;
			m_buildStatus = BUILD_STATUS::UPPER;
			m_easingTimer = 0.0f;
		}

		//Y����]�����ɖ߂��B
		m_transform.rotation.y += (m_rotateY - m_transform.rotation.y) / 20.0f;

	}

	//HP��0�ɂȂ����珉����
	if (m_hp <= 0) {

		SoundManager::Instance()->SoundPlayerWave(wall_break01, 0);
		SoundManager::Instance()->SoundPlayerWave(wall_break02, 0);
		m_transform.pos = m_initPos;
		Init();

		m_isActive = true;
		m_rotateY = m_initRotateY;
		m_boxTransform.rotation.y = m_initRotateY;
	}
}

void Wall::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{

	KazMath::Transform3D modelTransform = m_transform;
	modelTransform.pos.y += std::sin(m_sineWaveTimer) * SINE_WAVE_MOVE;

	KazMath::Transform3D boxTransform = m_boxTransform;
	boxTransform.scale += KazMath::Vec3<float>(std::sin(m_sineWaveTimer) * BOX_SINE_WAVE_SCALE, std::sin(m_sineWaveTimer) * BOX_SINE_WAVE_SCALE, std::sin(m_sineWaveTimer) * BOX_SINE_WAVE_SCALE);

	if (!m_isActive) {

		boxTransform.pos = { 0,-10000,0 };
		boxTransform.scale = { 0,0,0 };

		modelTransform.pos = { 0,-10000,0 };
		modelTransform.scale = { 0,0,0 };

	}

	if (m_noreadyStatus == NOREADY_STATUS::STAY) {
		boxTransform = m_boxTransform;
	}

	if (m_isBuild) {
		boxTransform.scale = { 0,0,0 };
	}

	if (m_isReady && !m_isBuild) {

		m_numberModelTransform.pos = boxTransform.pos;
		m_numberModelTransform.pos.y += 30.0f;
		m_numberModelTransform.scale.x = 5.0f;
		m_numberModelTransform.scale.z = 5.0f;
		m_numberModelTransform.scale.y += (5.0f - m_numberModelTransform.scale.y) / 5.0f;

	}
	else {

		m_numberModelTransform.pos = boxTransform.pos;
		m_numberModelTransform.pos.y += 30.0f;
		m_numberModelTransform.scale.x = 5.0f;
		m_numberModelTransform.scale.z = 5.0f;
		m_numberModelTransform.scale.y += (0.0f - m_numberModelTransform.scale.y) / 5.0f;
		if (m_numberModelTransform.scale.y < 0.5f) {
			m_numberModelTransform.scale.x = 0.0f;
			m_numberModelTransform.scale.y = 0.0f;
			m_numberModelTransform.scale.z = 0.0f;
		}
	}

	m_numberModel.m_model.materialBuffer.front().front() = NumberFont::Instance()->m_font[MATERIAL_COUNT - m_materialCounter];

	DessolveOutline outline;
	outline.m_outline = KazMath::Vec4<float>(0.5f, 0, 0, 1);
	m_buildingBoxModel.m_model.extraBufferArray[4].bufferWrapper->TransData(&outline, sizeof(DessolveOutline));
	m_buildingBoxModel.m_model.extraBufferArray.back() = GBufferMgr::Instance()->m_outlineBuffer;
	m_buildingBoxModel.m_model.extraBufferArray.back().rangeType = GRAPHICS_RANGE_TYPE_UAV_DESC;
	m_buildingBoxModel.m_model.extraBufferArray.back().rootParamType = GRAPHICS_PRAMTYPE_TEX;

	m_model[m_modelIndex].m_model.extraBufferArray[4].bufferWrapper->TransData(&outline, sizeof(DessolveOutline));
	m_model[m_modelIndex].m_model.extraBufferArray.back() = GBufferMgr::Instance()->m_outlineBuffer;
	m_model[m_modelIndex].m_model.extraBufferArray.back().rangeType = GRAPHICS_RANGE_TYPE_UAV_DESC;
	m_model[m_modelIndex].m_model.extraBufferArray.back().rootParamType = GRAPHICS_PRAMTYPE_TEX;

	m_model[m_modelIndex].Draw(arg_rasterize, arg_blasVec, modelTransform);
	m_buildingBoxModel.Draw(arg_rasterize, arg_blasVec, boxTransform);
	m_numberModel.Draw(arg_rasterize, arg_blasVec, m_numberModelTransform, 0, false);

	/*�I�J���g�]�[��*/
	m_hpBoxTransform.pos = m_transform.pos;
	m_hpBoxTransform.pos.y += 30.0f;
	m_hpBoxTransform.rotation.y = 45.0f;

	if (isDrawHpBox)
	{
		m_hpBoxModel.Draw(arg_rasterize, arg_blasVec, m_hpBoxTransform, 0, false);
	}
	/*�I�J���g�]�[��*/
}

MeshCollision::CheckHitResult Wall::CheckHitMesh(KazMath::Vec3<float> arg_pos, KazMath::Vec3<float> arg_dir) {

	return m_meshCollider[m_modelIndex].CheckHitRay(arg_pos, arg_dir);

}

KazMath::Vec3<float> Wall::GetPosZeroY()
{
	return KazMath::Vec3<float>(m_initPos.x, 0.0f, m_initPos.z);
}

void Wall::Damage(int arg_damage)
{
	SoundManager::Instance()->SoundPlayerWave(damage, 0);
	/*�I�J���g�]�[��*/
	damageAmount = static_cast<float>(arg_damage);
	isDrawHpBox = true;
	m_hpBoxDrawTimer = HP_BOX_DRAW_TIME_MAX;
	hpBoxScaleStart = static_cast <float>(m_hp);
	hpBoxScaleEnd = static_cast<float> (m_hp - arg_damage);
	hpBoxEaseTime = HP_BOX_EASE_TIME_MAX;
	/*�I�J���g�]�[��*/

	//hp��������鏈��
	m_hp = std::clamp(m_hp - arg_damage, 0, HP);
}
