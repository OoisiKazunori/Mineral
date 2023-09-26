#pragma once
#include "../Game/DrawCallSet.h"
#include "../KazLibrary/Helper/ISinglton.h"
#include <memory>
#include <array>

class DestructibleTree;
class Player;

class DestructibleObjectMgr : public ISingleton<DestructibleObjectMgr>{

private:

	static const int TREE_COUNT = 16;
	std::array<std::shared_ptr<DestructibleTree>, TREE_COUNT> m_trees;


public:

	void Setting();

	void Init();

	void DebugGenerate();
	void GenerateTree(KazMath::Vec3<float> arg_pos);

	void Update(std::weak_ptr<Player> arg_player);

	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	int GetTargetTreeIndex(KazMath::Vec3<float> arg_playerPos, float arg_targetRange, float& arg_targetDistance);

	KazMath::Vec3<float> GetTreeScale(int arg_index);
	KazMath::Vec3<float> GetTreeTargetScale(int arg_index);
	KazMath::Vec3<float> GetTreePosZeroY(int arg_index);
	std::weak_ptr<DestructibleTree> GetTree(int arg_index) { return m_trees[arg_index]; }

};