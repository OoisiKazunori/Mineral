#pragma once
#include"Render/DrawFunc.h"
#include"Render/DrawingByRasterize.h"
#include"Loader/ModelLoader.h"

namespace DrawFuncHelper
{
	//�X�v���C�g�`��̃��b�p�[
	struct TextureRender
	{
		DrawFuncData::DrawCallData m_drawCommand;
		KazBufferHelper::BufferData m_textureBuffer;
		KazMath::Vec2<float> m_textureSize;

		/// <summary>
		/// �e�N�X�`���ǂݍ���
		/// </summary>
		/// <param name="arg_textureFilePass">�e�N�X�`���̃t�@�C���p�X</param>
		/// <param name="arg_alphaFlag">���ߗL�薳��</param>
		TextureRender(const std::string& arg_textureFilePass);
		/// <summary>
		/// �e�N�X�`���ǂݍ��݂ƕ`����̐ݒ�
		/// </summary>
		/// <param name="arg_textureFilePass">�e�N�X�`���̃t�@�C���p�X</param>
		/// <param name="arg_drawCall">�`����̐���</param>
		TextureRender(const std::string& arg_textureFilePass, const DrawFuncData::DrawCallData& arg_drawCall);
		TextureRender(const DrawFuncData::DrawCallData& arg_drawCall);
		TextureRender();


		void operator=(const KazBufferHelper::BufferData& rhs);

		/// <summary>
		/// �X�v���C�g2D�`��
		/// </summary>
		void Draw2D(DrawingByRasterize& arg_rasterize, const KazMath::Transform2D& arg_trasform2D, const KazMath::Color& arg_addColor = KazMath::Color(255, 255, 255, 255));
		/// <summary>
		/// �X�v���C�g3D�`��
		/// </summary>
		void Draw3D(DrawingByRasterize& arg_rasterize, const KazMath::Transform3D& arg_trasform3D, const KazMath::Color& arg_addColor = KazMath::Color(255, 255, 255, 255));

		void Error();
	};


	struct ModelRender
	{
		DrawFuncData::DrawCallData m_drawCommand;
		std::shared_ptr<ModelInfomation> m_modelInfo;
		std::shared_ptr<ModelAnimator> m_animator;

		ModelRender(const std::string& arg_fileDir, const std::string& arg_filePass);
		ModelRender(const std::shared_ptr<ModelInfomation>& arg_modelInfomation, const DrawFuncData::DrawCallData& arg_drawCall);
		ModelRender();

		void Load(const std::string& arg_fileDir, const std::string& arg_filePass);
		void Load(const std::shared_ptr<ModelInfomation>& arg_modelInfomation, const DrawFuncData::DrawCallData& arg_drawCall);

		bool LoadAnimation();

		void Draw(DrawingByRasterize& arg_rasterize, KazMath::Transform3D& arg_trasform3D, const KazMath::Color& arg_addColor = KazMath::Color(255, 255, 255, 255), float arg_timeScale = 1.0f);
		void Error();

	};

}