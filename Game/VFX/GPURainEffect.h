#pragma once
#include"Helper/KazBufferHelper.h"
#include"../KazLibrary/Helper/Compute.h"
#include"../Game/Render/BasicDraw.h"

class GPURainEffect
{
public:
	GPURainEffect();

	void Update(bool arg_generateRain);
	void Draw(DrawingByRasterize& arg_rasterize);

private:
	static const int PARTICLE_MAX_NUM = 1024 * 1;
	ComputeShader m_initShader, m_updateShader;

	KazBufferHelper::BufferData m_particleBuffer;	//蛍のパーティクル情報
	KazBufferHelper::BufferData m_cameraBuffer;		//カメラ情報
	KazBufferHelper::BufferData m_outputBuffer;		//出力情報
	KazBufferHelper::BufferData m_uploadCounterBuffer;

	//パーティクル描画
	DrawFuncData::DrawCallData m_executeIndirect;

	struct FireFlyParticleData
	{
		DirectX::XMFLOAT3 m_pos;
	};
	struct OutputData
	{
		DirectX::XMMATRIX m_mat;
		DirectX::XMFLOAT4 m_color;
	};
	struct CameraData
	{
		DirectX::XMMATRIX m_viewProjectionMat;
		DirectX::XMMATRIX m_billboard;
		UINT m_appearFlag;
	};

	UINT GetThread()
	{
		return PARTICLE_MAX_NUM / 1024;
	}
};

