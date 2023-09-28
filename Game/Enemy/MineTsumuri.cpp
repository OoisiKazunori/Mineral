#include "MineTsumuri.h"
#include "../Game/Core/Core.h"
#include "../Effect/ShakeMgr.h"
#include "../KazLibrary/Input/KeyBoradInputManager.h"
#include "../Game/Player.h"
#include "../Game/Mineral/Mineral.h"
#include "../Game/Building/BuildingMgr.h"
#include "../Game/Mineral/MineralMgr.h"
#include "../KazLibrary/Easing/easing.h"
#include "../Game/EnemyScore.h"

MineTsumuri::MineTsumuri()
{

	m_model.LoadOutline("Resource/Enemy/MineKuji/", "Minekuzi.gltf");
	m_shellModel.LoadOutline("Resource/Enemy/MineTsumuri/", "Minetsumuri.gltf");
	/*オカモトゾーン*/
	m_hpBoxModel.LoadNoLighting("Resource/HpBox/", "Hp_Box.gltf");
	m_gardHpBoxModel.LoadNoLighting("Resource/HpBox/", "Gard_Hp_Box.gltf");
	/*オカモトゾーン*/
	m_kingShellModel.LoadOutline("Resource/Enemy/MineKing/", "MineKing.gltf");
	m_attackedScale = 0.0f;
	m_scale = 0.0f;

	//SE
	shell_slap = SoundManager::Instance()->SoundLoadWave("Resource/Sound/Guard.wav");
	shell_slap.volume = 0.07f;
	attack = SoundManager::Instance()->SoundLoadWave("Resource/Sound/Attack.wav");
	attack.volume = 0.1f;

	Init();

}

void MineTsumuri::Init()
{

	m_isActive = false;
	m_isAttackWall = false;
	m_attackedScale = 0.0f;

	m_hp = HP;
	m_shellHP = 0.0f;

	/*オカモトゾーン*/
	m_gardHpBoxTransform.scale.y = 1.0f;
	m_gardHpBoxTransform.scale.z = 1.0f;
	m_gardHpBoxTransform.scale.x = static_cast<float> (SHELL_HP);
	m_hpBoxTransform.scale.y = 1.0f;
	m_hpBoxTransform.scale.z = 1.0f;
	m_hpBoxTransform.scale.x = static_cast<float> (HP);
	/*オカモトゾーン*/
}

void MineTsumuri::Generate(std::vector<KazMath::Vec3<float>> arg_route, bool arg_isKing)
{

	m_route = arg_route;
	m_passedRouteIndex = 0;

	m_transform.pos = arg_route.front();
	m_scale = 0;
	m_isActive = true;
	m_isAttackedMineral = false;
	m_isAttackCore = false;
	m_isAttackMineral = false;
	m_mode = CoreAttack;
	m_attackID = ATTACK;
	m_gravity = 0.0f;
	m_attackedScale = 0.0f;
	m_oldTransform = m_transform;
	m_knockBackVec = {};

	m_coreMoveSpeed = 0.0f;
	m_coreMoveDelayTimer = 0.0f;
	m_coreAttackDelayTimer = 0;
	m_coreAttackDelay = KazMath::Rand(MIN_CORE_ATTACK_DELAY, MAX_CORE_ATTACK_DELAY);

	m_isMineking = arg_isKing;

	if (m_isMineking) {
		m_hp = MINEKING_HP;
		m_shellHP = MINEKING_SHELL_HP;
	}
	else {
		m_hp = HP;
		m_shellHP = SHELL_HP;
	}
	m_attackedReactionVec = {};
	m_attackedMineral.reset();

	//次の進路の方向に身体を向ける。
	KazMath::Vec3<float> forwardVec = KazMath::Vec3<float>(arg_route[1] - m_transform.pos).GetNormal();
	float angle = std::acosf(KazMath::Vec3<float>(0, 0, 1).Dot(forwardVec));
	KazMath::Vec3<float> cross = KazMath::Vec3<float>(0, 0, 1).Cross(forwardVec);
	if (cross.Length() <= 0) {
		cross = { 0,1,0 };
	}
	DirectX::XMVECTOR rotateQ = DirectX::XMQuaternionRotationAxis({ cross.x, cross.y, cross.z }, angle);
	m_transform.quaternion = rotateQ;

	m_forwardVec = { 0,0,1 };
	m_isAttackWall = false;

	m_isShell = true;
	m_inShell = false;
	m_inShellTimer = 0.0f;
	m_shellPosY = SHELL_POS_Y;
	m_shellGravity = 0.0f;
	m_shellBreakVel = {};

	//出現地点をランダム化。
	m_transform.pos += KazMath::Vec3<float>(KazMath::Rand(-RANDOM_SPAWN_RANGE, RANDOM_SPAWN_RANGE), 0.0f, KazMath::Rand(-RANDOM_SPAWN_RANGE, RANDOM_SPAWN_RANGE));

}

void MineTsumuri::Update(std::weak_ptr<Core> arg_core, std::weak_ptr<Player> arg_player)
{

	using namespace KazMath;

	if (!m_isActive) {
		return;
	}

	//座標を保存しておく。
	m_oldTransform = m_transform;

	//殻にこもっているときは何もしない。
	if (!m_inShell) {

		//現在の状態によって処理を分ける。
		switch (m_mode)
		{
		case MineTsumuri::CoreAttack:
		{

			//近くにコアがあるかを検索する。
			float coreDistance = KazMath::Vec3<float>(arg_core.lock()->GetPosZeroY() - m_transform.pos).Length();
			//コアの近くに居なかったらコアの方向に向かって移動させる。
			if (CORE_ATTACK_RANGE < coreDistance) {

				//コアに向かって動く遅延。
				float coreMoveDelay = CORE_MOVE_DELAY;
				if (m_isMineking) {
					coreMoveDelay = CORE_MOVE_DELAY_MINEING;
				}

				//一定時間置きに初速度を与えて動かす。
				m_coreMoveDelayTimer = std::clamp(m_coreMoveDelayTimer + 1.0f, 0.0f, coreMoveDelay);
				if (coreMoveDelay <= m_coreMoveDelayTimer) {

					m_coreMoveSpeed = CORE_MOVE_SPEED;
					m_coreMoveDelayTimer = 0.0f;

				}

				if (!m_isMineking) {

					//近くにミネラルが居るか？
					int mineralIndex = 0;
					if (MineralMgr::Instance()->SearchNearMineral(GetPosZeroY(), ENEMY_SEARCH_RANGE, mineralIndex)) {

						m_mode = MineralAttack;
						m_isAttackedMineral = true;
						m_isAttackMineral = false;
						m_isAttackWall = false;
						m_isAttackPlayer = false;
						m_attackedMineral = MineralMgr::Instance()->GetMineral(mineralIndex);
						break;

					}

					//近くにプレイヤーが居るか？
					if (!arg_player.lock()->GetIsStun() && KazMath::Vec3<float>(arg_player.lock()->GetPosZeroY() - GetPosZeroY()).Length() <= ENEMY_SEARCH_RANGE) {


						m_mode = PlayerAttack;
						m_isAttackedMineral = false;
						m_isAttackMineral = false;
						m_isAttackWall = false;
						m_isAttackPlayer = true;
						break;

					}

					//敵に攻撃されたらすぐに迎撃状態に入る。
					if (m_isAttackedMineral) {

						m_mode = MineralAttack;

					}

				}

				m_isAttackCore = false;

			}
			//近くにコアがあったら攻撃状態に移る。
			else {

				AttackCore(arg_core);

				m_isAttackCore = true;

			}

			m_isAttackMineral = false;
			m_isAttackWall = false;
			m_isAttackPlayer = false;

		}
		break;
		case MineTsumuri::MineralAttack:
		{

			AttackMineral();

			m_isAttackMineral = true;

		}
		break;
		case MineTsumuri::WallAttack:
		{

			AttackWall();

		}
		break;
		case MineTsumuri::PlayerAttack:
		{

			AttackPlayer(arg_player);

		}
		break;
		default:
			break;
		}

	}
	//殻にこもっていたら殻から出るまでのタイマーを更新。
	else {

		m_inShellTimer = std::clamp(m_inShellTimer - 1.0f, 0.0f, IN_SHELL_TIMER);
		if (m_inShellTimer <= 0) {
			m_inShell = false;
		}

	}

	//動かす。
	Move();

	//当たり判定
	CheckHit(arg_player);

	//回転させる。
	Rotation(arg_core, arg_player);

	//殻を更新。
	UpdateShell();

	//HPが0になったら死亡
	if (m_hp <= 0) {
		m_isActive = false;


		//ミネキングだったら
		if (m_isMineking) {
			EnemyScore::Instance()->m_score += 250;
		}
		else {
			EnemyScore::Instance()->m_score += 50;
		}
	}

}

void MineTsumuri::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{

	//m_deadEffectEmitter.Draw(arg_rasterize, arg_blasVec);
	if (!m_isActive && m_scale <= 0.0f)
	{
		return;
	}

	float useScale = SCALE;
	if (m_isMineking) {

		useScale = KING_SCALE;

	}


	//スケールをデフォルトの値に近づける。
	if (m_inShell) {

		m_scale -= m_scale / 2.0f;

	}
	else if (m_isActive) {

		m_scale += (useScale - m_scale) / 5.0f;

	}
	else {

		m_scale -= m_scale / 5.0f;
		if (m_scale < 0.1f) {
			m_scale = 0.0f;
		}

	}

	//本体のスケールをデフォルトに近づける。
	if (m_isActive) {
		m_transform.scale = { m_scale + m_attackedScale ,m_scale - m_attackedScale ,m_scale + m_attackedScale };
	}
	else {
		m_transform.scale = { m_scale ,m_scale ,m_scale };
	}

	//殻のスケール
	if (m_isActive && m_isShell) {
		const float KING_SHELL_SCALE = 2.0f;
		m_shellTransform.scale = { SCALE + KING_SHELL_SCALE ,SCALE + KING_SHELL_SCALE ,SCALE + KING_SHELL_SCALE };
		m_shellTransform.pos -= m_forwardVec * 10.0f;

		//ちょっとX軸に回転させる。
		//m_shellTransform.quaternion = DirectX::XMQuaternionMultiply(m_shellTransform.quaternion, DirectX::XMQuaternionRotationAxis(TransformVec3({ 0,0,1 }, m_shellTransform.quaternion).ConvertXMVECTOR(), 0.1f));
	}

	DessolveOutline outline;
	outline.m_outline = KazMath::Vec4<float>(0.2f, 0, 0, 1);
	m_model.m_model.extraBufferArray[4].bufferWrapper->TransData(&outline, sizeof(DessolveOutline));
	m_model.m_model.extraBufferArray.back() = GBufferMgr::Instance()->m_outlineBuffer;
	m_model.m_model.extraBufferArray.back().rangeType = GRAPHICS_RANGE_TYPE_UAV_DESC;
	m_model.m_model.extraBufferArray.back().rootParamType = GRAPHICS_PRAMTYPE_TEX;
	m_model.Draw(arg_rasterize, arg_blasVec, m_transform);

	if (m_isMineking) {

		outline.m_outline = KazMath::Vec4<float>(0.2f, 0, 0, 1);
		m_kingShellModel.m_model.extraBufferArray[4].bufferWrapper->TransData(&outline, sizeof(DessolveOutline));
		m_kingShellModel.m_model.extraBufferArray.back() = GBufferMgr::Instance()->m_outlineBuffer;
		m_kingShellModel.m_model.extraBufferArray.back().rangeType = GRAPHICS_RANGE_TYPE_UAV_DESC;
		m_kingShellModel.m_model.extraBufferArray.back().rootParamType = GRAPHICS_PRAMTYPE_TEX;
		m_kingShellModel.Draw(arg_rasterize, arg_blasVec, m_shellTransform);

		/*オカモトゾーン*/
		m_gardHpBoxTransform.pos = m_transform.pos;
		m_gardHpBoxTransform.pos.y += 53.0f;
		m_gardHpBoxTransform.rotation.y = 45.0f;

		m_hpBoxTransform.pos = m_transform.pos;
		m_hpBoxTransform.pos.y += 50.0f;
		m_hpBoxTransform.rotation.y = 45.0f;

		float shell_base_hp = (m_shellHP / SHELL_HP) * SCALE_MAG;
		float mine_base_hp = (static_cast<float>(m_hp) / HP) * SCALE_MAG;
		m_gardHpBoxTransform.scale.x += (shell_base_hp - m_gardHpBoxTransform.scale.x) / 5.0f;
		if (fabs(mine_base_hp - m_hpBoxTransform.scale.x) > 0)
		{
			m_hpBoxTransform.scale.x += (mine_base_hp - m_hpBoxTransform.scale.x) / 5.0f;
		}

		if (m_isActive && isDrawHpBox)
		{
			if (m_hpBoxDrawTimer > 0)
			{
				if (m_shellHP >= 0.05f)
				{
					m_gardHpBoxModel.Draw(arg_rasterize, arg_blasVec, m_gardHpBoxTransform, 0, false);
				}
				if (m_hp >= 0.05f)
				{
					m_hpBoxModel.Draw(arg_rasterize, arg_blasVec, m_hpBoxTransform, 0, false);
				}
				m_hpBoxDrawTimer--;
			}
			else
			{
				isDrawHpBox = false;
			}
		}
		/*オカモトゾーン*/

	}
	else {

		outline.m_outline = KazMath::Vec4<float>(0.2f, 0, 0, 1);
		m_shellModel.m_model.extraBufferArray[4].bufferWrapper->TransData(&outline, sizeof(DessolveOutline));
		m_shellModel.m_model.extraBufferArray.back() = GBufferMgr::Instance()->m_outlineBuffer;
		m_shellModel.m_model.extraBufferArray.back().rangeType = GRAPHICS_RANGE_TYPE_UAV_DESC;
		m_shellModel.m_model.extraBufferArray.back().rootParamType = GRAPHICS_PRAMTYPE_TEX;
		m_shellModel.Draw(arg_rasterize, arg_blasVec, m_shellTransform);

		/*オカモトゾーン*/
		m_gardHpBoxTransform.pos = m_transform.pos;
		m_gardHpBoxTransform.pos.y += 33.0f;
		m_gardHpBoxTransform.rotation.y = 45.0f;

		m_hpBoxTransform.pos = m_transform.pos;
		m_hpBoxTransform.pos.y += 30.0f;
		m_hpBoxTransform.rotation.y = 45.0f;

		float shell_base_hp = (m_shellHP / SHELL_HP) * SCALE_MAG;
		float mine_base_hp = (static_cast<float>(m_hp) / HP) * SCALE_MAG;
		m_gardHpBoxTransform.scale.x += (shell_base_hp - m_gardHpBoxTransform.scale.x) / 5.0f;
		if (fabs(mine_base_hp - m_hpBoxTransform.scale.x) > 0)
		{
			m_hpBoxTransform.scale.x += (mine_base_hp - m_hpBoxTransform.scale.x) / 5.0f;
		}

		if (m_isActive && isDrawHpBox)
		{
			if (m_hpBoxDrawTimer > 0)
			{
				if (m_shellHP >= 0.05f)
				{
					m_gardHpBoxModel.Draw(arg_rasterize, arg_blasVec, m_gardHpBoxTransform, 0, false);
				}
				if (m_hp >= 0.05f)
				{
					m_hpBoxModel.Draw(arg_rasterize, arg_blasVec, m_hpBoxTransform, 0, false);
				}
				m_hpBoxDrawTimer--;
			}
			else
			{
				isDrawHpBox = false;
			}
		}
		/*オカモトゾーン*/
	}

}

void MineTsumuri::Damage(std::weak_ptr<Mineral> arg_mineral, int arg_damage)
{

	if (!m_isMineking) {

		m_isAttackedMineral = true;
		if (m_attackedMineral.expired()) {
			m_attackedMineral = arg_mineral;
		}
	}

	/*オカモトゾーン*/
	isDrawHpBox = true;
	m_hpBoxDrawTimer = HP_BOX_DRAW_TIME_MAX;
	/*オカモトゾーン*/

	//殻があったら
	if (m_isShell) {

		if (m_isMineking) {
			m_shellHP = std::clamp(m_shellHP - arg_damage, 0.0f, MINEKING_SHELL_HP);
		}
		else {
			m_shellHP = std::clamp(m_shellHP - arg_damage, 0.0f, SHELL_HP);
		}
		if (m_shellHP <= 0.0f) {

			//殻が壊された瞬間だったら。
			if (m_isShell) {

				ShakeMgr::Instance()->m_shakeAmount = 3.0f;

				m_shellBreakVel = KazMath::Vec3<float>(GetPosZeroY() - arg_mineral.lock()->GetPosZeroY()).GetNormal();
				m_shellBreakVel.y = 1.0f;
				m_shellBreakVel.Normalize();
				m_shellBreakVel *= SHELL_BRWAK_VEL;

				m_shellBreakRightVec = KazMath::Vec3<float>(arg_mineral.lock()->GetPosZeroY() - GetPosZeroY()).GetNormal().Cross({ 0,1,0 });
				m_shellBreakRotation = 0;


				if (m_isMineking) {
					EnemyScore::Instance()->m_score += 250;
				}
				else {
					EnemyScore::Instance()->m_score += 50;
				}

			}
			m_isShell = false;
			m_inShell = false;
		}
	}

	else {
		if (m_isMineking) {
			m_hp = std::clamp(m_hp - arg_damage, 0, MINEKING_HP);
		}
		else {
			m_hp = std::clamp(m_hp - arg_damage, 0, HP);
		}
	}

}

void MineTsumuri::AttackCore(std::weak_ptr<Core> arg_core)
{

	switch (m_attackID)
	{
	case MineTsumuri::ATTACK:
	{
		//移動速度を上げる。
		m_coreAttackMoveSpeed = std::clamp(m_coreAttackMoveSpeed + ADD_CORE_ATTACK_SPEED, 0.0f, MAX_CORE_ATTACK_SPEED);

		//移動するベクトルを求める。
		KazMath::Vec3<float> moveDir = KazMath::Vec3<float>(arg_core.lock()->GetPosZeroY() - m_transform.pos);
		float distance = moveDir.Length();
		moveDir.Normalize();

		//移動速度が距離を超えていたら範囲に収める。
		m_coreAttackMoveSpeed = std::clamp(m_coreAttackMoveSpeed, 0.0f, distance);

		//移動させる。
		m_transform.pos += moveDir * m_coreAttackMoveSpeed;

		//距離が一定以下になったら待機状態へ
		if (KazMath::Vec3<float>(arg_core.lock()->GetPosZeroY() - m_transform.pos).Length() <= arg_core.lock()->GetScale() + m_transform.scale.x) {

			m_attackID = STAY;

			//反動で吹き飛ばす。
			KazMath::Vec3<float> reactionDir = moveDir;
			reactionDir *= -1.0f;
			reactionDir.y = 1.0f;
			reactionDir.Normalize();
			m_coreAttackReactionVec = reactionDir * (m_coreAttackMoveSpeed * 3.0f);

			//コアにダメージを与える。
			if (m_isMineking) {
				arg_core.lock()->Damage(ATTACK_POWER_KING);
			}
			else {
				arg_core.lock()->Damage(ATTACK_POWER);
			}

			ShakeMgr::Instance()->m_shakeAmount = 1.0f;

		}

	}
	break;
	case MineTsumuri::STAY:

		m_coreAttackDelayTimer = std::clamp(m_coreAttackDelayTimer + 1, 0, m_coreAttackDelay);
		if (m_coreAttackDelay <= m_coreAttackDelayTimer) {

			m_attackID = ATTACK;
			m_coreAttackMoveSpeed = 0.0f;
			m_coreAttackDelayTimer = 0;

		}

		//敵に攻撃されていたら、迎撃状態に入る。
		if (m_isAttackedMineral) {

			m_mode = MineralAttack;

		}

		break;
	default:
		break;
	}

}

void MineTsumuri::AttackMineral()
{

	//参照が切れていたら終わり。
	if (m_attackedMineral.expired()) {

		m_mode = CoreAttack;
		m_isAttackMineral = false;
		m_attackedMineral.reset();
		return;
	}

	//攻撃対象のミネラルが一定以上離れていたらコア攻撃状態二遷移。
	float mineralDistance = KazMath::Vec3<float>(GetPosZeroY() - m_attackedMineral.lock()->GetPosZeroY()).Length();
	if (ENEMY_SEARCH_END_RANGE < mineralDistance || (m_attackedMineral.lock()->GetHP() <= 0)) {

		m_mode = CoreAttack;
		m_isAttackMineral = false;
		m_attackedMineral.reset();
		return;

	}

	//この変数が初期枯れてていない場合があるので、ミネラルを攻撃しているときは壁を攻撃していないだろってことなのでこれ。
	m_isAttackWall = false;

	switch (m_attackID)
	{
	case MineTsumuri::ATTACK:
	{
		//移動速度を上げる。
		m_coreAttackMoveSpeed = std::clamp(m_coreAttackMoveSpeed + ADD_CORE_ATTACK_SPEED, 0.0f, MAX_CORE_ATTACK_SPEED);

		//移動するベクトルを求める。
		KazMath::Vec3<float> moveDir = KazMath::Vec3<float>(m_attackedMineral.lock()->GetPosZeroY() - m_transform.pos);
		float distance = moveDir.Length();
		moveDir.Normalize();

		//移動速度が距離を超えていたら範囲に収める。
		m_coreAttackMoveSpeed = std::clamp(m_coreAttackMoveSpeed, 0.0f, distance);

		//移動させる。
		m_transform.pos += moveDir * m_coreAttackMoveSpeed;

		//距離が一定以下になったら待機状態へ
		if (KazMath::Vec3<float>(m_attackedMineral.lock()->GetPosZeroY() - m_transform.pos).Length() <= m_attackedMineral.lock()->GetScale().x + m_transform.scale.x) {

			m_attackID = STAY;
			SoundManager::Instance()->SoundPlayerWave(attack, 0);
			//反動で吹き飛ばす。
			KazMath::Vec3<float> reactionDir = moveDir;
			reactionDir *= -1.0f;
			reactionDir.y = 1.0f;
			reactionDir.Normalize();
			m_coreAttackReactionVec = reactionDir * (m_coreAttackMoveSpeed * 1.5f);

			//コアにダメージを与える。
			if (m_isMineking) {
				m_attackedMineral.lock()->Damage(ATTACK_POWER_KING);
			}
			else {
				m_attackedMineral.lock()->Damage(ATTACK_POWER);
			}

			if (m_attackedMineral.lock()->GetHP() <= 0) {
				m_attackedMineral.reset();
			}

			ShakeMgr::Instance()->m_shakeAmount = 1.0f;

		}

	}
	break;
	case MineTsumuri::STAY:

		m_coreAttackDelayTimer = std::clamp(m_coreAttackDelayTimer + 1, 0, m_coreAttackDelay);
		if (m_coreAttackDelay <= m_coreAttackDelayTimer) {

			m_attackID = ATTACK;
			m_coreAttackMoveSpeed = 0.0f;
			m_coreAttackDelayTimer = 0;

		}

		//敵に攻撃されていたら、迎撃状態に入る。
		if (m_isAttackedMineral) {

			m_mode = MineralAttack;

		}

		break;
	default:
		break;
	}

}

void MineTsumuri::AttackPlayer(std::weak_ptr<Player> arg_player)
{

	//参照が切れていたら終わり。
	if (arg_player.lock()->GetIsStun()) {

		m_mode = CoreAttack;
		m_isAttackPlayer = false;
		return;
	}

	//攻撃対象のミネラルが一定以上離れていたらコア攻撃状態二遷移。
	float mineralDistance = KazMath::Vec3<float>(GetPosZeroY() - arg_player.lock()->GetPosZeroY()).Length();
	if (ENEMY_SEARCH_END_RANGE < mineralDistance) {

		m_mode = CoreAttack;
		m_isAttackPlayer = false;
		return;

	}

	//この変数が初期枯れてていない場合があるので、ミネラルを攻撃しているときは壁を攻撃していないだろってことなのでこれ。
	m_isAttackWall = false;

	switch (m_attackID)
	{
	case MineTsumuri::ATTACK:
	{
		//移動速度を上げる。
		m_coreAttackMoveSpeed = std::clamp(m_coreAttackMoveSpeed + ADD_CORE_ATTACK_SPEED, 0.0f, MAX_CORE_ATTACK_SPEED);

		//移動するベクトルを求める。
		KazMath::Vec3<float> moveDir = KazMath::Vec3<float>(arg_player.lock()->GetPosZeroY() - m_transform.pos);
		float distance = moveDir.Length();
		moveDir.Normalize();

		//移動速度が距離を超えていたら範囲に収める。
		m_coreAttackMoveSpeed = std::clamp(m_coreAttackMoveSpeed, 0.0f, distance);

		//移動させる。
		m_transform.pos += moveDir * m_coreAttackMoveSpeed;

		//距離が一定以下になったら待機状態へ
		distance = KazMath::Vec3<float>(arg_player.lock()->GetPosZeroY() - m_transform.pos).Length();
		if (distance <= arg_player.lock()->GetHitScale() + m_transform.scale.x) {

			m_attackID = STAY;

			//反動で吹き飛ばす。
			KazMath::Vec3<float> reactionDir = moveDir;
			reactionDir *= -1.0f;
			reactionDir.y = 1.0f;
			reactionDir.Normalize();
			m_coreAttackReactionVec = reactionDir * (m_coreAttackMoveSpeed * 1.5f);

			//コアにダメージを与える。
			if (m_isMineking) {
				arg_player.lock()->Damage(ATTACK_POWER_KING);
			}
			else {
				arg_player.lock()->Damage(ATTACK_POWER);
			}


			ShakeMgr::Instance()->m_shakeAmount = 3.0;

		}

	}
	break;
	case MineTsumuri::STAY:

		m_coreAttackDelayTimer = std::clamp(m_coreAttackDelayTimer + 1, 0, m_coreAttackDelay);
		if (m_coreAttackDelay <= m_coreAttackDelayTimer) {

			m_attackID = ATTACK;
			m_coreAttackMoveSpeed = 0.0f;
			m_coreAttackDelayTimer = 0;

		}

		break;
	default:
		break;
	}

}

void MineTsumuri::AttackWall()
{

	switch (m_attackID)
	{
	case MineTsumuri::ATTACK:
	{
		//移動速度を上げる。
		m_coreAttackMoveSpeed = std::clamp(m_coreAttackMoveSpeed + ADD_CORE_ATTACK_SPEED, 0.0f, MAX_CORE_ATTACK_SPEED);

		//移動するベクトルを求める。
		KazMath::Vec3<float> moveDir = KazMath::Vec3<float>(BuildingMgr::Instance()->GetWallPosZeroY(m_wallIndex) - m_transform.pos);
		float distance = moveDir.Length();
		moveDir.Normalize();

		//移動速度が距離を超えていたら範囲に収める。
		m_coreAttackMoveSpeed = std::clamp(m_coreAttackMoveSpeed, 0.0f, distance);

		//移動させる。
		m_transform.pos += moveDir * m_coreAttackMoveSpeed;

		//距離が一定以下になったら待機状態へ
		if (KazMath::Vec3<float>(BuildingMgr::Instance()->GetWallPosZeroY(m_wallIndex) - m_transform.pos).Length() <= 5.0f + m_transform.scale.x) {

			m_attackID = STAY;

			//反動で吹き飛ばす。
			KazMath::Vec3<float> reactionDir = moveDir;
			reactionDir *= -1.0f;
			reactionDir.y = 1.0f;
			reactionDir.Normalize();
			m_coreAttackReactionVec = reactionDir * (m_coreAttackMoveSpeed * 3.0f);

			//コアにダメージを与える。
			if (m_isMineking) {
				BuildingMgr::Instance()->DamageWall(m_wallIndex, ATTACK_POWER_KING);
			}
			else {
				BuildingMgr::Instance()->DamageWall(m_wallIndex, ATTACK_POWER);
			}

			ShakeMgr::Instance()->m_shakeAmount = 1.0f;

		}

	}
	break;
	case MineTsumuri::STAY:

		m_coreAttackDelayTimer = std::clamp(m_coreAttackDelayTimer + 1, 0, m_coreAttackDelay);
		if (m_coreAttackDelay <= m_coreAttackDelayTimer) {

			m_attackID = ATTACK;
			m_coreAttackMoveSpeed = 0.0f;
			m_coreAttackDelayTimer = 0;

		}

		break;
	default:
		break;
	}

	//参照している壁が壊れたら、おわり。
	if (BuildingMgr::Instance()->GetWallHP(m_wallIndex) <= 0 || !BuildingMgr::Instance()->GetIsBuildWall(m_wallIndex)) {

		m_mode = CoreAttack;
		m_isAttackWall = false;

	}

	m_isAttackMineral = false;
	m_coreMoveSpeed = 0.0f;

}

void MineTsumuri::CheckHitPlayer(std::weak_ptr<Player> arg_player)
{

	using namespace KazMath;

	//プレイヤーとの当たり判定を行う。
	if (arg_player.lock()->GetIsDaipanTrigger()) {

		//当たり判定チェック
		bool isHit = Vec3<float>(arg_player.lock()->GetTransform().pos - m_transform.pos).Length() <= arg_player.lock()->GetMineralAffectStrongRange() + 30.0f;
		if (isHit) {
			/*オカモトゾーン*/
			isDrawHpBox = true;
			m_hpBoxDrawTimer = HP_BOX_DRAW_TIME_MAX;
			/*オカモトゾーン*/

			if (m_isShell) {

				if (!m_inShell && !m_isMineking) {

					//殻にこもる。
					m_inShell = true;
					m_inShellTimer = IN_SHELL_TIMER;
					SoundManager::Instance()->SoundPlayerWave(shell_slap, 0);
				}

			}
			else {

				//ベクトルチェック。
				KazMath::Vec3<float> dir = KazMath::Vec3<float>(m_transform.pos - arg_player.lock()->GetTransform().pos);
				dir.y = 0.0f;
				dir.Normalize();

				//HPを減らす。
				if (m_isMineking) {
					m_hp = std::clamp(m_hp - arg_player.lock()->GetDaipanDamage(), 0, MINEKING_HP);
				}
				else {
					m_hp = std::clamp(m_hp - arg_player.lock()->GetDaipanDamage(), 0, HP);
				}

				//攻撃の反動を追加。
				m_attackedReactionVec = dir / 5.0f;
				ShakeMgr::Instance()->m_shakeAmount = 2.0f;
				m_attackedScale = ATTACKED_SCALE;

			}
		}
	}
}

KazMath::Vec3<float> MineTsumuri::SearchRoute()
{

	//ルートの最大到達地点
	const int ROUTE_COUT = static_cast<int>(m_route.size());

	//次のインデックスを計算。
	int nextIndex = m_passedRouteIndex + 1;

	//次のインデックスが存在するか？
	if (ROUTE_COUT <= nextIndex) return {};

	//次のインデックスに到達していたら。
	float nextIndexDistance = KazMath::Vec3<float>(m_route[nextIndex] - m_transform.pos).Length();
	if (nextIndexDistance < ROUTE_CHECK_SCALE) {

		//インデックスを次のものにする。
		m_passedRouteIndex = nextIndex;

		//次のインデックスがあるかを確認。
		nextIndex = m_passedRouteIndex + 1;

		//次のインデックスが存在するか？
		if (ROUTE_COUT <= nextIndex) return {};

	}

	//ベクトルを計算。
	KazMath::Vec3<float> moveDir = m_route[nextIndex] - m_transform.pos;
	moveDir.Normalize();

	return moveDir;

}

void MineTsumuri::KnockBack()
{

	//ノックバックする方向を求める。
	m_knockBackVec = KazMath::Vec3<float>(m_oldTransform.pos - m_transform.pos).GetNormal();
	m_knockBackVec *= KNOCKBACK_SPEED;

}

void MineTsumuri::Move() {

	using namespace KazMath;

	//ノックバック中は動かさない。
	if (m_knockBackVec.Length() <= 0.0f) {

		//ミネクジがコアに向かって行く移動を計算する。
		if (0 < m_coreMoveSpeed) {

			Vec3<float> moveDir = SearchRoute();
			m_transform.pos += moveDir * m_coreMoveSpeed;
			m_coreMoveSpeed -= m_coreMoveSpeed / 15.0f;

		}
		else if (m_coreMoveSpeed < 0.01f) {
			m_coreMoveSpeed = 0.0f;
		}

	}

	//攻撃の反動を与える。
	if (0.01f < m_coreAttackReactionVec.Length()) {

		m_transform.pos += m_coreAttackReactionVec;
		m_coreAttackReactionVec -= m_coreAttackReactionVec / 10.0f;

	}
	else {

		m_coreAttackReactionVec = {};

	}

	//攻撃を受けたときのベクトルを与える。
	if (0.01f < m_attackedReactionVec.Length()) {

		m_transform.pos += m_attackedReactionVec;
		m_attackedReactionVec -= m_attackedReactionVec / 10.0f;

	}
	else {

		m_attackedReactionVec = {};

	}

	//ノックバック
	if (0.01f < m_knockBackVec.Length()) {

		m_transform.pos += m_knockBackVec;
		m_knockBackVec -= m_knockBackVec / 5.0f;

	}
	else {

		m_knockBackVec = {};

	}

	//殻にこもっているときは殻に重力をかける。
	const float UNDER_Y = 5.0f;
	if (m_inShell && m_isShell) {

		if (UNDER_Y < m_shellTransform.pos.y + m_shellPosY) {

			m_shellGravity += GRAVITY;
			m_shellPosY -= m_shellGravity;

		}
		else {

			m_shellGravity = 0.0f;

		}

		//空中に浮いていたら重力を計算する。
		if (UNDER_Y < m_transform.pos.y) {

			m_gravity += GRAVITY;
			m_transform.pos.y -= m_gravity;

		}
		else {

			m_gravity = 0.0f;
			m_transform.pos.y = UNDER_Y;

		}

	}

	//空中に浮いていたら重力を計算する。
	if (UNDER_Y < m_transform.pos.y) {

		m_gravity += GRAVITY;
		m_transform.pos.y -= m_gravity;

	}
	else {

		m_gravity = 0.0f;
		m_transform.pos.y = UNDER_Y;

	}

	//ダメージを受けたときの拡縮をもとに戻す。
	if (0 < m_attackedScale) {

		m_attackedScale -= m_attackedScale / 5.0f;

	}
	else {

		m_attackedScale = 0.0f;

	}

}

void MineTsumuri::CheckHit(std::weak_ptr<Player> arg_player) {


	//プレイヤーとの当たり判定
	CheckHitPlayer(arg_player);

	//壁との当たり判定を行う。
	MeshCollision::CheckHitResult result = BuildingMgr::Instance()->CheckHitWall(m_transform.pos, m_forwardVec, 40.0f, m_wallIndex);

	//当たっていたら
	if (result.m_isHit) {

		m_isAttackWall = true;
		m_mode = WallAttack;

	}

}

void MineTsumuri::Rotation(std::weak_ptr<Core> arg_core, std::weak_ptr<Player> arg_player) {

	if (m_inShell) return;

	//ノックバック中は回転させない。
	if (0 < m_knockBackVec.Length()) return;

	//傾ける。
	KazMath::Vec3<float> nowPos = m_transform.pos;
	KazMath::Vec3<float> viewTargetPos;
	nowPos.y = 0.0f;
	//攻撃中はターゲットの方向を見る。
	if (m_isAttackCore) {

		nowPos = arg_core.lock()->GetPos();
		nowPos.y = 0.0f;
		viewTargetPos = m_transform.pos;
		viewTargetPos.y = 0.0f;

	}
	else if (m_isAttackWall) {
		nowPos = BuildingMgr::Instance()->GetWallPosZeroY(m_wallIndex);
		nowPos.y = 0.0f;
		viewTargetPos = m_transform.pos;
		viewTargetPos.y = 0.0f;
	}
	else if (m_isAttackPlayer) {
		nowPos = arg_player.lock()->GetPosZeroY();
		nowPos.y = 0.0f;
		viewTargetPos = m_transform.pos;
		viewTargetPos.y = 0.0f;
	}
	else if ((m_isAttackMineral || m_isAttackedMineral) && !m_attackedMineral.expired()) {

		nowPos = m_attackedMineral.lock()->GetPos();
		nowPos.y = 0.0f;
		viewTargetPos = m_transform.pos;
		viewTargetPos.y = 0.0f;

	}
	else {

		//動いた方向に傾ける。
		viewTargetPos = m_oldTransform.pos;
		viewTargetPos.y = 0.0f;

	}
	if (0 < KazMath::Vec3<float>(nowPos - viewTargetPos).Length()) {

		m_forwardVec = KazMath::Vec3<float>(nowPos - viewTargetPos).GetNormal();

		//デフォルトの正面ベクトルからの回転量を求める。
		float angle = std::acosf(KazMath::Vec3<float>(0, 0, 1).Dot(m_forwardVec));

		//クォータニオンを求める。
		KazMath::Vec3<float> cross = KazMath::Vec3<float>(0, 0, 1).Cross(m_forwardVec);
		if (cross.Length() <= 0) {
			cross = { 0,1,0 };
		}
		DirectX::XMVECTOR rotateQ = DirectX::XMQuaternionRotationAxis({ cross.x, cross.y, cross.z }, angle);

		//回転を適応
		m_transform.quaternion = rotateQ;

	}

}

void MineTsumuri::UpdateShell() {

	//殻が壊れていたら。
	if (!m_isShell) {

		if (0.01f < m_shellBreakVel.Length()) {

			m_shellTransform.pos += m_shellBreakVel;

			if (m_isMineking) {
				m_shellBreakVel -= m_shellBreakVel / 2.0f;
			}
			else {
				m_shellBreakVel -= m_shellBreakVel / 10.0f;
			}

		}
		else {

			m_shellBreakVel = {};

		}

		//空中に浮いていたら重力を計算する。
		const float UNDER_Y = -1000.0f;
		if (UNDER_Y < m_shellTransform.pos.y) {

			if (m_isMineking) {
				m_shellGravity += GRAVITY * 2.0f;
			}
			else {
				m_shellGravity += GRAVITY;
			}
			m_shellTransform.pos.y -= m_shellGravity;

		}
		else {

			m_shellGravity = 0.0f;
			m_shellTransform.pos.y = UNDER_Y;

		}

		//回転をかける。
		if (m_isMineking) {
			//m_shellBreakRotation += 0.01f;
		}
		else {
			m_shellBreakRotation += 0.02f;
		}
		m_shellTransform.quaternion = DirectX::XMQuaternionMultiply(m_shellTransform.quaternion, DirectX::XMQuaternionRotationAxis(m_shellBreakRightVec.ConvertXMVECTOR(), m_shellBreakRotation));


		return;
	}

	if (m_inShell) {

		m_shellTransform.pos = m_transform.pos;

		//殻の位置を計算。
		const float SHELL_POS_Z = 2.0f;
		m_shellTransform.pos -= m_forwardVec * SHELL_POS_Z;
		m_shellTransform.pos += KazMath::Vec3<float>(0, 1, 0) * m_shellPosY;

		return;
	}

	//殻の位置と回転を決める。
	m_shellTransform.pos = m_transform.pos;
	m_shellTransform.quaternion = m_transform.quaternion;

	//殻をY軸に90度回転させておく。
	m_shellTransform.quaternion = DirectX::XMQuaternionMultiply(m_shellTransform.quaternion, DirectX::XMQuaternionRotationAxis({ 0,1,0 }, -DirectX::XM_PIDIV2));

	//殻の位置を計算。
	const float SHELL_POS_Z = 2.0f;
	m_shellTransform.pos -= m_forwardVec * SHELL_POS_Z;
	m_shellPosY += (SHELL_POS_Y - m_shellPosY) / 5.0f;
	m_shellTransform.pos += KazMath::Vec3<float>(0, 1, 0) * m_shellPosY;

}

KazMath::Vec3<float> MineTsumuri::TransformVec3(KazMath::Vec3<float> arg_value, DirectX::XMVECTOR arg_quaternion)
{
	auto val = DirectX::XMVectorSet(arg_value.x, arg_value.y, arg_value.z, 1.0f);
	val = DirectX::XMVector3Rotate(val, arg_quaternion);
	return KazMath::Vec3<float>(val.m128_f32[0], val.m128_f32[1], val.m128_f32[2]);
}
