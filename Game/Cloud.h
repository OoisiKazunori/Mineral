#pragma once
#include "../Game/DrawCallSet.h"

class Cloud {

private:

	static const int CLOUD_COUNT = 32;
	std::array<DrawCallSet, CLOUD_COUNT> m_cloudModel;
	std::array<KazMath::Transform3D, CLOUD_COUNT> m_transform;
	std::array<KazMath::Transform3D, CLOUD_COUNT> m_baseTransform;
	std::array<float, CLOUD_COUNT> m_cloudSpeed;
	std::array<float, CLOUD_COUNT> m_movedAmount;

	const float MIN_SPEED = 0.3f;
	const float MAX_SPEED = 0.5f;

	const float GENERATE_CENTER_X = 400.0f;
	const float GENERATE_CENTER_Z = -600.0f;
	const float GENERATE_RANGE = 1600.0f;
	
	const float MIN_HEIGHT = 700.0f;
	const float MAX_HEIGHT = 720.0f;

	const KazMath::Vec3<float> MOVE_VEC = KazMath::Vec3<float>(-1.0f, 0.0f, 1.0f).GetNormal();

	const float RETURN_MOVE_AMOUNT = 1500.0f;

	const float MIN_SCALE_X = 9.0f;
	const float MAX_SCALE_X = 50.0f;
	const float MIN_SCALE_Z = 9.0f;
	const float MAX_SCALE_Z = 50.0f;

public:

	Cloud();

	void Update(bool arg_disapperCloud);

	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

};