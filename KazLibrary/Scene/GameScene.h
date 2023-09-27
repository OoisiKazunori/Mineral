#pragma once
#include"../KazLibrary/Scene/SceneBase.h"
#include"../KazLibrary/Buffer/DrawFuncData.h"
#include"../KazLibrary/Render/DrawFunc.h"
#include"../KazLibrary/Sound/SoundManager.h"
#include"../KazLibrary/Loader/ModelLoader.h"
#include"../KazLibrary/DrawFunc/DrawFuncHelper.h"
#include"../Game/Debug/DebugCamera.h"
#include"../Game/Render/GameRender.h"
#include"../Game/Render/BasicDraw.h"
#include"../Game/DrawCallSet.h"
#include"../Game/DrawCallUISet.h"
#include"../Game/VFX/SlapSmokeEffect.h"
#include"../Game/VFX/BulidSmokeEmitter.h"

class Player;
class MineralMgr;
class MineralTarget;
class Core;
class EnemyMgr;

class GameScene :public SceneBase
{

private:
	SoundData Select;
	SoundData Move;

	std::shared_ptr<Player> m_player;
	std::shared_ptr<MineralTarget> m_mineralTarget;
	std::shared_ptr<Core> m_goldCore;
	std::shared_ptr<EnemyMgr> m_enemyMgr;
	DebugCamera m_camera;

	KazMath::Transform3D m_stageTransform;

	DrawCallSet m_ground;
	DrawCallSet m_fence;
	DrawCallSet m_rock;
	DrawCallSet m_tree;
	DrawCallSet m_line;
	
	DrawCallSet m_puddle;

	DrawCallUISet m_titleLogoUI;
	DrawCallUISet m_titleBackGroundUI;
	DrawCallUISet m_titleStartUI;
	DrawCallUISet m_titleQuitUI;

	DrawCallUISet m_resultBackGroundUI;
	DrawCallUISet m_resultTitleUI;
	DrawCallUISet m_resultRetryUI;

	KazMath::Vec3<float> m_cameraEyeDir;
	KazMath::Vec3<float> m_targetOffset;
	float m_cameraEyeDistance;

	//タイトル関連
	int m_selectTitleNum;
	float m_selectTitleUISine;
	const float ADD_SELECT_TITLE_SINE = 0.2f;
	const float SELECT_TITLE_SINE_SCALE = 15.0f;

	//タイトルでロゴとかのアルファを消すまでのタイマー
	float m_titleLogoDeleteTimer;
	const float TITLELOGO_DELETE_TIMER = 10.0f;

	//リザルト関連。
	int m_selectResultNum;
	bool m_isResultToTitle;
	bool m_isResultToGame;
	int m_resultDayScore;
	int m_resultStayTimer;
	const int RESULT_STAY_TIMER = 60;

	bool m_isTitleNight;
	int m_isTitleNightChangeTimer;

	std::array<DrawCallUISet, 4> m_resultDayScoreUI;
	std::array<DrawCallUISet, 4> m_resultMineralScoreUI;
	std::array<DrawCallUISet, 4> m_reesultEnemyScoreUI;
	std::array<DrawCallUISet, 5> m_totalScoreUI;

	float fontScale = 29.5f;
	float gyokan = 25.3f;
	KazMath::Vec2<float> baseDayPos = { 700.0f, 202.0f };
	KazMath::Vec2<float> baseMineralPos = { 700.0f, 277.0f };
	KazMath::Vec2<float> baseEnemyScorePos = { 700.0f, 349.0f };
	KazMath::Vec2<float> baseTotalScorePos = { 673.0f, 453.0f };

	//UIのスケール
	KazMath::Vec2<float> UI_DEF_START_SCALE = KazMath::Vec2<float>(150.0f, 50.0f);
	KazMath::Vec2<float> UI_DEF_QUIT_SCALE = KazMath::Vec2<float>(125.0f, 50.0f);
	KazMath::Vec2<float> UI_MAX_START_SCALE = UI_DEF_START_SCALE * 1.5f;
	KazMath::Vec2<float> UI_MAX_QUIT_SCALE = UI_DEF_QUIT_SCALE * 1.5f;

	KazMath::Vec2<float> UI_DEF_TITLE_SCALE = KazMath::Vec2<float>(100.0f, 50.0f);
	KazMath::Vec2<float> UI_DEF_RETRY_SCALE = KazMath::Vec2<float>(100.0f, 50.0f);
	KazMath::Vec2<float> UI_MAX_TITLE_SCALE = KazMath::Vec2<float>(100.0f, 50.0f) * 1.3f;
	KazMath::Vec2<float> UI_MAX_RETRY_SCALE = KazMath::Vec2<float>(100.0f, 50.0f) * 1.3f;

	//煙エフェクト
	std::array<std::unique_ptr<SlapSmokeEffect>, 10> m_slapEffect;
	std::array<BulidSmokeEmitter, 10> m_bulidSmokeEffectArray;
	int m_slapEffectIndex;
	int m_bulidEffectIndex;
public:
	GameScene(DrawingByRasterize& arg_rasterize);
	~GameScene();

	void Init();
	void PreInit();
	void Finalize();
	void Input();
	void Update();
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	int SceneChange();

private:

	void UpdateTitle();
	void UpdateResult();

	int GetDigits(int arg_value, int arg_m, int arg_n) {
		int mod_value;
		int result;

		/* n桁目以下の桁を取得 */
		mod_value = arg_value % (int)pow(10, arg_n + 1);

		/* m桁目以上の桁を取得 */
		result = mod_value / static_cast<int>(pow(10, arg_m));

		return result;

	}
};