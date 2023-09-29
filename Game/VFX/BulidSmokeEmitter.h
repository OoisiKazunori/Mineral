#pragma once
#include"SmokeParticle.h"

class BulidSmokeEmitter
{
public:
	BulidSmokeEmitter();
	void Init(const KazMath::Vec3<float>& arg_emittPos, float arg_range);
	void Update();
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);
	static void DebugImGui();

	bool IsActive();
private:
	static const int PARTICLE_MAX_NUM = 80;
	std::array<SmokeParticle, PARTICLE_MAX_NUM>m_particleArray;

	KazBufferHelper::BufferData m_smokeWorldMatBuffer;
	KazBufferHelper::BufferData m_smokeWorldMatVRAMBuffer;
	DrawFuncData::DrawCallData m_drawSmokeRender;

	std::array<CoordinateSpaceMatData, PARTICLE_MAX_NUM>m_matArray;

	int m_timer;
	bool m_isActiveFlag;
};
