#pragma once
#include "../Game/DrawCallSet.h"

class BuildingMaterial {

private:

	DrawCallSet m_model;
	KazMath::Transform3D m_transform;
	KazMath::Vec3<float> m_generateVec;
	KazMath::Vec3<float> m_heldPos;
	float m_gravity;
	const float GRAVITY = 0.1f;
	bool m_isActive;
	bool m_isHeld;

	//âÒé˚ÇµÇΩéûÇÃãììÆóp
	float m_upperVec;
	const float UPPER_VEC = 8.0f;
	const float SUB_UPPER_VEC = 0.8f;
	float m_upperGravity;
	const float UPPER_GRAVITY = 0.2f;
	int m_upperStayTimer;
	const int UPPER_STAY_TIMER = 15;

	const float SCALE = 3.0f;

	const float GROUND = 5.0f;

public:

	BuildingMaterial();

	void Init();

	void Generate(KazMath::Vec3<float> arg_pos, KazMath::Vec3<float> arg_generateVec);

	void Update();

	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	//éùÇΩÇÍÇΩîªíË
	void Held();
	void Release() { m_isHeld = false; };

	bool GetIsActive() { return m_isActive; }
	bool GetIsHeld() { return m_isHeld; }
	KazMath::Vec3<float> GetPos() { return m_transform.pos; }
	KazMath::Vec3<float> GetPosZeroY() { return KazMath::Vec3<float>(m_transform.pos.x, 0.0f, m_transform.pos.z); }
	KazMath::Vec3<float> GetScale() { return m_transform.scale; }
	void SetPos(KazMath::Vec3<float> arg_pos);

};