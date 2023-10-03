#include "GPUFireFlyParticle.h"
#include"../KazLibrary/Buffer/ShaderRandomTable.h"

GPUFireFlyParticle::GPUFireFlyParticle()
{
	//�o�b�t�@����--------------------------------
	m_particleBuffer = KazBufferHelper::SetGPUBufferData(sizeof(FireFlyParticleData) * PARTICLE_MAX_NUM, "FireFly_GPU_Particle");
	m_outputBuffer = KazBufferHelper::SetGPUBufferData(sizeof(OutputData) * PARTICLE_MAX_NUM, "FireFly_GPU_Output");
	m_outputBuffer.elementNum = PARTICLE_MAX_NUM;
	m_outputBuffer.structureSize = sizeof(OutputData);
	m_outputBuffer.GenerateCounterBuffer();
	m_outputBuffer.CreateUAVView();
	m_cameraBuffer = KazBufferHelper::SetConstBufferData(sizeof(CameraData), "FireFly_UPLOAD_Camera");
	//�o�b�t�@����--------------------------------

	m_executeIndirect = DrawFuncData::SetExecuteIndirect(
		DrawFuncData::GetSpriteInstanceShader(),
		m_outputBuffer.bufferWrapper->GetGpuAddress(),					//�o�b�t�@�̃A�h���X
		PARTICLE_MAX_NUM					//�p�[�e�B�N����
	);
	m_executeIndirect.extraBufferArray.emplace_back(m_outputBuffer);
	m_executeIndirect.extraBufferArray.back().rangeType = GRAPHICS_RANGE_TYPE_UAV_VIEW;
	m_executeIndirect.extraBufferArray.back().rootParamType = GRAPHICS_PRAMTYPE_DATA;

	m_textureBuffer = TextureResourceMgr::Instance()->LoadGraphBuffer("Resource/VFX/Orb.png");
	m_executeIndirect.extraBufferArray.emplace_back(m_textureBuffer);
	m_executeIndirect.extraBufferArray.back().rangeType = GRAPHICS_RANGE_TYPE_SRV_DESC;
	m_executeIndirect.extraBufferArray.back().rootParamType = GRAPHICS_PRAMTYPE_DATA2;

	{
		std::vector<KazBufferHelper::BufferData>buffer;
		//�u
		buffer.emplace_back(m_particleBuffer);
		buffer.back().rangeType = GRAPHICS_RANGE_TYPE_UAV_VIEW;
		buffer.back().rootParamType = GRAPHICS_PRAMTYPE_DATA;
		//�����e�[�u��
		buffer.emplace_back(ShaderRandomTable::Instance()->GetCurlBuffer(GRAPHICS_PRAMTYPE_DATA2));
		buffer.back().rangeType = GRAPHICS_RANGE_TYPE_UAV_VIEW;
		m_initShader.Generate(ShaderOptionData("Resource/ShaderFiles/ShaderFile/FireFly.hlsl", "InitMain", "cs_6_4"), buffer);
	}

	{

		std::vector<KazBufferHelper::BufferData>buffer;
		//�u
		buffer.emplace_back(m_particleBuffer);
		buffer.back().rangeType = GRAPHICS_RANGE_TYPE_UAV_VIEW;
		buffer.back().rootParamType = GRAPHICS_PRAMTYPE_DATA;
		//�o��
		buffer.emplace_back(m_outputBuffer);
		buffer.back().rangeType = GRAPHICS_RANGE_TYPE_UAV_DESC;
		buffer.back().rootParamType = GRAPHICS_PRAMTYPE_DATA2;
		//�J����
		buffer.emplace_back(m_cameraBuffer);
		buffer.back().rangeType = GRAPHICS_RANGE_TYPE_CBV_VIEW;
		buffer.back().rootParamType = GRAPHICS_PRAMTYPE_DATA;
		m_updateShader.Generate(ShaderOptionData("Resource/ShaderFiles/ShaderFile/FireFly.hlsl", "UpdateMain", "cs_6_4"), buffer);
	}
	m_initShader.Compute({ GetThread(),1,1 });

	m_uploadCounterBuffer = KazBufferHelper::SetUploadBufferData(sizeof(UINT), "ResetCounterBuffer");
	UINT num = 0;
	m_uploadCounterBuffer.bufferWrapper->TransData(&num, sizeof(UINT));

}

void GPUFireFlyParticle::Update()
{
	CameraData data;
	data.m_viewProjectionMat = CameraMgr::Instance()->GetViewMatrix() * CameraMgr::Instance()->GetPerspectiveMatProjection();
	data.m_billboard = CameraMgr::Instance()->GetMatBillBoard();
	m_cameraBuffer.bufferWrapper->TransData(&data, sizeof(CameraData));
	m_outputBuffer.counterWrapper->CopyBuffer(m_uploadCounterBuffer.bufferWrapper->GetBuffer());
	//�`��̍X�V����
	m_updateShader.Compute({ GetThread(),1,1 });
}

void GPUFireFlyParticle::Draw(DrawingByRasterize& arg_rasterize)
{
	arg_rasterize.ObjectRender(m_executeIndirect);
}
