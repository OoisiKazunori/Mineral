#include "Rock.h"
#include "../Game/Player.h"
#include "../Game/Effect/ShakeMgr.h"
#include "../Game/Tutorial.h"

Rock::Rock()
{

	m_isAlive = false;
	m_gravity = 0;
	m_rockID = BIG;

	m_transform.pos = { 10000,-10000,10000 };
	m_transform.scale = { 0,0,0 };

	m_model[0].LoadOutline("Resource/Rock/", "Rock_S_01.gltf");
	m_model[1].LoadOutline("Resource/Rock/", "Rock_S_02.gltf");
	m_model[2].LoadOutline("Resource/Rock/", "Rock_S_03.gltf");
	m_model[3].LoadOutline("Resource/Mineral/Lerge/", "Mineral_L.gltf");
	m_modelIndex = 0;

	//SE
	rock_break01 = SoundManager::Instance()->SoundLoadWave("Resource/Sound/rock.wav");
	rock_break02 = SoundManager::Instance()->SoundLoadWave("Resource/Sound/break.wav");
	rock_break01.volume = 0.05f;
	rock_break02.volume = 0.05f;
}

void Rock::Init()
{

	m_isAlive = false;
	m_gravity = 0;
	m_reactionVec = {};

}

void Rock::Generate(KazMath::Vec3<float> arg_pos, KazMath::Vec3<float> arg_respawnVec, bool arg_isMineralRock, int arg_rockID)
{

	m_rockID = static_cast<ROCK_ID>(arg_rockID);
	m_transform.pos = arg_pos;
	m_respawnVec = arg_respawnVec;
	m_isAlive = true;
	m_canCollision = false;
	m_canOldCollision = false;
	m_gravity = 0;
	m_reactionVec = {};
	m_isMineralRock = arg_isMineralRock;

	if (m_isMineralRock) {
		m_hp = 1;
	}
	else {
		m_hp = MAX_HP[static_cast<int>(m_rockID)];
	}

	m_modelIndex = KazMath::Rand(0, 2);

}

void Rock::Update(std::weak_ptr<Player> arg_player, std::vector<std::pair<KazMath::Vec3<float>, int>>& arg_respawnVec)
{

	using namespace KazMath;

	//与えられた初速を座標に足す。
	if (0.01f < m_respawnVec.Length()) {

		m_transform.pos += m_respawnVec;
		m_respawnVec -= m_respawnVec / 10.0f;

	}
	else {

		m_respawnVec = {};

	}

	//攻撃を食らったときの移動を足す。
	if (0.01f < m_reactionVec.Length()) {

		m_transform.pos += m_reactionVec;
		m_reactionVec -= m_reactionVec / 10.0f;

	}
	else {

		m_reactionVec = {};

	}

	//空中に浮いていたら重力を計算する。
	m_canOldCollision = m_canCollision;
	if (0 < m_transform.pos.y) {

		m_gravity += GRAVITY;
		m_transform.pos.y -= m_gravity;
		m_canCollision = false;

	}
	else {

		m_gravity = 0.0f;
		m_transform.pos.y = 0.0f;
		m_canCollision = true;

	}

	//プレイヤーとの当たり判定を行う。
	if (m_canCollision && arg_player.lock()->GetIsDaipanTrigger()) {

		//当たり判定チェック
		bool isHit = Vec3<float>(arg_player.lock()->GetTransform().pos - m_transform.pos).Length() <= arg_player.lock()->GetMineralAffectStrongRange() + m_transform.scale.x;
		if (isHit) {

			//ベクトルチェック。
			KazMath::Vec3<float> dir = KazMath::Vec3<float>(m_transform.pos - arg_player.lock()->GetTransform().pos);
			dir.y = 0.0f;
			dir.Normalize();

			if (m_rockID == SMALL) {

				m_respawnVec = { Vec3<float>(KazMath::Rand(-0.1f, 0.1f) ,KazMath::Rand(0.5f, 2.0f) ,KazMath::Rand(-0.1f, 0.1f)) };
				m_respawnVec.Normalize();
			}
			else {

				//HPを減らす。
				Damage(dir / 5.0f, 4);
				if (m_hp <= 0.0f) {
					SoundManager::Instance()->SoundPlayerWave(rock_break01, 0);
					SoundManager::Instance()->SoundPlayerWave(rock_break02, 0);
					int id = 0;
					switch (m_rockID)
					{
					case ROCK_ID::SMALL:
						break;
					case ROCK_ID::MEDIUM:
						id = SMALL;
						//強力なシェイクをかける。
						ShakeMgr::Instance()->m_shakeAmount = 2.0f;
						break;
					case ROCK_ID::BIG:
						id = SMALL;
						//強力なシェイクをかける。
						ShakeMgr::Instance()->m_shakeAmount = 3.0f;
						break;
					default:
						break;
					}

					//ミネラル岩だったら
					if (m_isMineralRock) {

						//分散させる。
						for (int index = 0; index < 3; ++index) {
							std::pair<KazMath::Vec3<float>, int> respawnData;
							respawnData.first = { Vec3<float>(KazMath::Rand(-1.0f, 1.0f) ,KazMath::Rand(-0.0f, 2.0f) ,KazMath::Rand(-1.0f, 1.0f)) };
							respawnData.first.GetNormal();
							respawnData.first *= STRONG_DAIPAN_POWER;
							respawnData.second = id;
							arg_respawnVec.emplace_back(respawnData);
						}

					}
					else {


						//分散させる。
						for (int index = 0; index < 3; ++index) {
							std::pair<KazMath::Vec3<float>, int> respawnData;
							respawnData.first = { Vec3<float>(KazMath::Rand(-1.0f, 1.0f) ,KazMath::Rand(-0.0f, 2.0f) ,KazMath::Rand(-1.0f, 1.0f)) };
							respawnData.first.GetNormal();
							respawnData.first *= STRONG_DAIPAN_POWER;
							respawnData.second = id;
							arg_respawnVec.emplace_back(respawnData);
						}

					}

					//こいつ本体は消す。
					Init();

					//次のチュートリアルに送る
					if (Tutorial::Instance()->tutorial_num == 3)
					{
						Tutorial::Instance()->is_next = true;
					}

					return;


				}

			}

		}

	}

	//ミネラルのサイズをステータスによって変更させる。
	if (m_isMineralRock) {

		m_transform.scale = KazMath::Vec3(6.0f, 6.0f, 6.0f);

	}
	else {
		switch (m_rockID)
		{
		case ROCK_ID::SMALL:
			m_transform.scale = { DRAW_SIZE_SMALL,DRAW_SIZE_SMALL ,DRAW_SIZE_SMALL };
			break;
		case ROCK_ID::MEDIUM:
			m_transform.scale = { DRAW_SIZE_MEDIUM,DRAW_SIZE_MEDIUM ,DRAW_SIZE_MEDIUM };
			break;
		case ROCK_ID::BIG:
			m_transform.scale = { DRAW_SIZE_BIG,DRAW_SIZE_BIG ,DRAW_SIZE_BIG };
			break;
		default:
			break;
		}
	}

	//HPが0になったら初期化
	if (m_hp <= 0) {
		SoundManager::Instance()->SoundPlayerWave(rock_break01, 0);
		SoundManager::Instance()->SoundPlayerWave(rock_break02, 0);
		if (m_rockID != SMALL) {

			int id = 0;
			switch (m_rockID)
			{
			case ROCK_ID::SMALL:
				break;
			case ROCK_ID::MEDIUM:
				id = SMALL;
				//強力なシェイクをかける。
				ShakeMgr::Instance()->m_shakeAmount = 2.0f;
				break;
			case ROCK_ID::BIG:
				id = SMALL;
				//強力なシェイクをかける。
				ShakeMgr::Instance()->m_shakeAmount = 3.0f;
				break;
			default:
				break;
			}

			//ミネラル岩だったら
			if (m_isMineralRock) {

				//分散させる。
				for (int index = 0; index < 3; ++index) {
					std::pair<KazMath::Vec3<float>, int> respawnData;
					respawnData.first = { Vec3<float>(KazMath::Rand(-1.0f, 1.0f) ,KazMath::Rand(-0.0f, 2.0f) ,KazMath::Rand(-1.0f, 1.0f)) };
					respawnData.first.GetNormal();
					respawnData.first *= STRONG_DAIPAN_POWER;
					respawnData.second = id;
					arg_respawnVec.emplace_back(respawnData);
				}

			}
			else {


				//分散させる。
				for (int index = 0; index < 3; ++index) {
					std::pair<KazMath::Vec3<float>, int> respawnData;
					respawnData.first = { Vec3<float>(KazMath::Rand(-1.0f, 1.0f) ,KazMath::Rand(-0.0f, 2.0f) ,KazMath::Rand(-1.0f, 1.0f)) };
					respawnData.first.GetNormal();
					respawnData.first *= STRONG_DAIPAN_POWER;
					respawnData.second = id;
					arg_respawnVec.emplace_back(respawnData);
				}

			}

		}
		else {

			ShakeMgr::Instance()->m_shakeAmount = 1.0f;

		}

		//こいつ本体は消す。
		Init();

		//次のチュートリアルに送る
		if (Tutorial::Instance()->tutorial_num == 5)
		{
			Tutorial::Instance()->is_next = true;
		}
	}
}

void Rock::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{

	if (!m_isAlive) {

		m_transform.scale = { 0,0,0 };

	}

	DessolveOutline outline;
	outline.m_outline = KazMath::Vec4<float>(0.5f, 0, 0, 1);

	if (m_isMineralRock) {

		m_model.back().m_model.extraBufferArray[4].bufferWrapper->TransData(&outline, sizeof(DessolveOutline));

		m_model.back().m_model.extraBufferArray.back() = GBufferMgr::Instance()->m_outlineBuffer;
		m_model.back().m_model.extraBufferArray.back().rangeType = GRAPHICS_RANGE_TYPE_UAV_DESC;
		m_model.back().m_model.extraBufferArray.back().rootParamType = GRAPHICS_PRAMTYPE_TEX;

		KazMath::Transform3D transform = m_transform;
		transform.rotation.x = 180.0f;
		transform.pos.y += 10.0f;
		m_model.back().Draw(arg_rasterize, arg_blasVec, transform);

	}
	else {

		m_model[m_modelIndex].m_model.extraBufferArray[4].bufferWrapper->TransData(&outline, sizeof(DessolveOutline));

		m_model[m_modelIndex].m_model.extraBufferArray.back() = GBufferMgr::Instance()->m_outlineBuffer;
		m_model[m_modelIndex].m_model.extraBufferArray.back().rangeType = GRAPHICS_RANGE_TYPE_UAV_DESC;
		m_model[m_modelIndex].m_model.extraBufferArray.back().rootParamType = GRAPHICS_PRAMTYPE_TEX;

		m_model[m_modelIndex].Draw(arg_rasterize, arg_blasVec, m_transform);

	}

}

void Rock::Damage(KazMath::Vec3<float> arg_reactionVec, int arg_damage)
{

	m_hp = std::clamp(m_hp - arg_damage, 0, MAX_HP.back());
	m_reactionVec = arg_reactionVec;

}
