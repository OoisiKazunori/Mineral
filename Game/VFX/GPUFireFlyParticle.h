#pragma once
#include"Helper/KazBufferHelper.h"
#include"../KazLibrary/Helper/Compute.h"
#include"../Game/Render/BasicDraw.h"

class GPUFireFlyParticle
{
public:
	GPUFireFlyParticle();

	void Update();
	void Draw(DrawingByRasterize& arg_rasterize);

private:
	static const int PARTICLE_MAX_NUM = 1024 * 1;
	ComputeShader m_initShader, m_updateShader;
	//�u�̃p�[�e�B�N�����
	KazBufferHelper::BufferData m_particleBuffer;
	//�J�������
	KazBufferHelper::BufferData m_cameraBuffer;
	//�o�͏��
	KazBufferHelper::BufferData m_outputBuffer;
	KazBufferHelper::BufferData m_uploadCounterBuffer;

	//�p�[�e�B�N���`��
	DrawFuncData::DrawCallData m_executeIndirect;

	struct FireFlyParticleData
	{
		DirectX::XMFLOAT3 m_pos;
		DirectX::XMFLOAT3 m_scale;
		DirectX::XMFLOAT4 m_color;
		UINT m_timer;
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
	};

	UINT GetThread()
	{
		return PARTICLE_MAX_NUM / 1024;
	}
};
