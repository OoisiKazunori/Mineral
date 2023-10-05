#include "GameScene.h"
#include"Input/KeyBoradInputManager.h"
#include"Input/ControllerInputManager.h"
#include"Imgui/MyImgui.h"
#include"Loader/ObjResourceMgr.h"
#include"Helper/ResourceFilePass.h"
#include"../Game/Debug/ParameterMgr.h"
#include"Math/KazMath.h"
#include"../Game/Player.h"
#include"../Game/Effect/ShakeMgr.h"
#include"../Game/Mineral/MineralMgr.h"
#include"../Game/Rock/RockMgr.h"
#include"../Game/Mineral/MineralTarget.h"
#include"../Game/Core/Core.h"
#include"../Game/Enemy/EnemyMgr.h"
#include"../Game/DestructibleObject/DestructibleObjectMgr.h"
#include"../Game/BuildingMaterial/BuildingMaterialMgr.h"
#include"../Game/Building/BuildingMgr.h"
#include"../Game/UI/NumberFont.h"
#include"../Game/Collision/StageCollision.h"
#include"../Game/Enemy/EnemyRoute.h"
#include"../Game/WallAndTreeGeneratePos.h"
#include"../Game/Wave/WaveMgr.h"
#include"../Game/TitleFlag.h"
#include"../Game/ResultFlag.h"
#include"../Game/Tutorial.h"
#include"../KazLibrary/Easing/easing.h"
#include"../Game/Transition.h"
#include"../Game/EnemyScore.h"
#include"../Game/PointLightMgr.h"
#include"../Game/Effect/StopMgr.h"

GameScene::GameScene(DrawingByRasterize& arg_rasterize)
{
	skipTurtorialFlag = false;
	PreInit();
	firstGenerateFlag = true;

	m_stageTransform.scale = { 5.0f, 5.0f, 5.0f };
	m_stageTransform.pos = { 0.0f, -5.0f, 0.0f };
	m_stageTransform.rotation = { 0.0f, 197.5f, 0.0f };
	StageCollision::Instance()->Setting("Resource/Stage/Collision/", "StageCollision.gltf", m_stageTransform);
	m_cameraEyeDistance = 1200.0f;


	const float CAMERA_EYE_X = -400.0f;
	const float CAMERA_EYE_Y = 600.0f;
	const float CAMERA_EYE_Z = -300.0f;
	m_cameraEyeDir = { CAMERA_EYE_X, CAMERA_EYE_Y, CAMERA_EYE_Z };
	m_cameraEyeDir = { -0.691f, 0.503f, -0.519f };
	m_cameraEyeDir.Normalize();

	m_player = std::make_unique<Player>();

	m_mineralTarget = std::make_shared<MineralTarget>();
	m_enemyMgr = std::make_shared<EnemyMgr>();
	m_goldCore = std::make_shared<Core>();
	m_goldCore->SetPos({ 169.0f,10.0f,105.0f });
	MineralMgr::Instance()->Setting();
	RockMgr::Instance()->Setting();
	DestructibleObjectMgr::Instance()->Setting();
	BuildingMaterialMgr::Instance()->Setting();
	BuildingMgr::Instance()->Setting();
	EnemyRoute::Instance()->Setting();
	WallAndTreeGeneratePos::Instance()->Setting();
	WaveMgr::Instance()->Setting(m_goldCore);
	Transition::Instance()->Setting();


	m_ground.LoadOutline("Resource/Stage/", "Stage_Ground.gltf");
	m_fence.Load("Resource/Stage/", "Stage_Fence.gltf");
	m_tree.Load("Resource/Stage/", "Stage_Tree.gltf");
	m_rock.Load("Resource/Stage/", "Stage_Rock.gltf");
	m_line.Load("Resource/Stage/", "Line.gltf");
	m_puddle.Load("Resource/Puddle/", "puddle.gltf");

	//タイトルロゴをロード
	m_titleLogoUI.Load("Resource/Title/TitleLogo.png");
	m_titleBackGroundUI.Load("Resource/Title/Title_1.png");
	m_titleStartUI.Load("Resource/Title/Title_2.png");
	m_titleQuitUI.Load("Resource/Title/Title_3.png");

	//タイトルロゴの場所を設定。
	m_titleLogoUI.m_transform.pos = { 1280.0f / 2.0f, 720.0f / 2.0f };
	m_titleLogoUI.m_transform.scale = { 1280.0f / 2.0f, 720.0f / 2.0f };
	m_titleBackGroundUI.m_transform.pos = { 1280.0f / 2.0f, 720.0f / 2.0f };
	m_titleBackGroundUI.m_transform.scale = { 1280.0f, 720.0f };
	m_titleStartUI.m_transform.pos = { 640.0f, 440.0f };
	m_titleStartUI.m_transform.scale = UI_MAX_START_SCALE;
	m_titleQuitUI.m_transform.pos = { 640.0f, 545.0f };
	m_titleQuitUI.m_transform.scale = { UI_DEF_QUIT_SCALE };
	m_selectTitleNum = 0;
	m_selectTitleUISine = 0.0f;

	//リザルト関連
	m_resultBackGroundUI.Load("Resource/Result/Result_UI4.png");
	m_resultTitleUI.Load("Resource/Result/Result_UI2.png");
	m_resultRetryUI.Load("Resource/Result/Result_UI3.png");

	//リザルトのUIの数字をロード
	for (auto& index : m_resultDayScoreUI) {

		index.Load("Resource/Result/Result_UI3.png");

	}
	for (auto& index : m_resultMineralScoreUI) {

		index.Load("Resource/Result/Result_UI3.png");

	}
	for (auto& index : m_reesultEnemyScoreUI) {

		index.Load("Resource/Result/Result_UI3.png");

	}
	for (auto& index : m_totalScoreUI) {

		index.Load("Resource/Result/Result_UI3.png");

	}

	//サイズを調整。
	m_resultBackGroundUI.m_transform.pos = { 1280.0f / 2.0f, 720.0f / 2.0f };
	m_resultBackGroundUI.m_transform.scale = { 1280.0f, 720.0f };
	m_resultTitleUI.m_transform.pos = { 640.0f, 610.0f };
	m_resultTitleUI.m_transform.scale = UI_DEF_TITLE_SCALE;
	m_resultRetryUI.m_transform.pos = { 640.0f, 535.0f };
	m_resultRetryUI.m_transform.scale = { UI_MAX_RETRY_SCALE };
	m_isResultTransition = false;

	EnemyScore::Instance()->m_score = 0;

	NumberFont::Instance()->Load();
	Tutorial::Instance()->setting();

	//Tutorial::Instance()->is_tutorial = false;

	for (auto& obj : m_slapEffect)
	{
		obj = std::make_unique<SlapSmokeEffect>();
	}

}

GameScene::~GameScene()
{
}

void GameScene::Init()
{
	skipTurtorialFlag = false;

	m_player->Init();
	MineralMgr::Instance()->Init();
	//MineralMgr::Instance()->DebugGenerate();
	m_goldCore->Init();

	m_enemyMgr->Init();
	//m_enemyMgr->DebugGenerate();

	RockMgr::Instance()->Init();
	//m_rockMgr->DebugGenerate();

	DestructibleObjectMgr::Instance()->Init();
	//m_destructibleObject->DebugGenerate();

	WaveMgr::Instance()->Init(m_enemyMgr);
	WaveMgr::Instance()->GameStart();

	BuildingMaterialMgr::Instance()->Init();
	BuildingMgr::Instance()->Init();
	BuildingMgr::Instance()->Generate();

	//太陽の方向を昼に設定
	GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir = KazMath::Vec3<float>(0.0f, -0.894f, 0.4472f);

	TitleFlag::Instance()->Init();
	ResultFlag::Instance()->Init();
	m_titleLogoDeleteTimer = 0.0f;

	Transition::Instance()->Init();

	m_titleStartUI.m_color.color.a = 255;
	m_titleQuitUI.m_color.color.a = 255;
	m_titleBackGroundUI.m_color.color.a = 255;

	//リザルト関連
	m_selectResultNum = 0;
	m_isResultToTitle = false;
	m_resultDayScore = 0;

	m_isTitleNightChangeTimer = 0;
	m_isTitleNight = false;

	EnemyScore::Instance()->m_score = 0;

	m_resultStayTimer = 0;

	m_slapEffectIndex = 0;
	m_pauseFlag = false;
}

void GameScene::PreInit()
{
}

void GameScene::Finalize()
{
}

void GameScene::Input()
{
	if (KeyBoradInputManager::Instance()->InputTrigger(DIK_ESCAPE))
	{
		m_pauseFlag = !m_pauseFlag;
	}
	if (m_pauseFlag)
	{
		StopMgr::Instance()->HitStopStart({ 1000000,0.0f });
	}
	else
	{
		StopMgr::Instance()->HitStopStart({ 1000000,1.0f });
	}
}

void GameScene::Update()
{
	using namespace KazMath;

	if (StopMgr::Instance()->GetGameSpeed() != 1.0f)
	{
		return;
	}

	//ウェーブを更新。
	WaveMgr::Instance()->Update(m_enemyMgr);

	//プレイヤーを更新。
	m_player->Update();

	//ミネラルを更新。
	MineralMgr::Instance()->Update(m_player, m_mineralTarget, m_enemyMgr);

	//岩を更新。
	RockMgr::Instance()->Update(m_player);

	//敵を更新。
	m_enemyMgr->Update(m_goldCore, m_player);
	//デバック用の処理
	SlapSmokeEffect::DebugImGui();
	if (m_player->GetIsDaipanTrigger())
	{
		float radius = 0.0f;
		if (m_player->GetIsStrongDaipan())
		{
			radius = m_player->GetMineralAffectStrongRange();
		}
		else
		{
			radius = m_player->GetMineralAffectRange();
		}

		++m_slapEffectIndex;
		if (m_slapEffect.size() <= m_slapEffectIndex)
		{
			m_slapEffectIndex = 0;
		}
		m_slapEffect[m_slapEffectIndex]->Init(
			m_player->GetPosZeroY() + KazMath::Vec3<float>(0.0f, 5.0f, 0.0f),
			radius,
			m_player->GetIsStrongDaipan()
		);
	}

	//煙エフェクト
	for (auto& obj : m_slapEffect)
	{
		obj->Update();
	}

	//ミネラルのターゲットを更新。
	m_mineralTarget->Update(m_player->GetPosZeroY(), m_player->GetMineralAffectRange(), m_enemyMgr);

	//黄金のコアを更新
	m_goldCore->Update();

	//破壊可能オブジェクトを更新。
	DestructibleObjectMgr::Instance()->Update(m_player);

	//建築物の素材を更新。
	BuildingMaterialMgr::Instance()->Update();

	//建築物を更新。
	BuildingMgr::Instance()->Update(m_player);

	//カメラの位置を設定
	m_cameraEyeDir.Normalize();
	const float CAMERA_TARGET_X = 0.0f;
	const float CAMERA_TARGET_Y = 5.0f;
	const float CAMERA_TARGET_Z = 10.0f;
	CameraMgr::Instance()->Camera(m_player->GetTransformDefaultY() + m_cameraEyeDir * m_cameraEyeDistance, m_player->GetTransformDefaultY() + Vec3<float>(CAMERA_TARGET_X, CAMERA_TARGET_Y, CAMERA_TARGET_Z) + ShakeMgr::Instance()->GetShakeVec3(), Vec3<float>(0, 1, 0));

	//シェイクを更新。
	ShakeMgr::Instance()->Update();

	if (WaveMgr::Instance()->GetIsFinishAllWave())
	{
		MineralMgr::Instance()->Init();
	}

	//ウェーブが終わったらリザルトへ。
	if (!ResultFlag::Instance()->m_isResult) {
		ResultFlag::Instance()->m_isReadyResult = (WaveMgr::Instance()->GetIsFinishAllWave()) || m_goldCore->IsDead();
	}
	if ((!ResultFlag::Instance()->m_isResult && WaveMgr::Instance()->GetIsFinishAllWave()) || m_goldCore->IsDead()) {

		++ResultFlag::Instance()->m_uiDeleteTime;
		if (ResultFlag::Instance()->UI_DELETE_TIME <= ResultFlag::Instance()->m_uiDeleteTime) {

			//遷移が始まっていなかったら遷移させる。
			if (!Transition::Instance()->GetIsActive() && !m_isResultTransition) {
				Transition::Instance()->Activate();
				m_isResultTransition = true;
			}
			//終わっていたら
			else if (Transition::Instance()->GetIsFinish()) {
				ResultFlag::Instance()->m_isResult = true;
				ResultFlag::Instance()->m_isDraw = true;
				Transition::Instance()->Init();

				m_resultStayTimer = 0;

				//スコアを算出。
				m_resultDayScore = 0;
				for (int index = 0; index < WaveMgr::Instance()->GetNowWaveIndex() + 1; ++index) {

					if (m_resultDayScore <= 0) {
						m_resultDayScore = 100;
					}
					else {

						m_resultDayScore += m_resultDayScore;

					}

				}
			}

		}

		//敵を初期化
		m_enemyMgr->Init();

	}

	//建築物と敵のノックバック判定
	BuildingMgr::Instance()->CheckNockBack(m_enemyMgr);

	//タイトルの更新処理
	UpdateTitle();

	//リザルトの更新処理
	UpdateResult();

	Transition::Instance()->Update();

	Tutorial::Instance()->Update();

	m_fireFlyParticle.Update(WaveMgr::Instance()->GetIsNight());

}

void GameScene::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{

	//UIを表示
	PIXBeginEvent(DirectX12CmdList::Instance()->cmdList.Get(), 0, "UI");


	WaveMgr::Instance()->Draw(arg_rasterize, arg_blasVec);


	PIXEndEvent(DirectX12CmdList::Instance()->cmdList.Get());
	m_player->Draw(arg_rasterize, arg_blasVec);
	MineralMgr::Instance()->Draw(arg_rasterize, arg_blasVec);
	RockMgr::Instance()->Draw(arg_rasterize, arg_blasVec);
	m_mineralTarget->Draw(arg_rasterize, arg_blasVec, m_enemyMgr);
	m_enemyMgr->Draw(arg_rasterize, arg_blasVec);
	m_goldCore->Draw(arg_rasterize, arg_blasVec);
	DestructibleObjectMgr::Instance()->Draw(arg_rasterize, arg_blasVec);
	BuildingMaterialMgr::Instance()->Draw(arg_rasterize, arg_blasVec);
	BuildingMgr::Instance()->Draw(arg_rasterize, arg_blasVec);

	static UINT reflection = 1;
	m_puddle.m_model.extraBufferArray[1].bufferWrapper->TransData(&reflection, sizeof(UINT));

	DessolveOutline outline;
	outline.m_outline = KazMath::Vec4<float>(0.0f, 0, 0, 1);
	m_ground.m_model.extraBufferArray[4].bufferWrapper->TransData(&outline, sizeof(DessolveOutline));
	m_ground.m_model.extraBufferArray.back() = GBufferMgr::Instance()->m_outlineBuffer;
	m_ground.m_model.extraBufferArray.back().rangeType = GRAPHICS_RANGE_TYPE_UAV_DESC;
	m_ground.m_model.extraBufferArray.back().rootParamType = GRAPHICS_PRAMTYPE_TEX;

	m_ground.Draw(arg_rasterize, arg_blasVec, m_stageTransform);
	m_fence.Draw(arg_rasterize, arg_blasVec, m_stageTransform);
	m_tree.Draw(arg_rasterize, arg_blasVec, m_stageTransform);
	m_rock.Draw(arg_rasterize, arg_blasVec, m_stageTransform);
	m_line.Draw(arg_rasterize, arg_blasVec, m_stageTransform);

	m_fireFlyParticle.Draw(arg_rasterize);

	static KazMath::Transform3D transform;
	static int a = 0;
	if (a == 0) {
		transform.pos = { -360.0f, 2.5f, -207.0f };
		transform.scale = { 30.0f, 30.0f, 30.0f };
		++a;
	}

	//m_puddle.Draw(arg_rasterize, arg_blasVec, transform);


	//煙エフェクト
	for (auto& obj : m_slapEffect)
	{
		obj->Draw(arg_rasterize, arg_blasVec);
	}
	//ImGui::Begin("UI");

	//ImGui::DragFloat("POS_X", &transform.pos.x, 1.0f);
	//ImGui::DragFloat("POS_Y", &transform.pos.y, 1.0f);
	//ImGui::DragFloat("POS_Z", &transform.pos.z, 1.0f);
	//ImGui::DragFloat("SCALE_X", &transform.scale.x, 0.01f);
	//ImGui::DragFloat("SCALE_Y", &transform.scale.y, 0.01f);
	//ImGui::DragFloat("SCALE_Z", &transform.scale.z, 0.01f);
	//ImGui::DragFloat("ROTATE_X", &transform.rotation.x, 0.1f);
	//ImGui::DragFloat("ROTATE_Y", &transform.rotation.y, 0.1f);
	//ImGui::DragFloat("ROTATE_Z", &transform.rotation.z, 0.1f);

	//ImGui::Text(" ");


	//////KazMath::Vec2<float> baseDayPos = { 1280.0f / 2.0f, 200.0f };
	//////KazMath::Vec2<float> baseMineralPos = { 1280.0f / 2.0f, 300.0f };
	//////KazMath::Vec2<float> baseEnemyScorePos = { 1280.0f / 2.0f, 400.0f };
	//////KazMath::Vec2<float> baseTotalScorePos = { 1280.0f / 2.0f, 500.0f };

	//////ImGui::DragFloat("EyeDistance", &m_cameraEyeDistance, 1.0f);
	////ImGui::DragFloat("TITLE_UI_X", &baseEnemyScorePos.x, 1.0f);
	////ImGui::DragFloat("TITLE_UI_Y", &baseEnemyScorePos.y, 1.0f);
	////ImGui::DragFloat("RETRY_UI_X", &baseTotalScorePos.x, 1.0f);
	////ImGui::DragFloat("RETRY_UI_Y", &baseTotalScorePos.y, 1.0f);
	////ImGui::DragFloat("FONT_SIZE", &fontScale, 0.1f);
	////ImGui::DragFloat("GYOKAN", &gyokan, 0.1f);
	//////ImGui::SliderFloat("UI_Z", &GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir.z, 0.0f, 1.0f);  

	//ImGui::End();

	//タイトルのUIをロード
	if (TitleFlag::Instance()->m_isTitle) {

		//Startを選択していたら。
		if (m_selectTitleNum == 0) {
			m_titleStartUI.m_transform.scale += ((UI_MAX_START_SCALE + sinf(m_selectTitleUISine) * SELECT_TITLE_SINE_SCALE) - m_titleStartUI.m_transform.scale) / 3.0f;
			m_titleQuitUI.m_transform.scale += (UI_DEF_QUIT_SCALE - m_titleQuitUI.m_transform.scale) / 3.0f;
		}
		else {
			m_titleStartUI.m_transform.scale += (UI_DEF_START_SCALE - m_titleStartUI.m_transform.scale) / 3.0f;
			m_titleQuitUI.m_transform.scale += ((UI_MAX_QUIT_SCALE + sinf(m_selectTitleUISine) * SELECT_TITLE_SINE_SCALE) - m_titleQuitUI.m_transform.scale) / 3.0f;
		}


	}
	if (TitleFlag::Instance()->m_isDrawTitle)
	{
		m_titleBackGroundUI.Draw(arg_rasterize);
		m_titleStartUI.Draw(arg_rasterize);
		m_titleQuitUI.Draw(arg_rasterize);
	}

	//リザルト画面を描画
	if (ResultFlag::Instance()->m_isResult) {

		//Startを選択していたら。
		if (m_selectResultNum == 0) {
			m_resultRetryUI.m_transform.scale += ((UI_MAX_RETRY_SCALE + sinf(m_selectTitleUISine) * SELECT_TITLE_SINE_SCALE) - m_resultRetryUI.m_transform.scale) / 3.0f;
			m_resultTitleUI.m_transform.scale += (UI_DEF_TITLE_SCALE - m_resultTitleUI.m_transform.scale) / 3.0f;
		}
		else {
			m_resultRetryUI.m_transform.scale += (UI_DEF_RETRY_SCALE - m_resultRetryUI.m_transform.scale) / 3.0f;
			m_resultTitleUI.m_transform.scale += ((UI_MAX_TITLE_SCALE + sinf(m_selectTitleUISine) * SELECT_TITLE_SINE_SCALE) - m_resultTitleUI.m_transform.scale) / 3.0f;
		}


	}
	if (ResultFlag::Instance()->m_isDraw) {
		for (auto& index : m_resultDayScoreUI) index.Draw(arg_rasterize);
		for (auto& index : m_resultMineralScoreUI) index.Draw(arg_rasterize);
		for (auto& index : m_reesultEnemyScoreUI) index.Draw(arg_rasterize);
		for (auto& index : m_totalScoreUI) index.Draw(arg_rasterize);
		m_resultBackGroundUI.Draw(arg_rasterize);
		m_resultTitleUI.Draw(arg_rasterize);
		m_resultRetryUI.Draw(arg_rasterize);
	}

	//チュートリアルを描画。
	Tutorial::Instance()->Draw(arg_rasterize, arg_blasVec);

	//遷移を描画。
	Transition::Instance()->Draw(arg_rasterize, arg_blasVec);

}

int GameScene::SceneChange()
{
	return SCENE_NONE;
}

void GameScene::UpdateTitle()
{

	//タイトル画面だったら。
	if (TitleFlag::Instance()->m_isTitle) {


		if (KeyBoradInputManager::Instance()->InputState(DIK_W) ||
			KeyBoradInputManager::Instance()->InputState(DIK_UP) ||
			ControllerInputManager::Instance()->InputStickState(ControllerStickSide::LEFT_STICK, ControllerSide::UP_SIDE, 10000)) {

			m_selectTitleNum = std::clamp(m_selectTitleNum - 1, 0, 1);

		}

		if (KeyBoradInputManager::Instance()->InputState(DIK_S) ||
			KeyBoradInputManager::Instance()->InputState(DIK_DOWN) ||
			ControllerInputManager::Instance()->InputStickState(ControllerStickSide::LEFT_STICK, ControllerSide::DOWN_SIDE, 10000)) {

			m_selectTitleNum = std::clamp(m_selectTitleNum + 1, 0, 1);

		}

		//決定キーが入力されたら
		if (KeyBoradInputManager::Instance()->InputState(DIK_SPACE) ||
			KeyBoradInputManager::Instance()->InputState(DIK_RETURN) ||
			ControllerInputManager::Instance()->InputTrigger(XINPUT_GAMEPAD_A)) {

			//Startを選択していたら。
			if (m_selectTitleNum == 0) {

				TitleFlag::Instance()->m_isStart = true;

			}
			//Quitを選択していたら
			else if (m_selectTitleNum == 1) {

				TitleFlag::Instance()->m_isQuit = true;

			}

		}


		m_selectTitleUISine += ADD_SELECT_TITLE_SINE;

		//タイトルが消えていて、遷移も終わっていたらタイトルを終える。
		if (!TitleFlag::Instance()->m_isDrawTitle && !Transition::Instance()->GetIsActive()) {

			TitleFlag::Instance()->m_isTitle = false;
			Transition::Instance()->Init();

		}

		++m_isTitleNightChangeTimer;
		if (600 < m_isTitleNightChangeTimer) {

			m_isTitleNightChangeTimer = 0;
			m_isTitleNight = !m_isTitleNight;

		}

		//ディレクショナルライトの方向を変えて昼夜を表現
		if (!m_isTitleNight) {
			GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir += (KazMath::Vec3<float>(0.0f, -0.894f, 0.4472f) - GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir) / 30.0f;
		}
		else if (m_isTitleNight) {
			GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir += (KazMath::Vec3<float>(0.0f, -0.4472f, 0.894f) - GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir) / 30.0f;
		}
		GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir.Normalize();

		PointLightMgr::Instance()->Update(m_isTitleNight);

	}

	//タイトルロゴを消すまでのタイマー
	if (TitleFlag::Instance()->m_isStart && TitleFlag::Instance()->m_isDrawTitle) {

		m_titleLogoDeleteTimer = std::clamp(m_titleLogoDeleteTimer + 1.0f, 0.0f, TITLELOGO_DELETE_TIMER);

		//イージングをかけてタイマーを減らす。
		float easingAmount = EasingMaker(In, Sine, m_titleLogoDeleteTimer / TITLELOGO_DELETE_TIMER);

		//アルファを減らす。
		m_titleStartUI.m_color.color.a = static_cast<int>((1.0f - easingAmount) * 255);
		m_titleQuitUI.m_color.color.a = static_cast<int>((1.0f - easingAmount) * 255);
		m_titleBackGroundUI.m_color.color.a = static_cast<int>((1.0f - easingAmount) * 255);

		if (TITLELOGO_DELETE_TIMER <= m_titleLogoDeleteTimer) {

			m_titleLogoDeleteTimer = 0.0f;
			TitleFlag::Instance()->m_isDrawTitle = false;
			Transition::Instance()->Activate();

		}

	}
	else if (TitleFlag::Instance()->m_isTitle) {

		m_titleStartUI.m_color.color.a += static_cast<int>((255.0f - m_titleStartUI.m_color.color.a) / 10.0f);
		m_titleQuitUI.m_color.color.a += static_cast<int>((255.0f - m_titleQuitUI.m_color.color.a) / 10.0f);
		m_titleBackGroundUI.m_color.color.a += static_cast<int>((255.0f - m_titleBackGroundUI.m_color.color.a) / 10.0f);

	}
	else {

		m_titleStartUI.m_color.color.a = 0;
		m_titleQuitUI.m_color.color.a = 0;
		m_titleBackGroundUI.m_color.color.a = 0;

	}

}

void GameScene::UpdateResult()
{


	//リザルト画面だったら。
	if (ResultFlag::Instance()->m_isResult) {

		++m_resultStayTimer;
		if (RESULT_STAY_TIMER <= m_resultStayTimer) {

			if (KeyBoradInputManager::Instance()->InputState(DIK_W) ||
				KeyBoradInputManager::Instance()->InputState(DIK_UP) ||
				ControllerInputManager::Instance()->InputStickState(ControllerStickSide::LEFT_STICK, ControllerSide::UP_SIDE, 10000)) {

				m_selectResultNum = std::clamp(m_selectResultNum - 1, 0, 1);

			}

			if (KeyBoradInputManager::Instance()->InputState(DIK_S) ||
				KeyBoradInputManager::Instance()->InputState(DIK_DOWN) ||
				ControllerInputManager::Instance()->InputStickState(ControllerStickSide::LEFT_STICK, ControllerSide::DOWN_SIDE, 10000)) {

				m_selectResultNum = std::clamp(m_selectResultNum + 1, 0, 1);

			}

			//決定キーが入力されたら
			if (KeyBoradInputManager::Instance()->InputState(DIK_SPACE) ||
				KeyBoradInputManager::Instance()->InputState(DIK_RETURN) ||
				ControllerInputManager::Instance()->InputTrigger(XINPUT_GAMEPAD_A)) {

				//Startを選択していたら。
				if (m_selectResultNum == 0) {

					//リトライ
					m_isResultToGame = true;
					m_titleLogoDeleteTimer = 0;
					m_isResultTransition = false;

				}
				//Quitを選択していたら
				else if (m_selectResultNum == 1) {

					m_isResultToTitle = true;
					m_titleLogoDeleteTimer = 0;
					m_selectTitleNum = 0;
					m_isResultTransition = false;
					//Init();

				}

			}

		}


		m_selectTitleUISine += ADD_SELECT_TITLE_SINE;

	}

	//タイトルか再挑戦が選ばれていて、遷移が終わっていたら。
	if ((m_isResultToTitle) && Transition::Instance()->GetIsBlackOut()) {

		m_player->Init();
		MineralMgr::Instance()->Init();
		//MineralMgr::Instance()->DebugGenerate();
		m_goldCore->Init();

		m_enemyMgr->Init();
		//m_enemyMgr->DebugGenerate();

		RockMgr::Instance()->Init();
		//m_rockMgr->DebugGenerate();

		DestructibleObjectMgr::Instance()->Init();
		//m_destructibleObject->DebugGenerate();

		WaveMgr::Instance()->Init(m_enemyMgr);
		WaveMgr::Instance()->GameStart();

		BuildingMaterialMgr::Instance()->Init();
		BuildingMgr::Instance()->Init();
		BuildingMgr::Instance()->Generate();

		//太陽の方向を昼に設定
		GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir = KazMath::Vec3<float>(0.0f, -0.894f, 0.4472f);

	}
	if ((m_isResultToTitle) && Transition::Instance()->GetIsFinish()) {

		ResultFlag::Instance()->Init();
		TitleFlag::Instance()->Init();

		Transition::Instance()->Init();
		m_isResultToTitle = false;

		EnemyScore::Instance()->m_score = 0;

	}

	//タイトルか再挑戦が選ばれていて、遷移が終わっていたら。
	if ((m_isResultToGame) && Transition::Instance()->GetIsBlackOut()) {

		m_player->Init();
		MineralMgr::Instance()->Init();
		//MineralMgr::Instance()->DebugGenerate();
		m_goldCore->Init();

		m_enemyMgr->Init();
		//m_enemyMgr->DebugGenerate();

		RockMgr::Instance()->Init();
		//m_rockMgr->DebugGenerate();

		DestructibleObjectMgr::Instance()->Init();
		//m_destructibleObject->DebugGenerate();

		WaveMgr::Instance()->Init(m_enemyMgr);
		WaveMgr::Instance()->GameStart();

		BuildingMaterialMgr::Instance()->Init();
		BuildingMgr::Instance()->Init();
		BuildingMgr::Instance()->Generate();

		//太陽の方向を昼に設定
		GBufferMgr::Instance()->m_lightConstData.m_dirLight.m_dir = KazMath::Vec3<float>(0.0f, -0.894f, 0.4472f);

	}
	if ((m_isResultToGame) && Transition::Instance()->GetIsFinish()) {


		ResultFlag::Instance()->Init();
		Transition::Instance()->Init();
		m_isResultToGame = false;

		EnemyScore::Instance()->m_score = 0;

	}

	//タイトルロゴを消すまでのタイマー
	if (ResultFlag::Instance()->m_isResult && ResultFlag::Instance()->m_isDraw && (m_isResultToTitle || m_isResultToGame)) {

		m_titleLogoDeleteTimer = std::clamp(m_titleLogoDeleteTimer + 1.0f, 0.0f, TITLELOGO_DELETE_TIMER);

		//イージングをかけてタイマーを減らす。
		float easingAmount = EasingMaker(In, Sine, m_titleLogoDeleteTimer / TITLELOGO_DELETE_TIMER);

		//アルファを減らす。
		m_resultTitleUI.m_color.color.a = static_cast<int>((1.0f - easingAmount) * 255);
		m_resultRetryUI.m_color.color.a = static_cast<int>((1.0f - easingAmount) * 255);
		m_resultBackGroundUI.m_color.color.a = static_cast<int>((1.0f - easingAmount) * 255);
		for (auto& index : m_resultDayScoreUI) {
			index.m_color.color.a = static_cast<int>((1.0f - easingAmount) * 255);
		}
		for (auto& index : m_resultMineralScoreUI) {
			index.m_color.color.a = static_cast<int>((1.0f - easingAmount) * 255);
		}
		for (auto& index : m_reesultEnemyScoreUI) {
			index.m_color.color.a = static_cast<int>((1.0f - easingAmount) * 255);
		}
		for (auto& index : m_totalScoreUI) {
			index.m_color.color.a = static_cast<int>((1.0f - easingAmount) * 255);
		}

		if (TITLELOGO_DELETE_TIMER <= m_titleLogoDeleteTimer) {

			m_titleLogoDeleteTimer = 0.0f;
			ResultFlag::Instance()->m_isDraw = false;
			Transition::Instance()->Activate();

		}

	}
	else if (ResultFlag::Instance()->m_isResult) {

		m_resultTitleUI.m_color.color.a += static_cast<int>((255.0f - m_resultTitleUI.m_color.color.a) / 10.0f);
		m_resultRetryUI.m_color.color.a += static_cast<int>((255.0f - m_resultRetryUI.m_color.color.a) / 10.0f);
		m_resultBackGroundUI.m_color.color.a += static_cast<int>((255.0f - m_resultBackGroundUI.m_color.color.a) / 10.0f);
		for (auto& index : m_resultDayScoreUI) {
			index.m_color.color.a += static_cast<int>((255.0f - index.m_color.color.a) / 10.0f);
		}
		for (auto& index : m_resultMineralScoreUI) {
			index.m_color.color.a += static_cast<int>((255.0f - index.m_color.color.a) / 10.0f);
		}
		for (auto& index : m_reesultEnemyScoreUI) {
			index.m_color.color.a += static_cast<int>((255.0f - index.m_color.color.a) / 10.0f);
		}
		for (auto& index : m_totalScoreUI) {
			index.m_color.color.a += static_cast<int>((255.0f - index.m_color.color.a) / 10.0f);
		}


		//日数スコアを更新。
		const float FONT_SIZE = fontScale;
		for (int index = 0; index < 4; ++index) {

			m_resultDayScoreUI[index].m_texture = NumberFont::Instance()->m_font[GetDigits(m_resultDayScore, 3 - index, 3 - index)];
			m_resultDayScoreUI[index].m_transform.pos = baseDayPos;
			m_resultDayScoreUI[index].m_transform.pos.x += index * gyokan;

			m_resultDayScoreUI[index].m_transform.scale = { FONT_SIZE ,FONT_SIZE };

		}
		const int MINERAL_SCORE = std::clamp(MineralMgr::Instance()->GetMineralScore(), 0, 9999);
		for (int index = 0; index < 4; ++index) {

			m_resultMineralScoreUI[index].m_texture = NumberFont::Instance()->m_font[GetDigits(MINERAL_SCORE, 3 - index, 3 - index)];
			m_resultMineralScoreUI[index].m_transform.pos = baseMineralPos;
			m_resultMineralScoreUI[index].m_transform.pos.x += index * gyokan;

			m_resultMineralScoreUI[index].m_transform.scale = { FONT_SIZE ,FONT_SIZE };

		}
		for (int index = 0; index < 4; ++index) {

			int score = EnemyScore::Instance()->m_score;
			int fontNum = std::clamp(GetDigits(score, 3 - index, 3 - index), 0, 9999);
			m_reesultEnemyScoreUI[index].m_texture = NumberFont::Instance()->m_font[fontNum];
			//m_reesultEnemyScoreUI[index].m_texture = NumberFont::Instance()->m_font[GetDigits(3456, 3 - index, 3 - index)];
			m_reesultEnemyScoreUI[index].m_transform.pos = baseEnemyScorePos;
			m_reesultEnemyScoreUI[index].m_transform.pos.x += index * gyokan;

			m_reesultEnemyScoreUI[index].m_transform.scale = { FONT_SIZE ,FONT_SIZE };

		}
		const int TOTAL_SCORE = std::clamp(m_resultDayScore + MINERAL_SCORE + EnemyScore::Instance()->m_score, 0, 99999);
		for (int index = 0; index < 5; ++index) {

			m_totalScoreUI[index].m_texture = NumberFont::Instance()->m_font[GetDigits(TOTAL_SCORE, 4 - index, 4 - index)];
			m_totalScoreUI[index].m_transform.pos = baseTotalScorePos;
			m_totalScoreUI[index].m_transform.pos.x += index * gyokan;

			m_totalScoreUI[index].m_transform.scale = { FONT_SIZE ,FONT_SIZE };

		}



	}
	else {

		m_resultTitleUI.m_color.color.a = 0;
		m_resultRetryUI.m_color.color.a = 0;
		m_resultBackGroundUI.m_color.color.a = 0;
		for (auto& index : m_resultDayScoreUI) {
			index.m_color.color.a = 0;
		}
		for (auto& index : m_resultMineralScoreUI) {
			index.m_color.color.a = 0;
		}
		for (auto& index : m_reesultEnemyScoreUI) {
			index.m_color.color.a = 0;
		}
		for (auto& index : m_totalScoreUI) {
			index.m_color.color.a = 0;
		}


	}


}
