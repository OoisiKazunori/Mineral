#pragma once
#include "../KazLibrary/Helper/ISinglton.h"
#include "../Game/DrawCallSet.h"
#include "../Game/Collision/MeshCollision.h"
#include "../Game/VFX/BulidSmokeEmitter.h"
#include <memory>
#include <array>

class Wall;
class Player;

class BuildingMgr : public ISingleton<BuildingMgr> {

private:

	static const int WALL_COUNT = 3;
	std::array<std::shared_ptr<Wall>, WALL_COUNT> m_walls;

public:

	void Setting();

	void Init();

	void Generate();

	void Update(std::weak_ptr<Player> arg_player, BulidSmokeEmitter& arg_smokeEffect,int *arg_smokeIndex);

	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	int GetTargetWallIndex(KazMath::Vec3<float> arg_playerPos, float arg_targetRange, float& arg_targetDistance);

	MeshCollision::CheckHitResult CheckHitWall(KazMath::Vec3<float> arg_rayPos, KazMath::Vec3<float> arg_rayDir, float arg_rayLength, int& arg_wallIndex);

	std::weak_ptr<Wall> GetWall(int arg_index) { return m_walls[arg_index]; }
	KazMath::Vec3<float> GetWallPosZeroY(int arg_index);
	KazMath::Vec3<float> GetWallScale(int arg_index);
	void DamageWall(int arg_index, int arg_damage = 1);
	int GetWallHP(int arg_index);

	int GetMaterialCounterWall(int arg_index);
	void AddMaterialWall(int arg_index);
	bool IsFullMaterialWall(int arg_index);
	bool GetIsBuildWall(int arg_index);

};