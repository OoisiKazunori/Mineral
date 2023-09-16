#pragma once
#include "DrawCallSet.h"

class DrawCallUISet {

public:

	DrawFuncData::DrawCallData m_render;		//描画オブジェクト
	KazMath::Transform2D m_transform;			//座標、回転、スケール
	KazBufferHelper::BufferData m_texture;		//使用テクスチャ
	KazMath::Color m_color;						//色情報

public:

	void Load(std::string arg_filepath);

	void Draw(DrawingByRasterize& arg_rasterize);


};