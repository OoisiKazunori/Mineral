#pragma once
#include "../Game/DrawCallSet.h"
#include "../Enemy/EnemyRoute.h"
#include <array>
#include <memory>

class MineKuji;
class MineTsumuri;
class Core;
class Player;

class EnemyMgr {

private:

	static const int MAX_MINEKUJI = 32;
	std::array<std::shared_ptr<MineKuji>, MAX_MINEKUJI> m_minekujis;
	static const int MAX_MINETSUMURI = 16;
	std::array<std::shared_ptr<MineTsumuri>, MAX_MINETSUMURI> m_minetsumuri;

public:

	EnemyMgr();

	void Init();

	void DebugGenerate();

	void GenerateMinekuji(EnemyRoute::ROUTE_ID arg_routeID, bool arg_isTutorialEnemy = false, bool arg_isTogekuri = false);
	void GenerateMinetsumuri(EnemyRoute::ROUTE_ID arg_routeID, bool arg_isKing);

	//ノックバック判定
	void CheckNockBack(KazMath::Vec3<float> arg_wallPos, float arg_scale);

	void Update(std::weak_ptr<Core> arg_core, std::weak_ptr<Player> arg_player);

	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	int GetTargetMineKujiIndex(KazMath::Vec3<float> arg_playerPos, float arg_targetRange, float& arg_targetDistance);
	int GetTargetMineTsumuriIndex(KazMath::Vec3<float> arg_playerPos, float arg_targetRange, float& arg_targetDistance);
	KazMath::Vec3<float> GetMineKujiScale(int arg_index);
	KazMath::Vec3<float> GetMineKujiTargetScale(int arg_index);
	KazMath::Vec3<float> GetMineKujiPos(int arg_index);
	std::weak_ptr<MineKuji> GetMineKuji(int arg_index);
	KazMath::Vec3<float> GetMineTsumuriScale(int arg_index);
	KazMath::Vec3<float> GetMineTsumuriTargetScale(int arg_index);
	KazMath::Vec3<float> GetMineTsumuriPos(int arg_index);
	std::weak_ptr<MineTsumuri> GetMineTsumuri(int arg_index);
	bool GetIsMineking(int arg_index);

	int GetAliveEnemyCount();

};