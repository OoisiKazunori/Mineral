#include "Player.h"
#include "../KazLibrary/Input/KeyBoradInputManager.h"
#include "../KazLibrary/Easing/easing.h"
#include "../Game/Effect/ShakeMgr.h"
#include "../KazLibrary/Input/ControllerInputManager.h"
#include "../Game/Collision/StageCollision.h"
#include "../Game/Effect/ChromaticAberration.h"
#include <Imgui/imgui.h>
#include "../Game/TitleFlag.h"
#include "../Game/ResultFlag.h"
#include"../Game/Tutorial.h"

Player::Player()
{

	m_model.LoadOutline("Resource/Player/", "Hand.gltf", true);

	m_attackModel.LoadOutline("Resource/Player/", "HandAttack.gltf", true);

	m_daipanModel.LoadNoLighting("Resource/Player/HandCircle/", "HandCircle.gltf");

	m_daipanStrongModel.LoadNoLighting("Resource/Player/HandCircle/", "HandCircleStrong.gltf");


	//UI
	m_hpFrameUI.Load("Resource/UI/Player/UI_HP1.png");
	m_hpUI.Load("Resource/UI/Player/UI_HP2.png");
	m_hpBackGroundUI.Load("Resource/UI/Player/UI_HP3.png");
	m_sousaUI.Load("Resource/UI/Player/UI_Info1.png");
	m_dadanUI[0].Load("Resource/UI/Player/UI_HP4.png");
	m_dadanUI[1].Load("Resource/UI/Player/UI_HP5.png");
	m_dadanUI[2].Load("Resource/UI/Player/UI_HP6.png");
	m_dadanBackGroundUI.Load("Resource/UI/Player/UI_HP7.png");

	m_sousaUI.m_transform.pos = { 1280.0f / 2.0f, 720.0f / 2.0f };
	m_sousaUI.m_transform.scale = { 1280.0f, 720.0f };

	//SE
	slap_se = SoundManager::Instance()->SoundLoadWave("Resource/Sound/slap.wav");
	slap_se.volume = 0.1f;
	slap_strong_se = SoundManager::Instance()->SoundLoadWave("Resource/Sound/slap_strong.wav");
	slap_strong_se.volume = 0.2f;
	down = SoundManager::Instance()->SoundLoadWave("Resource/Sound/Player_Down.wav");
	down.volume = 0.05f;
	damage = SoundManager::Instance()->SoundLoadWave("Resource/Sound/Player_Damage.wav");
	damage.volume = 0.05f;
}

void Player::Init()
{

	m_isInputDaipan = false;
	m_isDaipanStrong = false;
	m_isStun = false;
	m_isBreakUp = false;
	m_isOldBreakUp = false;
	m_isWaveHand = false;
	m_daipanStatus = NONE;
	m_daipanStartPosY = 0;
	m_stanGravity = 0;
	m_daipanTimer = 0;
	m_daipanReturnTimer = 0;
	m_daipanStrongTimer = 0.0f;
	m_daipanShakePos = {};
	m_transform.pos.x = 113.0f;
	m_transform.pos.z = 117.0f;
	m_transform.pos.y = DEFAULT_Y;
	m_daipanPos = m_transform.pos;
	m_mineralCenterPos = m_transform.pos;
	m_mineralCenterPos.y = 0.0f;
	m_oldTransform = m_transform;
	m_forwardQ = DirectX::XMQuaternionIdentity();
	m_daipanReturnTimer = 0.0f;
	m_hp = HP;
	m_damageShake = 0;
	m_damageShakePos = {};
	m_hpRecoveryTimer = 0;
	m_damageChangeDadanUITimer = 0;
	m_autoRecoveryStartTimer = 0.0f;
	m_autoRecoveryDelayTimer = 0.0f;

	m_hpUI.m_transform.scale = UI_HPBAR_SCALE;
	m_hpUI.m_transform.pos = UI_HPBAR_POS;


}

void Player::Update()
{

	//回転を元に戻す。
	if (!m_isWaveHand) {
		m_transform.quaternion = DirectX::XMQuaternionSlerp(m_transform.quaternion, m_forwardQ, 0.2f);
	}

	//座標を保存しておく。
	m_oldTransform = m_transform;

	//スタン中は動かさない。
	if (!m_isStun && (!TitleFlag::Instance()->m_isTitle && !ResultFlag::Instance()->m_isResult)) {

		//プレイヤーを動かす。
		const float MOVE_SPEED = 1.5f;
		float moveSpeed = MOVE_SPEED;
		//台パンチャージ中は若干移動速度を上げる。
		if (m_daipanStatus == CHARGE) {
			const float DAIPAN_SPEED = 0.5f;
			moveSpeed += DAIPAN_SPEED;
		}

		//動く方向
		KazMath::Vec3<float> moveVec = {};

		moveVec.z += (KeyBoradInputManager::Instance()->InputState(DIK_W));
		moveVec.z -= (KeyBoradInputManager::Instance()->InputState(DIK_S));
		moveVec.x += (KeyBoradInputManager::Instance()->InputState(DIK_D));
		moveVec.x -= (KeyBoradInputManager::Instance()->InputState(DIK_A));

		//コントローラーも対応。
		float inputStickX = ControllerInputManager::Instance()->GetJoyStickLXNum() / 32767.0f;
		float inputStickY = ControllerInputManager::Instance()->GetJoyStickLYNum() / 32767.0f;
		const float STICK_DEADLINE = 0.7f;
		if (STICK_DEADLINE < KazMath::Vec2<float>(inputStickX, inputStickY).Length()) {

			moveVec.x += inputStickX;
			moveVec.z += inputStickY;

		}

		//入力があったら。
		if (0 < moveVec.Length()) {

			//範囲を調整。
			moveVec.x = std::clamp(moveVec.x, -1.0f, 1.0f);
			moveVec.z = std::clamp(moveVec.z, -1.0f, 1.0f);

			//正規化して方向を決める。
			moveVec.Normalize();

			//回転をかけて入力を直感的にする。
			moveVec = TransformVec3(moveVec, DirectX::XMQuaternionRotationAxis({ 0,1,0 }, DirectX::XM_PIDIV4 * 1.18f));

			//動かす。
			m_transform.pos += moveVec * moveSpeed;

		}

		//隊列を操作する。
		m_mineralCenterPos.z += (KeyBoradInputManager::Instance()->InputState(DIK_UP)) * MINERAL_MOVE_SPEED;
		m_mineralCenterPos.z -= (KeyBoradInputManager::Instance()->InputState(DIK_DOWN)) * MINERAL_MOVE_SPEED;
		m_mineralCenterPos.x += (KeyBoradInputManager::Instance()->InputState(DIK_RIGHT)) * MINERAL_MOVE_SPEED;
		m_mineralCenterPos.x -= (KeyBoradInputManager::Instance()->InputState(DIK_LEFT)) * MINERAL_MOVE_SPEED;
		inputStickX = ControllerInputManager::Instance()->GetJoyStickRXNum() / 32767.0f;
		inputStickY = ControllerInputManager::Instance()->GetJoyStickRYNum() / 32767.0f;
		if (STICK_DEADLINE <= fabs(inputStickX)) {
			m_mineralCenterPos.x += inputStickX * MINERAL_MOVE_SPEED;
		}
		if (STICK_DEADLINE <= fabs(inputStickY)) {
			m_mineralCenterPos.z += inputStickY * MINERAL_MOVE_SPEED;
		}

		//隊列の操作量に上限を設ける。
		const float MINERAL_CENTER_MOVE_RANGE = 20.0f;
		if (MINERAL_CENTER_MOVE_RANGE < m_mineralCenterPos.Length()) {
			m_mineralCenterPos = m_mineralCenterPos.GetNormal() * MINERAL_CENTER_MOVE_RANGE;
		}

		//台パンが入力されているか。
		m_isOldInputDaipan = m_isInputDaipan;
		m_isInputDaipan = KeyBoradInputManager::Instance()->InputState(DIK_SPACE) || ControllerInputManager::Instance()->InputState(XINPUT_GAMEPAD_A);

		//解散状態かを更新
		m_isOldBreakUp = m_isBreakUp;
		m_isBreakUp = KeyBoradInputManager::Instance()->InputState(DIK_T) || ControllerInputManager::Instance()->InputState(XINPUT_GAMEPAD_B);

	}

	//台パン中はカメラを追尾させない仕様を切るためのやつ。確認を取ったら正式に実装する。
	m_daipanPos += (m_transform.pos - m_daipanPos) / 3.0f;

	//ステータスによって台パンの状態を変える。
	switch (m_daipanStatus)
	{
	case Player::NONE:

		//入力があったらステータスを変える。
		if (m_isInputDaipan) {

			m_daipanStatus = Player::CHARGE;

		}

		//プレイヤーがスタン中だったら
		if (m_hp <= 0) {

			m_stanGravity += STAN_GRAVITY;
			m_transform.pos.y = std::clamp(m_transform.pos.y - m_stanGravity, 2.0f, DEFAULT_Y);

		}
		else {

			//座標をデフォルトの位置に補間する。
			m_transform.pos.y += (DEFAULT_Y - m_transform.pos.y) / 3.0f;

			m_stanGravity = 0;

		}

		//なにもない時のみ台パン座標を保存する。
		//m_daipanPos += (m_transform.pos - m_daipanPos) / 3.0f;

		//何もしないときは初期化する。
		m_daipanTimer = 0.0f;
		m_daipanReturnTimer = 0.0f;
		m_daipanStartPosY = 0.0f;
		m_daipanStrongTimer = 0.0f;
		m_daipanShakePos = {};

		break;
	case Player::CHARGE:

	{

		//高さをチャージ中の位置に動かしていく。
		m_transform.pos.y += (DAIPAN_Y - m_transform.pos.y) / 10.0f;

		//チャージされた高さの割合。
		float heightRate = (m_transform.pos.y - DEFAULT_Y) / (DAIPAN_Y - DEFAULT_Y);

		//チャージされた分だけ上に傾ける。
		m_daipanXAngle = -heightRate * DirectX::XM_PIDIV4;
		m_transform.quaternion = DirectX::XMQuaternionMultiply(m_transform.quaternion, DirectX::XMQuaternionRotationAxis(TransformVec3({ 1,0,0 }, m_transform.quaternion).ConvertXMVECTOR(), m_daipanXAngle));

		//リリースされた瞬間だったら
		if (m_isOldInputDaipan && !m_isInputDaipan) {

			m_daipanStatus = Player::DAIPAN_NOW;
			m_daipanStartPosY = m_transform.pos.y;

		}

		//台パンを強にするかのタイマーを更新。
		m_daipanStrongTimer = std::clamp(m_daipanStrongTimer + 1.0f, 0.0f, DAIPAN_STRONG_TIMER);
		m_isDaipanStrong = DAIPAN_STRONG_TIMER <= m_daipanStrongTimer;

		//台パン中はシェイクさせる。
		if (m_isDaipanStrong) {

			//シェイク量を計算
			m_daipanShakePos = { KazMath::Rand(-DAIPAN_SHAKE_POS, DAIPAN_SHAKE_POS),KazMath::Rand(-DAIPAN_SHAKE_POS, DAIPAN_SHAKE_POS),KazMath::Rand(-DAIPAN_SHAKE_POS, DAIPAN_SHAKE_POS) };
			m_transform.quaternion = m_forwardQ;

		}

	}

	break;
	case Player::DAIPAN_NOW:
	{

		//台パンで振り下ろすやつのタイマーを設定する。
		float daipanTimer = DAIPAN_NOW_TIMER;
		if (m_isDaipanStrong) {
			daipanTimer = DAIPAN_NOW_STRONG_TIMER;
		}

		//キャンセルのタイマーを更新。
		m_daipanTimer = std::clamp(m_daipanTimer + 1.0f, 0.0f, daipanTimer);

		//Y軸をもとに戻す。
		float easingAmount = EasingMaker(In, Cubic, m_daipanTimer / daipanTimer);

		//Y軸を設定。
		m_transform.pos.y = m_daipanStartPosY - m_daipanStartPosY * easingAmount;

		//台パンのX軸回転ももとに戻す。
		float nowXAngle = m_daipanXAngle - (m_daipanXAngle * easingAmount);
		m_transform.quaternion = DirectX::XMQuaternionMultiply(m_transform.quaternion, DirectX::XMQuaternionRotationAxis(TransformVec3({ 1,0,0 }, m_transform.quaternion).ConvertXMVECTOR(), nowXAngle));


		//タイマーが上限に達したら通常状態に戻す。
		if (daipanTimer <= m_daipanTimer) {
			if (!m_isDaipanStrong)
			{
				SoundManager::Instance()->SoundPlayerWave(slap_se, 0);
			}
			else
			{
				SoundManager::Instance()->SoundPlayerWave(slap_strong_se, 0);
			}
			m_daipanStatus = Player::RETURN;

			//強大パンだったらそれ用のシェイクにする。
			ShakeMgr::Instance()->m_shakeAmount = m_isDaipanStrong ? DAIPAN_STRONG_SHAKE : DAIPAN_SHAKE;

		}
	}

	break;

	case Player::RETURN:
	{

		m_daipanReturnTimer = std::clamp(m_daipanReturnTimer + 1.0f, 0.0f, DAIPAN_RETURN_TIMER);
		if (DAIPAN_RETURN_TIMER <= m_daipanReturnTimer) {
			m_daipanReturnTimer = 0.0f;
			m_daipanStatus = Player::NONE;
		}

		//座標をデフォルトの位置に補間する。
		m_transform.pos.y += (DEFAULT_Y - m_transform.pos.y) / 3.0f;

	}
	break;

	default:
		break;
	}

	//本来あるべきミネラルの中心点を計算する。
	m_mineralCenterBasePos = m_transform.pos;
	if (!m_isDaipanStrong) {

		m_mineralCenterBasePos -= m_forwardVec * MINERAL_CENTER_DISTANCE;

	}
	m_mineralCenterBasePos.y = 0.0f;

	//動いた方向に傾ける。
	KazMath::Vec3<float> nowPos = m_transform.pos;
	nowPos.y = 0.0f;
	KazMath::Vec3<float> oldPos = m_oldTransform.pos;
	oldPos.y = 0.0f;
	if (0.5f < KazMath::Vec3<float>(nowPos - oldPos).Length()) {

		m_forwardVec = KazMath::Vec3<float>(nowPos - oldPos).GetNormal();

		//デフォルトの正面ベクトルからの回転量を求める。
		float angle = std::acosf(KazMath::Vec3<float>(0, 0, 1).Dot(m_forwardVec));

		//クォータニオンを求める。
		KazMath::Vec3<float> cross = KazMath::Vec3<float>(0, 0, 1).Cross(m_forwardVec);
		if (cross.Length() <= 0) {
			cross = { 0,1,0 };
		}
		DirectX::XMVECTOR rotateQ = DirectX::XMQuaternionRotationAxis({ cross.x, cross.y, cross.z }, angle);

		//回転を適応
		m_forwardQ = rotateQ;

	}

	//当たり判定を計算。
	const float RAY_LENGTH = 10.0f;
	MeshCollision::CheckHitResult rayResult = StageCollision::Instance()->m_stageCollision.CheckHitRay(m_transform.pos, m_forwardVec);
	if (rayResult.m_isHit && 0.0f < rayResult.m_distance && rayResult.m_distance <= RAY_LENGTH) {

		//押し戻し。
		m_transform.pos += rayResult.m_normal * (RAY_LENGTH - rayResult.m_distance);

	}
	//右方向
	KazMath::Vec3<float> rightVec = TransformVec3({ 1,0,0 }, m_transform.quaternion);
	rayResult = StageCollision::Instance()->m_stageCollision.CheckHitRay(m_transform.pos, rightVec);
	if (rayResult.m_isHit && 0.0f < rayResult.m_distance && rayResult.m_distance <= RAY_LENGTH) {

		//押し戻し。
		m_transform.pos += rayResult.m_normal * (RAY_LENGTH - rayResult.m_distance);

	}
	//左方向
	rayResult = StageCollision::Instance()->m_stageCollision.CheckHitRay(m_transform.pos, -rightVec);
	if (rayResult.m_isHit && 0.0f < rayResult.m_distance && rayResult.m_distance <= RAY_LENGTH) {

		//押し戻し。
		m_transform.pos += rayResult.m_normal * (RAY_LENGTH - rayResult.m_distance);

	}

	//ダメージを受けたときのシェイクを更新。
	m_damageShakePos = { KazMath::Rand(-m_damageShake, m_damageShake), KazMath::Rand(-m_damageShake, m_damageShake), KazMath::Rand(-m_damageShake, m_damageShake) };
	m_damageShake -= m_damageShake / 10.0f;
	if (m_damageShake < 0.01f) {
		m_damageShake = 0.0f;
	}

	m_drawTransform = m_transform;
	m_drawTransform.quaternion = m_transform.quaternion;

	//シェイクをかける。
	m_drawTransform.pos += m_daipanShakePos;
	m_drawTransform.pos += m_damageShakePos;


	float baseFacter = 1.0f - (m_hp / HP);
	ChromaticAberration::Instance()->m_facter += (baseFacter - ChromaticAberration::Instance()->m_facter) / 20.0f;



	//HPが0になったら
	if (m_hp <= 0 && !m_isStun) {

		m_isStun = true;
		SoundManager::Instance()->SoundPlayerWave(down, 0);

		ShakeMgr::Instance()->m_shakeAmount = 5.0f;
		m_damageShake = DAMAGE_SHAKE * 1.5f;

	}

	//スタン中は回復するまでのタイマーを更新する。
	if (m_isStun) {

		m_hpRecoveryTimer = std::clamp(m_hpRecoveryTimer + 1.0f, 0.0f, HP_RECOVERY_TIMER);

		if (HP_RECOVERY_TIMER <= m_hpRecoveryTimer) {

			m_hp = HP;
			m_hpRecoveryTimer = 0;
			m_isStun = false;

		}

	}

	//一定時間ダメージを受けていなかったら自動回復。
	if (m_hp < HP) {

		m_autoRecoveryStartTimer = std::clamp(m_autoRecoveryStartTimer + 1.0f, 0.0f, AUTO_RECOVERY_START_TIMER);

		if (AUTO_RECOVERY_START_TIMER <= m_autoRecoveryStartTimer) {

			//自動回復開始。
			m_autoRecoveryDelayTimer = std::clamp(m_autoRecoveryDelayTimer + 1.0f, 0.0f, AUTO_RECOVERY_DELAY_TIMER);

			if (AUTO_RECOVERY_DELAY_TIMER <= m_autoRecoveryDelayTimer) {

				//自動回復する。
				m_hp = std::clamp(m_hp + 1.0f, 0.0f, HP);
				m_autoRecoveryDelayTimer = 0.0f;

			}

		}

	}

	//ダメージを受けてから一定フレームは立ち絵を変える。
	m_damageChangeDadanUITimer = std::clamp(m_damageChangeDadanUITimer - 1, 0, DAMAGE_CHANGE_DADAN_UI_TIMER);

	float hpRate = m_hp / HP;
	//UIの座標を設定。
	m_hpFrameUI.m_transform.scale = UI_BASE_SCALE;
	m_hpFrameUI.m_transform.pos = UI_BASE_POS;
	KazMath::Color baseColor;
	if (m_isStun) {
		m_hpUI.m_transform.scale = { UI_HPBAR_SCALE.x * (m_hpRecoveryTimer / HP_RECOVERY_TIMER), UI_HPBAR_SCALE.y };
		KazMath::Vec2<float> deadUIPos = UI_HPBAR_POS - KazMath::Vec2<float>((UI_HPBAR_SCALE.x / 2.0f / HP) * (HP - m_hp), 0.0f);
		m_hpUI.m_transform.pos = KazMath::Vec2<float>(deadUIPos.x + (UI_HPBAR_POS.x - deadUIPos.x) * (m_hpRecoveryTimer / HP_RECOVERY_TIMER), deadUIPos.y);
		baseColor = KazMath::Color(178, 54, 87, 255);
	}
	else {
		float baseScale = UI_HPBAR_SCALE.x * hpRate;;
		m_hpUI.m_transform.scale.x += (baseScale - m_hpUI.m_transform.scale.x) / 5.0f;;
		m_hpUI.m_transform.scale.y = UI_HPBAR_SCALE.y;
		KazMath::Vec2<float> basePos = UI_HPBAR_POS - KazMath::Vec2<float>((UI_HPBAR_SCALE.x / 2.0f / HP) * (HP - m_hp), 0.0f);
		m_hpUI.m_transform.pos += (basePos - m_hpUI.m_transform.pos) / 5.0f;
		baseColor = KazMath::Color(150, 214, 79, 255);
	}
	m_hpUI.m_color.color.x += static_cast<int>((baseColor.color.x - m_hpUI.m_color.color.x) / 5.0f);
	m_hpUI.m_color.color.y += static_cast<int>((baseColor.color.y - m_hpUI.m_color.color.y) / 5.0f);
	m_hpUI.m_color.color.z += static_cast<int>((baseColor.color.z - m_hpUI.m_color.color.z) / 5.0f);
	m_hpBackGroundUI.m_transform.scale = UI_BASE_SCALE;
	m_hpBackGroundUI.m_transform.pos = UI_BASE_POS;
	for (auto& index : m_dadanUI) {
		index.m_transform.scale = UI_BASE_SCALE;
		index.m_transform.pos = UI_BASE_POS + KazMath::Vec2<float>(KazMath::Rand(-m_damageShake * 2.0f, m_damageShake * 2.0f), KazMath::Rand(-m_damageShake * 2.0f, m_damageShake * 2.0f));
	}
	m_dadanBackGroundUI.m_transform.scale = UI_BASE_SCALE;
	m_dadanBackGroundUI.m_transform.pos = UI_BASE_POS;




	DessolveOutline outline;
	outline.m_outline = KazMath::Vec4<float>(0.1f, 0, 0, 1);
	m_model.m_model.extraBufferArray[4].bufferWrapper->TransData(&outline, sizeof(DessolveOutline));

	m_model.m_model.extraBufferArray.back() = GBufferMgr::Instance()->m_outlineBuffer;
	m_model.m_model.extraBufferArray.back().rangeType = GRAPHICS_RANGE_TYPE_UAV_DESC;
	m_model.m_model.extraBufferArray.back().rootParamType = GRAPHICS_PRAMTYPE_TEX;

	outline.m_outline = KazMath::Vec4<float>(0.1f, 0, 0, 1);
	m_attackModel.m_model.extraBufferArray[4].bufferWrapper->TransData(&outline, sizeof(DessolveOutline));

	m_attackModel.m_model.extraBufferArray.back() = GBufferMgr::Instance()->m_outlineBuffer;
	m_attackModel.m_model.extraBufferArray.back().rangeType = GRAPHICS_RANGE_TYPE_UAV_DESC;
	m_attackModel.m_model.extraBufferArray.back().rootParamType = GRAPHICS_PRAMTYPE_TEX;

	//チュートリアルのとき
	if (Tutorial::Instance()->is_tutorial && Tutorial::Instance()->tutorial_num == 1)
	{
		Tutorial::Instance()->move_amount += (m_oldTransform.pos - m_transform.pos).Length();

		//一定量動いたらチュートリアルを進める
		if (Tutorial::Instance()->move_amount >= 400.0f)
		{
			Tutorial::Instance()->is_next = true;
		}
	}

	//隊列解除のトリガーだったら
	if (m_isBreakUp && !m_isOldBreakUp && !m_isWaveHand) {

		m_isWaveHand = true;
		m_waveHandTimer = 0;

	}
	if (m_isWaveHand) {

		const float WAVE_HAND_TIMER = 0.4f;
		m_waveHandTimer += WAVE_HAND_TIMER;

		//手を振る。
		const float WAVE_HANDLE = DirectX::XM_PI / 4.0f;
		m_drawTransform.quaternion = DirectX::XMQuaternionMultiply(m_drawTransform.quaternion, DirectX::XMQuaternionRotationAxis({ 0,1,0,0 }, std::sinf(m_waveHandTimer) * WAVE_HANDLE));

		if (DirectX::XM_2PI * 1.5f < m_waveHandTimer) {

			m_isWaveHand = false;

		}

	}

}

void Player::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{

	//プレイヤー本体を描画
	m_drawTransform.scale = { 9.0f, 9.0f, 9.0f };
	if (m_isDaipanStrong && m_daipanStatus != NONE) {
		m_attackModel.Draw(arg_rasterize, arg_blasVec, m_drawTransform, 10);
	}
	else {
		m_model.Draw(arg_rasterize, arg_blasVec, m_drawTransform, 10);
	}

	//プレイヤーの台パンの範囲を描画
	KazMath::Transform3D transform;
	transform.scale = { 0,0,0 };
	if (m_isInputDaipan && !m_isDaipanStrong) {

		transform.pos = m_transform.pos;
		transform.pos.y = 1.0f;
		transform.scale = { MINERAL_AFFECT_RANGE,MINERAL_AFFECT_RANGE,MINERAL_AFFECT_RANGE };

	}
	m_daipanModel.Draw(arg_rasterize, arg_blasVec, transform);


	std::vector<D3D12_RESOURCE_BARRIER> barrier;

	barrier.emplace_back(CD3DX12_RESOURCE_BARRIER::UAV(GBufferMgr::Instance()->m_outlineBuffer.bufferWrapper->GetBuffer().Get()));;

	DirectX12CmdList::Instance()->cmdList->ResourceBarrier(static_cast<UINT>(barrier.size()), barrier.data());

	//どのダダンを描画するか。
	int dadanIndex = 0;
	if (m_isStun) {
		dadanIndex = 2;
	}
	else if (0 < m_damageChangeDadanUITimer) {
		dadanIndex = 1;
	}

	if (0 < ResultFlag::Instance()->m_uiDeleteTime) {


		m_hpFrameUI.m_color.color.a += static_cast<int>((0.0f - m_hpFrameUI.m_color.color.a) / 3.0f);
		m_hpUI.m_color.color.a += static_cast<int>((0.0f - m_hpUI.m_color.color.a) / 3.0f);
		m_hpBackGroundUI.m_color.color.a += static_cast<int>((0.0f - m_hpBackGroundUI.m_color.color.a) / 3.0f);
		m_sousaUI.m_color.color.a += static_cast<int>((0.0f - m_sousaUI.m_color.color.a) / 3.0f);
		m_dadanUI[0].m_color.color.a += static_cast<int>((0.0f - m_dadanUI[0].m_color.color.a) / 3.0f);
		m_dadanUI[1].m_color.color.a += static_cast<int>((0.0f - m_dadanUI[1].m_color.color.a) / 3.0f);
		m_dadanUI[2].m_color.color.a += static_cast<int>((0.0f - m_dadanUI[2].m_color.color.a) / 3.0f);
		m_dadanBackGroundUI.m_color.color.a += static_cast<int>((0.0f - m_dadanBackGroundUI.m_color.color.a) / 3.0f);

		if (m_hpFrameUI.m_color.color.a <= 50.0f) {

			m_hpFrameUI.m_color.color.a = 0;
			m_hpUI.m_color.color.a = 0;
			m_hpBackGroundUI.m_color.color.a = 0;
			m_sousaUI.m_color.color.a = 0;
			m_dadanUI[0].m_color.color.a = 0;
			m_dadanUI[1].m_color.color.a = 0;
			m_dadanUI[2].m_color.color.a = 0;
			m_dadanBackGroundUI.m_color.color.a = 0;

		}
		else {

			//UIを描画。
			m_dadanBackGroundUI.Draw(arg_rasterize);
			m_dadanUI[dadanIndex].Draw(arg_rasterize);
			m_sousaUI.Draw(arg_rasterize);
			m_hpBackGroundUI.Draw(arg_rasterize);
			m_hpUI.Draw(arg_rasterize);
			m_hpFrameUI.Draw(arg_rasterize);
		}



	}
	else if ((!TitleFlag::Instance()->m_isTitle && !ResultFlag::Instance()->m_isResult)) {

		//UIを描画。
		m_dadanBackGroundUI.Draw(arg_rasterize);
		m_dadanUI[dadanIndex].Draw(arg_rasterize);
		m_sousaUI.Draw(arg_rasterize);
		m_hpBackGroundUI.Draw(arg_rasterize);
		m_hpUI.Draw(arg_rasterize);
		m_hpFrameUI.Draw(arg_rasterize);

		m_hpFrameUI.m_color.color.a += static_cast<int>((255.0f - m_hpFrameUI.m_color.color.a) / 15.0f);
		m_hpUI.m_color.color.a += static_cast<int>((255.0f - m_hpUI.m_color.color.a) / 15.0f);
		m_hpBackGroundUI.m_color.color.a += static_cast<int>((255.0f - m_hpBackGroundUI.m_color.color.a) / 15.0f);
		m_sousaUI.m_color.color.a += static_cast<int>((255.0f - m_sousaUI.m_color.color.a) / 15.0f);
		m_dadanUI[0].m_color.color.a += static_cast<int>((255.0f - m_dadanUI[0].m_color.color.a) / 15.0f);
		m_dadanUI[1].m_color.color.a += static_cast<int>((255.0f - m_dadanUI[1].m_color.color.a) / 15.0f);
		m_dadanUI[2].m_color.color.a += static_cast<int>((255.0f - m_dadanUI[2].m_color.color.a) / 15.0f);
		m_dadanBackGroundUI.m_color.color.a += static_cast<int>((255.0f - m_dadanBackGroundUI.m_color.color.a) / 15.0f);

	}
	else {

		m_hpFrameUI.m_color.color.a = 0;
		m_hpUI.m_color.color.a = 0;
		m_hpBackGroundUI.m_color.color.a = 0;
		m_sousaUI.m_color.color.a = 0;
		m_dadanUI[0].m_color.color.a = 0;
		m_dadanUI[1].m_color.color.a = 0;
		m_dadanUI[2].m_color.color.a = 0;
		m_dadanBackGroundUI.m_color.color.a = 0;

	}



	ImGui::Begin("UI");

	ImGui::Text("POS_X : %f", m_transform.pos.x);
	ImGui::Text("POS_Z : %f", m_transform.pos.z);

	ImGui::End();


}

KazMath::Vec3<float> Player::TransformVec3(KazMath::Vec3<float> arg_value, DirectX::XMVECTOR arg_quaternion)
{
	auto val = DirectX::XMVectorSet(arg_value.x, arg_value.y, arg_value.z, 1.0f);
	val = DirectX::XMVector3Rotate(val, arg_quaternion);
	return KazMath::Vec3<float>(val.m128_f32[0], val.m128_f32[1], val.m128_f32[2]);
}

void Player::Damage(int arg_damage) {

	SoundManager::Instance()->SoundPlayerWave(damage, 0);
	m_hp = std::clamp(m_hp - arg_damage, 0.0f, HP);
	m_damageShake = DAMAGE_SHAKE;
	m_damageChangeDadanUITimer = DAMAGE_CHANGE_DADAN_UI_TIMER;

	//自動回復までの時間を初期化。
	m_autoRecoveryStartTimer = 0.0f;
	m_autoRecoveryDelayTimer = 0.0f;

}