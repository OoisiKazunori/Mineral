#pragma once
#include"RenderTarget/RenderTargetStatus.h"
#include"PostEffect/GaussianBlur.h"
#include"DrawFunc/DrawFuncHelper.h"

/// <summary>
/// �|�X�g�G�t�F�N�g
/// </summary>
class GameRender
{
public:
	GameRender();
	void DrawPass(DrawingByRasterize& arg_rasterize);

private:
	std::unique_ptr<PostEffect::GaussianBlur> m_blurEmissive;

	DrawFuncHelper::TextureRender m_albedoAndNormalPass;//�A���x�h�Ɩ@���̏o��
	DrawFuncHelper::TextureRender m_emissivePass;		//�G�~�b�V�u
	DrawFuncHelper::TextureRender m_finalPass;			//�ŏI����

	RESOURCE_HANDLE m_compositeHandle;				//�A���x�h&�@���ƃu���[���̍���

	KazMath::Transform2D m_transform;
};

