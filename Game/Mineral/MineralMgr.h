#pragma once
#include <Render/DrawingByRasterize.h>
#include <Buffer/DrawFuncData.h>
#include <Raytracing/BlasVector.h>
#include <memory>
#include <array>
#include "../Game/Core/Core.h"
#include "../KazLibrary/Helper/ISinglton.h"

class Mineral;
class Player;
class MineralTarget;

class MineralMgr : public ISingleton<MineralMgr> {

private:

	static const int MINERAL_COUNT = 64;
	std::array<std::shared_ptr<Mineral>, MINERAL_COUNT> m_minerals;

	int moveSECount;

public:

	void Setting();

	void DebugGenerate();

	void Generate(KazMath::Vec3<float>& arg_pos, KazMath::Vec3<float>& arg_respawnDir, int arg_mineralID);

	void Init();

	void Update(std::weak_ptr<Player> arg_player, std::weak_ptr<MineralTarget> arg_mineralTarget);

	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	bool SearchNearMineral(KazMath::Vec3<float> arg_pos, float arg_searchRange, int& arg_mineralIndex);

	std::weak_ptr<Mineral> GetMineral(int arg_index) { return m_minerals[arg_index]; }

	int GetMineralScore();

};