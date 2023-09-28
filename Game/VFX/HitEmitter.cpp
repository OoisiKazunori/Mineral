#include "HitEmitter.h"
#include"../KazLibrary/Easing/easing.h"

HitEmitter::HitEmitter()
{
	m_particleIndex = 0;
	
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
}

void HitEmitter::Init(const KazMath::Vec3<float>& arg_emittPos, float arg_range, bool arg_isStrong)
{
	++m_particleIndex;
	if (m_particleArray.size() <= m_particleIndex)
	{
		m_particleIndex = 0;
	}
	m_particleArray[m_particleIndex].m_transform.pos = arg_emittPos;
	m_particleArray[m_particleIndex].m_vel = { 0.0f,0.2f,0.0f };
	m_particleArray[m_particleIndex].m_maxScale = { 8.0f,8.0f,8.0f };
	m_particleArray[m_particleIndex].m_timer = 0;
	m_particleArray[m_particleIndex].m_maxTimer = 20;
	++m_particleIndex;
}

void HitEmitter::Update()
{
	int index = 0;
	for (auto& obj : m_particleArray)
	{
		//イージング用の計算
		if (obj.m_timer < obj.m_maxTimer)
		{
			++obj.m_timer;
		}
		obj.m_transform.pos += obj.m_vel;
		float rate = static_cast<float>(obj.m_timer) / static_cast<float>(obj.m_maxTimer);
		obj.m_transform.scale =
		{
			1.0f - EasingMaker(In, Exp, rate),
			1.0f - EasingMaker(In, Exp, rate),
			1.0f - EasingMaker(In, Exp, rate)
		};
		obj.m_transform.scale *= obj.m_maxScale;

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
}

void HitEmitter::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{
	arg_rasterize.ObjectRender(m_drawSmokeRender);
	for (auto& obj : m_matArray)
	{
		arg_blasVec.Add(m_drawSmokeRender.m_raytracingData.m_blas[0], obj.m_world, 0);
	}
}
