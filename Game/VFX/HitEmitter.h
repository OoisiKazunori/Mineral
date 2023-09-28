#pragma once
#include"../DrawCallSet.h"
#include"../DrawCallUISet.h"
#include"SmokeParticle.h"

class HitEmitter
{
public:
	HitEmitter();
	void Init(const KazMath::Vec3<float>& arg_emittPos, float arg_range, bool arg_isStrong);
	void Update();
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);
private:
	static const int PARTICLE_MAX_NUM = 10;
	int m_particleIndex;
	struct Particle
	{
		KazMath::Transform3D m_transform;
		KazMath::Vec3<float>m_vel;
		KazMath::Vec3<float>m_maxScale;
		int m_timer;
		int m_maxTimer;
	};
	std::array<Particle, PARTICLE_MAX_NUM>m_particleArray;

	KazBufferHelper::BufferData m_smokeWorldMatBuffer;
	KazBufferHelper::BufferData m_smokeWorldMatVRAMBuffer;
	DrawFuncData::DrawCallData m_drawSmokeRender;

	std::array<CoordinateSpaceMatData, PARTICLE_MAX_NUM>m_matArray;
};

