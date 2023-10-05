#pragma once
#include"Helper/KazBufferHelper.h"
#include"../KazLibrary/Helper/Compute.h"
#include"../Game/Render/BasicDraw.h"

class GPUFireFlyParticle
{
public:
	GPUFireFlyParticle();

	void Update(bool arg_nightFlag);
	void Draw(DrawingByRasterize& arg_rasterize);

private:
	static const int PARTICLE_MAX_NUM = 1024 * 10;
	ComputeShader m_initShader, m_updateShader;

	KazBufferHelper::BufferData m_particleBuffer;	//蛍のパーティクル情報
	KazBufferHelper::BufferData m_cameraBuffer;		//カメラ情報
	KazBufferHelper::BufferData m_outputBuffer;		//出力情報
	KazBufferHelper::BufferData m_uploadCounterBuffer;
	KazBufferHelper::BufferData m_textureBuffer;

	//パーティクル描画
	DrawFuncData::DrawCallData m_executeIndirect;

	struct LerpData
	{
		DirectX::XMFLOAT3 base;
		DirectX::XMFLOAT3 lerp;
		float mul;
	};

	struct FireFlyParticleData
	{
		LerpData m_pos;
		LerpData m_scale;
		UINT m_shrinkFlag;
		float m_shrinkScale;
		float shirinkBaseScale;
		DirectX::XMFLOAT4 m_color;
		float m_scaleTimer;
		float m_scaleMaxTime;
		float m_timer;
		DirectX::XMFLOAT2 m_moveTimer;
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
