#pragma once
#include"Helper/KazBufferHelper.h"
#include"../KazLibrary/Helper/Compute.h"
#include"../Game/Render/BasicDraw.h"

class GPURipplesEffect
{
public:
	GPURipplesEffect();

	void Update(bool arg_generateRain, const KazMath::Vec3<float>& arg_pos);
	void Draw(DrawingByRasterize& arg_rasterize);

private:
	static const int PARTICLE_MAX_NUM = 1024 * 3;
	ComputeShader m_initShader, m_updateShader;

	KazBufferHelper::BufferData m_particleBuffer;	//蛍のパーティクル情報
	KazBufferHelper::BufferData m_cameraBuffer;		//カメラ情報
	KazBufferHelper::BufferData m_outputBuffer;		//出力情報
	KazBufferHelper::BufferData m_uploadCounterBuffer;
	KazBufferHelper::BufferData m_textureBuffer;

	//パーティクル描画
	DrawFuncData::DrawCallData m_executeIndirect;

	struct FireFlyParticleData
	{
		DirectX::XMFLOAT3 m_pos;
		DirectX::XMFLOAT3 m_scale;
		int m_timer;
		int m_coolTimer;
		int m_coolMaxTimer;
		int m_coolTimeFlag;
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
		DirectX::XMFLOAT3 m_pos;
		UINT m_appearFlag;
	};

	UINT GetThread()
	{
		return PARTICLE_MAX_NUM / 1024;
	}
};

