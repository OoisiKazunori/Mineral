#include "HideSmokeEmitter.h"
#include"../KazLibrary/Easing/easing.h"

HideSmokeEmitter::HideSmokeEmitter()
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
}

void HideSmokeEmitter::Init(const KazMath::Vec3<float>& arg_emittPos, float arg_range, bool arg_isStrong)
{
	for (auto& obj : m_particleArray)
	{
		obj.m_transform.pos = arg_emittPos;
		obj.m_vel = { 0.0f,1.5f,0.0f };
		obj.m_maxScale = { 10.0f,10.0f,10.0f };
		obj.m_timer = 0;
		obj.m_maxTimer = 60;
	}
}

void HideSmokeEmitter::Update()
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

void HideSmokeEmitter::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{
	arg_rasterize.ObjectRender(m_drawSmokeRender);
	for (auto& obj : m_matArray)
	{
		arg_blasVec.Add(m_drawSmokeRender.m_raytracingData.m_blas[0], obj.m_world, 0);
	}
}
