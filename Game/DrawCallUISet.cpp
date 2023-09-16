#include "DrawCallUISet.h"

void DrawCallUISet::Load(std::string arg_filepath)
{

	m_texture = TextureResourceMgr::Instance()->LoadGraphBuffer(arg_filepath);
	m_color = KazMath::Color(255,255,255,255);
	m_render = DrawFuncData::SetSpriteAlphaData(DrawFuncData::GetSpriteAlphaShader());

}

void DrawCallUISet::Draw(DrawingByRasterize& arg_rasterize)
{

	DrawFunc::DrawTextureIn2D(m_render, m_transform, m_texture, m_color);
	arg_rasterize.UIRender(m_render);

}
