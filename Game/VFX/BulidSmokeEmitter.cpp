#include "BulidSmokeEmitter.h"

BulidSmokeEmitter::BulidSmokeEmitter()
{
	std::vector<ShaderOptionData>shaderArray;
	shaderArray.emplace_back(ShaderOptionData(KazFilePathName::RelativeShaderPath + "ShaderFile/" + "InstanceModel.hlsl", "VSDefferdMain", "vs_6_4", SHADER_TYPE_VERTEX));
	shaderArray.emplace_back(ShaderOptionData(KazFilePathName::RelativeShaderPath + "ShaderFile/" + "InstanceModel.hlsl", "PSDefferdMain", "ps_6_4", SHADER_TYPE_PIXEL));
	m_drawSmokeRender = DrawFuncData::SetDefferdRenderingInstanceModel(ModelLoader::Instance()->Load("Resource/VFX/smoke/", "Slap_VFX.gltf"), shaderArray);
	for (auto& obj : m_drawSmokeRender.drawMultiMeshesIndexInstanceCommandData.drawIndexInstancedData)
	{
		obj.instanceCount = PARTICLE_MAX_NUM;
	}

	m_smokeWorldMatBuffer = KazBufferHelper::SetUploadBufferData(sizeof(CoordinateSpaceMatData) * PARTICLE_MAX_NUM, "SmokeParticleRAM");
	m_smokeWorldMatVRAMBuffer = KazBufferHelper::SetGPUBufferData(sizeof(CoordinateSpaceMatData) * PARTICLE_MAX_NUM, "SmokeParticleVRAM");
	m_smokeWorldMatVRAMBuffer.bufferWrapper->ChangeBarrier(D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	m_smokeWorldMatVRAMBuffer.rangeType = GRAPHICS_RANGE_TYPE_UAV_VIEW;
	m_smokeWorldMatVRAMBuffer.rootParamType = GRAPHICS_PRAMTYPE_DATA;
	m_drawSmokeRender.extraBufferArray[0] = m_smokeWorldMatVRAMBuffer;
	m_isActiveFlag = false;
}

void BulidSmokeEmitter::Init(const KazMath::Vec3<float>& arg_emittPos, float arg_range)
{
	for (int i = 0; i < m_particleArray.size(); ++i)
	{
		float angle = KazMath::Rand<float>(360.0f, 0.0f);
		KazMath::Vec3<float>vel(cosf(KazMath::AngleToRadian(angle)), 0.0f, sinf(KazMath::AngleToRadian(angle)));
		m_particleArray[i].s_disappearMaxTime = 20;
		m_particleArray[i].s_velRateTime = 20;
		m_particleArray[i].s_scaleRateTime = 10;
		m_particleArray[i].Init(arg_emittPos, vel, arg_range, KazMath::Rand<float>(2.0f, 1.0f));
	}
	m_timer = 120;
	m_isActiveFlag = true;
}

void BulidSmokeEmitter::Update()
{
	int index = 0;
	for (auto& obj : m_particleArray)
	{
		obj.Update();
		CoordinateSpaceMatData data(
			obj.m_transform.GetMat(),
			CameraMgr::Instance()->GetViewMatrix(),
			CameraMgr::Instance()->GetPerspectiveMatProjection()
		);
		data.m_rotaion = DirectX::XMMatrixIdentity();
		m_matArray[index] = data;
		++index;
	}
	m_smokeWorldMatBuffer.bufferWrapper->TransData(m_matArray.data(), sizeof(CoordinateSpaceMatData) * PARTICLE_MAX_NUM);
	m_smokeWorldMatVRAMBuffer.bufferWrapper->CopyBuffer(m_smokeWorldMatBuffer.bufferWrapper->GetBuffer());

	--m_timer;
	if (m_timer <= 0)
	{
		m_isActiveFlag = false;
	}
}

void BulidSmokeEmitter::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{
	arg_rasterize.ObjectRender(m_drawSmokeRender);
	for (auto& obj : m_matArray)
	{
		arg_blasVec.Add(m_drawSmokeRender.m_raytracingData.m_blas[0], obj.m_world, 0);
	}
}

void BulidSmokeEmitter::DebugImGui()
{
}

bool BulidSmokeEmitter::IsActive()
{
	return m_isActiveFlag;
}
