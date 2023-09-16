#include "NumberFont.h"

void NumberFont::Load()
{

	m_font[0] = TextureResourceMgr::Instance()->LoadGraphBuffer("Resource/UI/NumFont/0.png");
	m_font[0].rootParamType = GRAPHICS_PRAMTYPE_DATA;
	m_font[1] = TextureResourceMgr::Instance()->LoadGraphBuffer("Resource/UI/NumFont/1.png");
	m_font[1].rootParamType = GRAPHICS_PRAMTYPE_DATA;
	m_font[2] = TextureResourceMgr::Instance()->LoadGraphBuffer("Resource/UI/NumFont/2.png");
	m_font[2].rootParamType = GRAPHICS_PRAMTYPE_DATA;
	m_font[3] = TextureResourceMgr::Instance()->LoadGraphBuffer("Resource/UI/NumFont/3.png");
	m_font[3].rootParamType = GRAPHICS_PRAMTYPE_DATA;
	m_font[4] = TextureResourceMgr::Instance()->LoadGraphBuffer("Resource/UI/NumFont/4.png");
	m_font[4].rootParamType = GRAPHICS_PRAMTYPE_DATA;
	m_font[5] = TextureResourceMgr::Instance()->LoadGraphBuffer("Resource/UI/NumFont/5.png");
	m_font[5].rootParamType = GRAPHICS_PRAMTYPE_DATA;
	m_font[6] = TextureResourceMgr::Instance()->LoadGraphBuffer("Resource/UI/NumFont/6.png");
	m_font[6].rootParamType = GRAPHICS_PRAMTYPE_DATA;
	m_font[7] = TextureResourceMgr::Instance()->LoadGraphBuffer("Resource/UI/NumFont/7.png");
	m_font[7].rootParamType = GRAPHICS_PRAMTYPE_DATA;
	m_font[8] = TextureResourceMgr::Instance()->LoadGraphBuffer("Resource/UI/NumFont/8.png");
	m_font[8].rootParamType = GRAPHICS_PRAMTYPE_DATA;
	m_font[9] = TextureResourceMgr::Instance()->LoadGraphBuffer("Resource/UI/NumFont/9.png");
	m_font[9].rootParamType = GRAPHICS_PRAMTYPE_DATA;

}
