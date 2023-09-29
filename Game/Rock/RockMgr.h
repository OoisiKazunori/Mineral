#pragma once
#include <Render/DrawingByRasterize.h>
#include <Buffer/DrawFuncData.h>
#include <Raytracing/BlasVector.h>
#include <memory>
#include <array>
#include "../KazLibrary/Helper/ISinglton.h"
#include"../VFX/BulidSmokeEmitter.h"

class Rock;
class Player;

class RockMgr : public ISingleton<RockMgr>{

private:

	static const int ROCK_COUNT = 32;
	std::array<std::shared_ptr<Rock>, ROCK_COUNT> m_rocks;
	std::array<BulidSmokeEmitter, ROCK_COUNT> m_buildSmokeEmitterArray;

public:

	void Setting();

	void DebugGenerate();

	void Generate(KazMath::Vec3<float> arg_pos, KazMath::Vec3<float> arg_respawnDir, bool m_isMineralRock, int arg_mineralID);

	void Init();

	void Update(std::weak_ptr<Player> arg_player);

	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	int GetTargetRockIndex(KazMath::Vec3<float> arg_playerPos, float arg_targetRange, float& arg_targetDistance);

	KazMath::Vec3<float> GetPos(int arg_index);
	KazMath::Vec3<float> GetScale(int arg_index);

	std::weak_ptr<Rock> GetRock(int arg_index) { return m_rocks[arg_index];};

};