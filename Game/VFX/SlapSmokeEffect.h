#pragma once
#include"../DrawCallSet.h"
#include"../DrawCallUISet.h"

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
	class Particle
	{
	public:
		Particle();
		void Init(
			const KazMath::Vec3<float>& arg_emittPos,
			const KazMath::Vec3<float>& arg_vel,
			float arg_radius,
			float arg_scale
		);
		void Update();
		void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

		static int s_disappearMaxTime;	//スケールの縮小が始まるタイミングの基準
		static int s_velRateTime;		//攻撃範囲まで広がる時間
		static int s_scaleRateTime;		//煙が消える時間
	private:
		bool m_initFlag;
		DrawCallSet m_smokeRender;
		KazMath::Transform3D m_transform;
		KazMath::Vec3<float>m_emittPos;
		KazMath::Vec3<float>m_vel;
		float m_scaleMax;
		float m_velRate;		//攻撃範囲まで広げる割合
		float m_radius;			//広がる範囲
		int m_disappearTimer;
		int m_disappearMaxTimer;//スケールの縮小が始まるタイミングの基準からある程度ランダム性を持たせたもの
		float m_scaleRate;		//消える際のスピード
	};

	static const int PARTICLE_MAX_NUM = 80;
	std::array<Particle, PARTICLE_MAX_NUM>m_particleArray;

	//煙の大きさ調整
	static KazMath::Vec2<float>s_smokeScale;
	static KazMath::Vec2<float>s_smokeSubRange;
	static KazMath::Vec2<int>s_smokeInterval;

};

