#include "DestructibleTree.h"
#include "../Game/BuildingMaterial/BuildingMaterialMgr.h"
#include "../Game/Tutorial.h"
#include "../Game/Player.h"

DestructibleTree::DestructibleTree()
{

	m_model.LoadOutline("Resource/DestructibleObject/Tree/", "Tree.gltf");
	m_transform.scale = { 5,5,5 };
	m_isActive = false;
	m_hp = HP;

	//SE
	break_se = SoundManager::Instance()->SoundLoadWave("Resource/Sound/break_tree.wav");
	break_se.volume = 0.1f;

}

void DestructibleTree::Init()
{

	m_isActive = false;
	m_hp = HP;

}

void DestructibleTree::Generate(KazMath::Vec3<float> arg_generatePos)
{

	m_isActive = true;
	m_transform.pos = arg_generatePos;
	m_hp = HP;

}

void DestructibleTree::Update(std::weak_ptr<Player> arg_player)
{

	using namespace KazMath;

	//プレイヤーとの当たり判定を行う。
	if (arg_player.lock()->GetIsDaipanTrigger()) {

		//当たり判定チェック
		bool isHit = Vec3<float>(arg_player.lock()->GetTransform().pos - m_transform.pos).Length() <= arg_player.lock()->GetMineralAffectStrongRange() + m_transform.scale.x * 4.0f;
		if (isHit) {

			//HPを減らす。
			Damage(4);

		}

	}

	if (m_hp <= 0) {
		m_isActive = false;
		SoundManager::Instance()->SoundPlayerWave(break_se, 0);
		//素材を散らばらせる。
		const int MATERIAL_COUNT = 3;
		for (int index = 0; index < 3; ++index) {
			const float STRONG_DAIPAN_POWER = 2.0f;
			KazMath::Vec3<float> generateVec;
			generateVec = { KazMath::Vec3<float>(KazMath::Rand(-1.0f, 1.0f) ,KazMath::Rand(1.0f, 4.0f) ,KazMath::Rand(-1.0f, 1.0f)) };
			generateVec.Normalize();
			generateVec *= STRONG_DAIPAN_POWER;
			BuildingMaterialMgr::Instance()->Generate(m_transform.pos, generateVec);
		}

		//次のチュートリアルに送る
		if (Tutorial::Instance()->tutorial_num == 8)
		{
			Tutorial::Instance()->is_next = true;
		}
	}
}

void DestructibleTree::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{

	if (m_isActive) {
		m_transform.scale = { 5,5,5 };
	}
	else {
		m_transform.scale = { 0,0,0 };
	}




	DessolveOutline outline;
	outline.m_outline = KazMath::Vec4<float>(0.5f, 0, 0, 1);
	m_model.m_model.extraBufferArray[4].bufferWrapper->TransData(&outline, sizeof(DessolveOutline));

	m_model.m_model.extraBufferArray.back() = GBufferMgr::Instance()->m_outlineBuffer;
	m_model.m_model.extraBufferArray.back().rangeType = GRAPHICS_RANGE_TYPE_UAV_DESC;
	m_model.m_model.extraBufferArray.back().rootParamType = GRAPHICS_PRAMTYPE_TEX;

	m_model.Draw(arg_rasterize, arg_blasVec, m_transform);
}

void DestructibleTree::Damage(int arg_damage) {

	m_hp = std::clamp(m_hp - arg_damage, 0, HP);

}