#pragma once
#include"../DrawCallSet.h"
#include"../DrawCallUISet.h"
#include"SmokeParticle.h"

/// <summary>
/// 床を叩いたときの煙のエフェクト
/// </summary>
class SlapSmokeEffect
{
public:
	SlapSmokeEffect();
	void Init(const KazMath::Vec3<float>& arg_emittPos, float arg_range,bool arg_isStrong);
	void Update();
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);
	static void DebugImGui();
private:
	static const int PARTICLE_MAX_NUM = 80;
	std::array<SmokeParticle, PARTICLE_MAX_NUM>m_particleArray;

	//煙の大きさ調整
	static KazMath::Vec2<float>s_smokeScale;
	static KazMath::Vec2<float>s_smokeSubRange;
	static KazMath::Vec2<int>s_smokeInterval;

	KazBufferHelper::BufferData m_smokeWorldMatBuffer;
	KazBufferHelper::BufferData m_smokeWorldMatVRAMBuffer;
	DrawFuncData::DrawCallData m_drawSmokeRender;

	std::array<CoordinateSpaceMatData, PARTICLE_MAX_NUM>m_matArray;

};

