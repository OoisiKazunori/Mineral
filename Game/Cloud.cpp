#include "Cloud.h"

Cloud::Cloud()
{

	for (int index = 0; index < CLOUD_COUNT; ++index) {

		m_cloudModel[index].Load("Resource/Cloud/", "Cloud.gltf");

		m_cloudSpeed[index] = KazMath::Rand(MIN_SPEED, MAX_SPEED);

		m_transform[index].scale = { KazMath::Rand(MIN_SCALE_X, MAX_SCALE_X), 10.0f, KazMath::Rand(MIN_SCALE_Z, MAX_SCALE_Z) };

		KazMath::Vec3<float> generatePos = { GENERATE_CENTER_X, KazMath::Rand(MIN_HEIGHT, MAX_HEIGHT), GENERATE_CENTER_Z };
		generatePos += KazMath::Vec3<float>(1.0f, 0.0f, 1.0f).GetNormal() * KazMath::Rand(-GENERATE_RANGE, GENERATE_RANGE);

		m_transform[index].pos = generatePos;

	}

	//������x�������B
	for (int index = 0; index < CLOUD_COUNT; ++index) {

		m_movedAmount[index] = KazMath::Rand(0.0f, RETURN_MOVE_AMOUNT);
		m_transform[index].pos += MOVE_VEC * m_movedAmount[index];

	}

}

void Cloud::Update()
{

	for (int index = 0; index < CLOUD_COUNT; ++index) {

		m_transform[index].pos += MOVE_VEC * m_cloudSpeed[index];
		m_movedAmount[index] += KazMath::Vec3<float>(MOVE_VEC * m_cloudSpeed[index]).Length();

		//��萔�������珉���ʒu�ɖ߂��B
		if (RETURN_MOVE_AMOUNT <= m_movedAmount[index]) {

			m_movedAmount[index] = 0.0f;
			KazMath::Vec3<float> generatePos = { GENERATE_CENTER_X, KazMath::Rand(MIN_HEIGHT, MAX_HEIGHT), GENERATE_CENTER_Z };
			generatePos += KazMath::Vec3<float>(1.0f, 0.0f, 1.0f).GetNormal() * KazMath::Rand(-GENERATE_RANGE, GENERATE_RANGE);
			m_transform[index].pos = generatePos;

		}

	}

}

#include "../KazLibrary/Imgui/imgui.h"
void Cloud::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{

	static float offset = 0.0f;


	ImGui::Begin("Cloud");

	ImGui::DragFloat("Y", &offset, 1.0f);

	ImGui::End();

	for (int index = 0; index < CLOUD_COUNT; ++index) {

		KazMath::Transform3D transform = m_transform[index];
		transform.pos.y += offset;
		m_cloudModel[index].Draw(arg_rasterize, arg_blasVec, transform);

	}

}