#include "GPURipplesEffect.h"
#include"../KazLibrary/Buffer/ShaderRandomTable.h"
#include"../KazLibrary/Input/KeyBoradInputManager.h"

GPURipplesEffect::GPURipplesEffect()
{
	//バッファ生成--------------------------------
	m_particleBuffer = KazBufferHelper::SetGPUBufferData(sizeof(FireFlyParticleData) * PARTICLE_MAX_NUM, "Ripples_GPU_Particle");
	m_outputBuffer = KazBufferHelper::SetGPUBufferData(sizeof(OutputData) * PARTICLE_MAX_NUM, "Ripples_GPU_Output");
	m_outputBuffer.elementNum = PARTICLE_MAX_NUM;
	m_outputBuffer.structureSize = sizeof(OutputData);
	m_outputBuffer.GenerateCounterBuffer();
	m_outputBuffer.CreateUAVView();
	m_cameraBuffer = KazBufferHelper::SetConstBufferData(sizeof(CameraData), "Ripples_UPLOAD_Camera");
	//バッファ生成--------------------------------

	m_executeIndirect = DrawFuncData::SetExecuteIndirect(
		DrawFuncData::GetSpriteInstanceShader(),
		m_outputBuffer.bufferWrapper->GetGpuAddress(),	//バッファのアドレス
		PARTICLE_MAX_NUM								//パーティクル数
	);
	m_executeIndirect.extraBufferArray.emplace_back(m_outputBuffer);
	m_executeIndirect.extraBufferArray.back().rangeType = GRAPHICS_RANGE_TYPE_UAV_VIEW;
	m_executeIndirect.extraBufferArray.back().rootParamType = GRAPHICS_PRAMTYPE_DATA;

	m_textureBuffer = TextureResourceMgr::Instance()->LoadGraphBuffer("Resource/VFX/WaterRing.png");
	m_executeIndirect.extraBufferArray.emplace_back(m_textureBuffer);
	m_executeIndirect.extraBufferArray.back().rangeType = GRAPHICS_RANGE_TYPE_SRV_DESC;
	m_executeIndirect.extraBufferArray.back().rootParamType = GRAPHICS_PRAMTYPE_DATA2;

	{
		std::vector<KazBufferHelper::BufferData>buffer;
		//波紋
		buffer.emplace_back(m_particleBuffer);
		buffer.back().rangeType = GRAPHICS_RANGE_TYPE_UAV_VIEW;
		buffer.back().rootParamType = GRAPHICS_PRAMTYPE_DATA;
		//乱数テーブル
		buffer.emplace_back(ShaderRandomTable::Instance()->GetBuffer(GRAPHICS_PRAMTYPE_DATA2));
		buffer.back().rangeType = GRAPHICS_RANGE_TYPE_UAV_VIEW;
		m_initShader.Generate(ShaderOptionData("Resource/ShaderFiles/ShaderFile/WaterRing.hlsl", "InitMain", "cs_6_4"), buffer);
	}

	{

		std::vector<KazBufferHelper::BufferData>buffer;
		//波紋
		buffer.emplace_back(m_particleBuffer);
		buffer.back().rangeType = GRAPHICS_RANGE_TYPE_UAV_VIEW;
		buffer.back().rootParamType = GRAPHICS_PRAMTYPE_DATA;
		//乱数テーブル
		buffer.emplace_back(ShaderRandomTable::Instance()->GetBuffer(GRAPHICS_PRAMTYPE_DATA2));
		buffer.back().rangeType = GRAPHICS_RANGE_TYPE_UAV_VIEW;
		//出力
		buffer.emplace_back(m_outputBuffer);
		buffer.back().rangeType = GRAPHICS_RANGE_TYPE_UAV_DESC;
		buffer.back().rootParamType = GRAPHICS_PRAMTYPE_DATA3;
		//カメラ
		buffer.emplace_back(m_cameraBuffer);
		buffer.back().rangeType = GRAPHICS_RANGE_TYPE_CBV_VIEW;
		buffer.back().rootParamType = GRAPHICS_PRAMTYPE_DATA;
		m_updateShader.Generate(ShaderOptionData("Resource/ShaderFiles/ShaderFile/WaterRing.hlsl", "UpdateMain", "cs_6_4"), buffer);
	}
	m_initShader.Compute({ GetThread(),1,1 });

	m_uploadCounterBuffer = KazBufferHelper::SetUploadBufferData(sizeof(UINT), "ResetCounterBuffer");
	UINT num = 0;
	m_uploadCounterBuffer.bufferWrapper->TransData(&num, sizeof(UINT));

}

void GPURipplesEffect::Update(bool arg_generateRain, const KazMath::Vec3<float>& arg_pos)
{
	CameraData data;
	data.m_viewProjectionMat = CameraMgr::Instance()->GetViewMatrix() * CameraMgr::Instance()->GetPerspectiveMatProjection();
	data.m_billboard = CameraMgr::Instance()->GetMatBillBoard();
	if (arg_generateRain)
	{
		data.m_appearFlag = 1;
	}
	else
	{
		data.m_appearFlag = 0;
	}
	data.m_pos = arg_pos.ConvertXMFLOAT3();
	m_cameraBuffer.bufferWrapper->TransData(&data, sizeof(CameraData));
	m_outputBuffer.counterWrapper->CopyBuffer(m_uploadCounterBuffer.bufferWrapper->GetBuffer());

	//描画の更新処理
	m_updateShader.Compute({ GetThread(),1,1 });
}

void GPURipplesEffect::Draw(DrawingByRasterize& arg_rasterize)
{
	arg_rasterize.ObjectRender(m_executeIndirect);
}
