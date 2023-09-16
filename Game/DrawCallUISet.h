#pragma once
#include "DrawCallSet.h"

class DrawCallUISet {

public:

	DrawFuncData::DrawCallData m_render;		//�`��I�u�W�F�N�g
	KazMath::Transform2D m_transform;			//���W�A��]�A�X�P�[��
	KazBufferHelper::BufferData m_texture;		//�g�p�e�N�X�`��
	KazMath::Color m_color;						//�F���

public:

	void Load(std::string arg_filepath);

	void Draw(DrawingByRasterize& arg_rasterize);


};