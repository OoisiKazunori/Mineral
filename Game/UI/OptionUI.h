//#pragma once
//#include "../KazLibrary/Helper/ISinglton.h"
//#include "../KazLibrary/Helper/KazBufferHelper.h"
//#include "../KazLibrary/Math/KazMath.h"
//#include "../KazLibrary/Render/DrawFunc.h"
//#include "Render/DrawingByRasterize.h"
//#include "../KazLibrary/Sound/SoundManager.h"
//#include <string>
//#include <memory>
//#include <vector>
//#include <Buffer/DrawFuncData.h>
//
//class OptionUI :public ISingleton<OptionUI>
//{
//
//public:
//
//	//�`�悷�镶����A�`��ɕK�v��DrawCallData���܂Ƃ߂��\����
//	struct DrawStringData {
//		std::string m_string;
//		std::array<DrawFuncData::DrawCallData, 32> m_render;
//		std::array<KazMath::Color, 32> m_color;
//		DrawStringData() {};
//		DrawStringData(std::string arg_name) : m_string(arg_name) {};
//	};
//
//	//�I�v�V�����̏����o���p�\����
//	struct OptionHeadline {
//		DrawStringData m_name;
//		KazMath::Vec2<float> m_pos;
//		float m_fontSize;
//		int m_headlineID;
//		OptionHeadline(std::string arg_headline, KazMath::Vec2<float> arg_pos, float arg_fontSize, int arg_headlineID);
//	};
//
//	//�����o���z��
//	std::vector<OptionHeadline> m_headlines;
//	std::vector<OptionHeadline> m_optionUI;	//"OPTION"�p
//
//	//�I�v�V�����ڍחp�\����
//	struct OptionDetails {
//		DrawStringData m_name;
//		std::vector<DrawStringData> m_selectName;
//		KazMath::Vec2<float> m_pos;
//		int m_id;
//		int m_selectID;
//		OptionDetails(std::string arg_name, std::vector<DrawStringData> arg_selectName, KazMath::Vec2<float> arg_pos, int arg_id);
//	};
//
//	//�I�v�V�����ڍחp�z��
//	std::vector<OptionDetails> m_optionDetails;
//
//	//���
//	KazBufferHelper::BufferData m_rightArrowTexture;
//	KazBufferHelper::BufferData m_leftArrowTexture;
//	DrawFuncData::DrawCallData m_rightArrowRender;
//	DrawFuncData::DrawCallData m_leftArrowRender;
//
//	//OnOff�؂�ւ����C��
//	DrawFuncData::DrawCallData m_debugOnOffLineRender;
//	KazBufferHelper::BufferData m_debugOnOffLineBuffer;
//	KazBufferHelper::BufferData m_debugOnOffLineStayBuffer;
//	KazMath::Transform2D m_debugOnOffLineTransform;
//
//
//	SoundData m_doneSE;
//	SoundData m_cancelSE;
//	SoundData m_selectSE;
//
//	//���ݑI�𒆂̏����o��ID
//	int m_nowSelectHeadline;
//
//	//���͕ۑ��p
//	bool m_prevInputUp;
//	bool m_prevInputDown;
//	bool m_prevInputRight;
//	bool m_prevInputLeft;
//
//	//UI��\�����邩�ǂ����̃t���O
//	bool m_isDisplayUI;
//	bool m_isChangeDisplayUI;
//
//	//���C�g���̃f�o�b�O�̃t���O
//	bool m_isRaytracingDebug;
//	bool m_isExit;
//
//	//�t�H���g�z��
//	std::array<KazBufferHelper::BufferData, 26> m_font;
//
//	//�f�o�b�O�̕\�����̔w�i���Â�����p�̃e�N�X�`��
//	DrawFuncData::DrawCallData m_backGroundRender;
//	KazMath::Color m_backGroundColor;
//	KazBufferHelper::BufferData m_backGroundTexture;
//	const int BACK_GROUND_ALPHA = 220;
//	float m_backGroundEasingTimer;
//	const float BACK_GROUND_EASING_TIMER = 20;
//
//	//OnOff�̕���
//	std::string m_onString;
//	std::string m_offString;
//	std::array<DrawFuncData::DrawCallData, 2> m_onRender;
//	std::array<DrawFuncData::DrawCallData, 3> m_offRender;
//
//	//UI�z�u�p�ϐ�
//	const float BETWEEN_LINES = 55.0f;
//	const KazMath::Vec2<float> HEADLINE_BASEPOS = KazMath::Vec2<float>(170 + 150, 300);
//	const float DEFAULT_FONTSIZE = 25.0f;
//	const float SELECT_FONTSIZE = 30.0f;
//	const float OPTION_FONTSIZE = 45.7f;
//	const KazMath::Vec2<float> OPTION_BASEPOS = KazMath::Vec2<float>(143 + 150, 240);
//	const KazMath::Vec2<float> DETAIL_BASEPOS = KazMath::Vec2<float>(600 + 150, 300);
//	const float DETAIL_FONTSIZE = 25.0f;
//	const float DETAIL_FLAG_POS = 180.0f;	//�ڍׂ̉E����ONOFF�Ƃ���`�悷��ʒu�B
//
//
//	//������@
//	DrawFuncData::DrawCallData m_guideUI;
//	KazBufferHelper::BufferData m_guideTex;
//	//�f�o�b�O�̖���
//	enum DEBUG_NAME {
//		RAYTRACING,
//		TIMEZONE,
//		SEA,
//		EXIT,
//	};
//
//public:
//
//	void Setting();
//
//	void Update();
//
//	void Draw(DrawingByRasterize& arg_rasterize, float arg_sliderRate);
//
//private:
//
//	void Input();
//
//};