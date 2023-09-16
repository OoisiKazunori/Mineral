#pragma once
#include "../KazLibrary/Helper/ISinglton.h"
#include "../Game/DrawCallSet.h"
#include <memory>
#include <array>

class BuildingMaterial;

class BuildingMaterialMgr : public ISingleton<BuildingMaterialMgr> {

private:

	static const int BUILDING_MATERIAL_COUNT = 16;
	std::array<std::shared_ptr<BuildingMaterial>, BUILDING_MATERIAL_COUNT> m_materials;

public:

	void Setting();

	void Init();

	void Generate(KazMath::Vec3<float> arg_pos, KazMath::Vec3<float> arg_generateVec);

	void Update();

	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	int GetTargetMaterialIndex(KazMath::Vec3<float> arg_playerPos, float arg_targetRange, float& arg_targetDistance);
	std::weak_ptr<BuildingMaterial> GetMaterial(int arg_index) { return m_materials[arg_index]; }
	KazMath::Vec3<float> GetMaterialScale(int arg_index);
	KazMath::Vec3<float> GetMaterialPosZeroY(int arg_index);

};