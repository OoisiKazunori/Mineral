#include "BuildingMaterial.h"

BuildingMaterial::BuildingMaterial()
{

	m_model.LoadOutline("Resource/Mineral/", "Wood.gltf");
	m_isActive = false;
	m_isHeld = false;
	m_gravity = 0.0f;

}

void BuildingMaterial::Init()
{

	m_isActive = false;
	m_isHeld = false;
	m_gravity = 0.0f;

}

void BuildingMaterial::Generate(KazMath::Vec3<float> arg_pos, KazMath::Vec3<float> arg_generateVec)
{

	m_transform.pos = arg_pos;
	m_generateVec = arg_generateVec;
	m_isActive = true;
	m_isHeld = false;
	m_gravity = 0.0f;

}

void BuildingMaterial::Update()
{

	if (m_isHeld) {


		m_upperVec -= m_upperVec / 5.0f;

		m_transform.pos.y += m_upperVec;

		if (m_upperVec <= 0.1f) {
		}

		//UpperVecが1より小さくて座標がGROUNDより小さかったらそれ以下にならないようにする。
		if (m_upperVec < 0.1f) {

			m_upperStayTimer = std::clamp(m_upperStayTimer + 1, 0, UPPER_STAY_TIMER);

			if (UPPER_STAY_TIMER <= m_upperStayTimer) {

				m_transform.pos.x += (m_heldPos.x - m_transform.pos.x) / 5.0f;
				m_transform.pos.z += (m_heldPos.z - m_transform.pos.z) / 5.0f;


				m_upperGravity = std::clamp(m_upperGravity - UPPER_GRAVITY, -10000.0f, 0.0f);
				m_transform.pos.y += m_upperGravity;

			}

			m_transform.pos.y = std::clamp(m_transform.pos.y, m_heldPos.y, 10000.0f);

		}

		return;
	}

	//座標を更新。
	if (0.01f < m_generateVec.Length()) {

		m_transform.pos += m_generateVec;
		m_generateVec -= m_generateVec / 10.0f;

	}
	else {

		m_generateVec = {};

	}

	//重力をかける。
	if (GROUND < m_transform.pos.y) {

		m_gravity += GRAVITY;
		m_transform.pos.y -= m_gravity;

	}
	else {

		m_gravity = 0.0f;
		m_transform.pos.y = GROUND;

	}

}

void BuildingMaterial::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{

	if (m_isActive) {

		m_transform.scale = { SCALE,SCALE,SCALE };

	}
	else {

		m_transform.scale = { 0,0,0 };

	}

	DessolveOutline outline;
	outline.m_outline = KazMath::Vec4<float>(0.5f, 0, 0, 1);
	m_model.m_model.extraBufferArray[4].bufferWrapper->TransData(&outline, sizeof(DessolveOutline));

	m_model.m_model.extraBufferArray.back() = GBufferMgr::Instance()->m_outlineBuffer;
	m_model.m_model.extraBufferArray.back().rangeType = GRAPHICS_RANGE_TYPE_UAV_DESC;
	m_model.m_model.extraBufferArray.back().rootParamType = GRAPHICS_PRAMTYPE_TEX;

	m_model.Draw(arg_rasterize, arg_blasVec, m_transform);

}

void BuildingMaterial::Held()
{
	m_isHeld = true;
	m_upperVec = UPPER_VEC;
	m_upperGravity = 0.0f;
	m_upperStayTimer = 0;
}

void BuildingMaterial::SetPos(KazMath::Vec3<float> arg_pos)
{
	m_heldPos = arg_pos;
}
