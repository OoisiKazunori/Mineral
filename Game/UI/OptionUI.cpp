//#include "OptionUI.h"
//#include "../KazLibrary/Loader/TextureResourceMgr.h"
//#include "../KazLibrary/Render/DrawFunc.h"
//#include "../KazLibrary/Input/ControllerInputManager.h"
//#include "../KazLibrary/Input/KeyBoradInputManager.h"
//#include <Imgui/imgui.h>
//#include "../Effect/TimeZone.h"
//#include "../Effect/SeaEffect.h"
//#include "../KazLibrary/Easing/easing.h"
//
//void OptionUI::Setting()
//{
//
//	//�t�H���g�����[�h
//	int counter = 97; //97��"a"
//	std::string path = "Resource/UI/Font/";
//	std::string extension = ".png";
//	for (auto& index : m_font) {
//
//		//���[�h����t�@�C���̕�������擾�B
//		std::string filePath = path;
//		filePath.push_back(static_cast<char>(counter));
//		filePath += extension;
//
//		//�t�@�C�������[�h
//		index = TextureResourceMgr::Instance()->LoadGraphBuffer(filePath);
//
//		++counter;
//	}
//
//	//�����o����ݒ�B
//	m_headlines.emplace_back(OptionHeadline("RAYTRACING", KazMath::Vec2<float>(0, 0), 32.0f, RAYTRACING));
//	m_headlines.emplace_back(OptionHeadline("TIMEZONE", KazMath::Vec2<float>(0, 0), 32.0f, TIMEZONE));
//	m_headlines.emplace_back(OptionHeadline("SEA", KazMath::Vec2<float>(0, 0), 32.0f, SEA));
//	m_headlines.emplace_back(OptionHeadline("EXIT", KazMath::Vec2<float>(0, 0), 32.0f, EXIT));
//	m_optionUI.emplace_back(OptionHeadline("OPTION", KazMath::Vec2<float>(0, 0), OPTION_FONTSIZE, 0));
//
//	//�I�v�V�����ڍׂ�ݒ�B
//	m_optionDetails.emplace_back(OptionDetails("DEBUG", { DrawStringData("ON"),DrawStringData("OFF") }, KazMath::Vec2<float>(), RAYTRACING));
//	m_optionDetails.emplace_back(OptionDetails("TIME", { DrawStringData("EVENING"),DrawStringData("NOON") }, KazMath::Vec2<float>(), TIMEZONE));
//	m_optionDetails.emplace_back(OptionDetails("STATE", { DrawStringData("A"),DrawStringData("B"),DrawStringData("C") }, KazMath::Vec2<float>(), SEA));
//	m_optionDetails.emplace_back(OptionDetails("", { DrawStringData("") }, KazMath::Vec2<float>(), EXIT));
//
//	//�w�i�����[�h
//	m_backGroundTexture = TextureResourceMgr::Instance()->LoadGraphBuffer("Resource/UI/white.png");
//	m_backGroundColor = KazMath::Color(0, 0, 0, 0);
//	m_backGroundRender = DrawFuncData::SetSpriteAlphaData(DrawFuncData::GetSpriteAlphaShader());
//	m_backGroundEasingTimer = 0;
//
//	//�������[�h
//	m_rightArrowTexture = TextureResourceMgr::Instance()->LoadGraphBuffer("Resource/UI/rightArrow.png");
//	m_leftArrowTexture = TextureResourceMgr::Instance()->LoadGraphBuffer("Resource/UI/leftArrow.png");
//	m_rightArrowRender = DrawFuncData::SetSpriteAlphaData(DrawFuncData::GetSpriteAlphaShader());
//	m_leftArrowRender = DrawFuncData::SetSpriteAlphaData(DrawFuncData::GetSpriteAlphaShader());
//
//	//OnOff�؂�ւ����C��
//	m_debugOnOffLineRender = DrawFuncData::SetSpriteAlphaData(DrawFuncData::GetSpriteAlphaShader());
//	m_debugOnOffLineBuffer = TextureResourceMgr::Instance()->LoadGraphBuffer("Resource/UI/DebugOnOffLine.png");
//	m_debugOnOffLineStayBuffer = TextureResourceMgr::Instance()->LoadGraphBuffer("Resource/UI/DebugOnOffLineStay.png");
//
//	//����K�C�h��UI
//	m_guideUI = DrawFuncData::SetSpriteAlphaData(DrawFuncData::GetSpriteAlphaShader());
//	m_guideTex = TextureResourceMgr::Instance()->LoadGraphBuffer("Resource/UI/Guide/GuideMenu.png");
//
//	for (auto& index : m_onRender) {
//		index = DrawFuncData::SetSpriteAlphaData(DrawFuncData::GetSpriteAlphaShader());
//	}
//	for (auto& index : m_offRender) {
//		index = DrawFuncData::SetSpriteAlphaData(DrawFuncData::GetSpriteAlphaShader());
//	}
//	m_onString = "ON";
//	m_offString = "OFF";
//
//	//�e�ϐ��̏����ݒ�
//	m_nowSelectHeadline = 0;
//	m_prevInputUp = false;
//	m_prevInputDown = false;
//	m_prevInputRight = false;
//	m_prevInputLeft = false;
//	m_isDisplayUI = false;
//	m_isChangeDisplayUI = false;
//	m_isRaytracingDebug = false;
//	m_isExit = false;
//
//
//	m_doneSE = SoundManager::Instance()->SoundLoadWave("Resource/Sound/done.wav");
//	m_doneSE.volume = 0.01f;
//
//	m_cancelSE = SoundManager::Instance()->SoundLoadWave("Resource/Sound/cancel.wav");
//	m_cancelSE.volume = 0.01f;
//
//	m_selectSE = SoundManager::Instance()->SoundLoadWave("Resource/Sound/select.wav");
//	m_selectSE.volume = 0.01f;
//
//	m_optionDetails[RAYTRACING].m_selectID = 1;
//}
//
//void OptionUI::Update()
//{
//
//	//���͏���
//	Input();
//
//	//�I�����Ă���l�ɂ���ăf�o�b�O��؂�ւ���B
//	switch (m_nowSelectHeadline) {
//	case RAYTRACING:
//	{
//
//		//�I�����Ă���ڍׂ�ID�𔽉f�B
//		m_isRaytracingDebug = !m_optionDetails[RAYTRACING].m_selectID;
//
//		break;
//	}
//	case TIMEZONE:
//	{
//
//		//�I�����Ă���ڍׂ�ID�𔽉f�B
//		TimeZone::Instance()->ChangeTimeZone(!m_optionDetails[TIMEZONE].m_selectID);
//
//		break;
//	}
//	case SEA:
//	{
//
//		//�I�����Ă���ڍׂ�ID�𔽉f�B
//		SeaEffect::Instance()->ChangeSeaEffect(m_optionDetails[SEA].m_selectID);
//
//		break;
//	}
//	case EXIT:
//	{
//
//		//EXIT���I�΂�Ă����ԁI
//
//		break;
//	}
//	default:
//		break;
//	}
//
//	//�\����Ԃ�������
//	if (m_isDisplayUI) {
//		//�\����؂�ւ����Ԃ�������
//		if (m_isChangeDisplayUI) {
//
//			//�w�i�̃A���t�@���X�V�B
//			m_backGroundEasingTimer = std::clamp(m_backGroundEasingTimer + 1.0f, 0.0f, BACK_GROUND_EASING_TIMER);
//			float backGroundAlphaEasing = EasingMaker(Out, Cubic, m_backGroundEasingTimer / BACK_GROUND_EASING_TIMER);
//			m_backGroundColor.color.a = BACK_GROUND_ALPHA - static_cast<int>(BACK_GROUND_ALPHA * backGroundAlphaEasing);
//
//			//���ׂẴC�[�W���O���I�������B
//			if (BACK_GROUND_EASING_TIMER <= m_backGroundEasingTimer) {
//
//				m_isDisplayUI = false;
//				m_isChangeDisplayUI = false;
//				m_backGroundEasingTimer = 0;
//
//			}
//
//		}
//
//	}
//	else {
//
//		//�\����؂�ւ����Ԃ�������
//		if (m_isChangeDisplayUI) {
//
//			//�w�i�̃A���t�@���X�V�B
//			m_backGroundEasingTimer = std::clamp(m_backGroundEasingTimer + 1.0f, 0.0f, BACK_GROUND_EASING_TIMER);
//			float backGroundAlphaEasing = EasingMaker(Out, Cubic, m_backGroundEasingTimer / BACK_GROUND_EASING_TIMER);
//			m_backGroundColor.color.a = static_cast<int>(BACK_GROUND_ALPHA * backGroundAlphaEasing);
//
//			//���ׂẴC�[�W���O���I�������B
//			if (BACK_GROUND_EASING_TIMER <= m_backGroundEasingTimer) {
//
//				m_isDisplayUI = true;
//				m_isChangeDisplayUI = false;
//				m_backGroundEasingTimer = 0;
//
//			}
//
//		}
//
//
//	}
//
//
//}
//
//void OptionUI::Draw(DrawingByRasterize& arg_rasterize, float arg_sliderRate)
//{
//
//	//ImGui::Begin("UI");
//
//	//ImGui::End();
//
//
//	const int ASCII_A = 65;	//"A"��ASCII�R�[�h
//	const int ASCII_Z = 90;	//"Z"��ASCII�R�[�h
//
//	//OnOff���C����`��
//	if (m_isRaytracingDebug) {
//
//		if (m_isDisplayUI) {
//
//			m_debugOnOffLineTransform.scale.x = 2.0f;
//		}
//		else {
//
//			m_debugOnOffLineTransform.scale.x = 2.0f;
//		}
//
//		m_debugOnOffLineTransform.pos.x = arg_sliderRate;
//		m_debugOnOffLineTransform.pos.y = 720.0f / 2.0f;
//		m_debugOnOffLineTransform.scale.y = 720.0f;
//
//		//�F��ݒ�
//		KazMath::Color color = KazMath::Color(255, 255, 255, static_cast<int>(255));
//
//		DrawFunc::DrawTextureIn2D(m_debugOnOffLineRender, m_debugOnOffLineTransform, m_debugOnOffLineStayBuffer, color);
//
//		arg_rasterize.ObjectRender(m_debugOnOffLineRender);
//
//
//		//ON
//		{
//			const int CHARA_COUNT = static_cast<int>(m_onString.size());
//			for (int index = 0; index < CHARA_COUNT; ++index) {
//
//				//���̕�����Font�̔ԍ��𒲂ׂ�B
//				int fontNum = static_cast<int>(m_onString[index]) - ASCII_A;
//
//				//�t�H���g��������l�𒴂��Ă������΂��B
//				if (fontNum < 0 || static_cast<int>(m_font.size()) <= fontNum) continue;
//
//				KazMath::Color color = KazMath::Color(255, 255, 255, 255);
//
//				//�g�����X�t�H�[����p�ӁB
//				KazMath::Transform2D transform;
//				transform.pos = KazMath::Vec2<float>(1280.0f - OPTION_FONTSIZE * 2.0f, OPTION_FONTSIZE);
//				transform.pos.x += OPTION_FONTSIZE * (index);
//				transform.scale.x = OPTION_FONTSIZE;
//				transform.scale.y = OPTION_FONTSIZE;
//				DrawFunc::DrawTextureIn2D(m_onRender[index], transform, m_font[fontNum], color);
//				arg_rasterize.ObjectRender(m_onRender[index]);
//
//
//			}
//		}
//		//OFF
//		{
//			const int CHARA_COUNT = static_cast<int>(m_offString.size());
//			for (int index = 0; index < CHARA_COUNT; ++index) {
//
//				//���̕�����Font�̔ԍ��𒲂ׂ�B
//				int fontNum = static_cast<int>(m_offString[index]) - ASCII_A;
//
//				//�t�H���g��������l�𒴂��Ă������΂��B
//				if (fontNum < 0 || static_cast<int>(m_font.size()) <= fontNum) continue;
//
//				KazMath::Color color = KazMath::Color(255, 255, 255, 255);
//
//				//�g�����X�t�H�[����p�ӁB
//				KazMath::Transform2D transform;
//				transform.pos = KazMath::Vec2<float>(OPTION_FONTSIZE * 1.0f, OPTION_FONTSIZE);
//				transform.pos.x += OPTION_FONTSIZE * (index);
//				transform.scale.x = OPTION_FONTSIZE;
//				transform.scale.y = OPTION_FONTSIZE;
//				DrawFunc::DrawTextureIn2D(m_offRender[index], transform, m_font[fontNum], color);
//				arg_rasterize.ObjectRender(m_offRender[index]);
//
//
//			}
//		}
//
//	}
//
//	//OPTION�̕�����`�悷��B
//	{
//		const int CHARA_COUNT = static_cast<int>(m_optionUI.front().m_name.m_string.size());
//		for (int index = 0; index < CHARA_COUNT; ++index) {
//
//			//���̕�����Font�̔ԍ��𒲂ׂ�B
//			int fontNum = static_cast<int>(m_optionUI.front().m_name.m_string[index]) - ASCII_A;
//
//			//�t�H���g��������l�𒴂��Ă������΂��B
//			if (fontNum < 0 || static_cast<int>(m_font.size()) <= fontNum) continue;
//
//			m_optionUI.front().m_name.m_color[index] = KazMath::Color(255, 255, 255, m_backGroundColor.color.a);
//
//			m_optionUI.front().m_fontSize = OPTION_FONTSIZE;
//
//			//�g�����X�t�H�[����p�ӁB
//			KazMath::Transform2D transform;
//			transform.pos = OPTION_BASEPOS;
//			transform.pos.x += (m_optionUI.front().m_fontSize / 2.0f) + (m_optionUI.front().m_fontSize * index);
//			transform.scale.x = m_optionUI.front().m_fontSize;
//			transform.scale.y = m_optionUI.front().m_fontSize;
//			DrawFunc::DrawTextureIn2D(m_optionUI.front().m_name.m_render[index], transform, m_font[fontNum], m_optionUI.front().m_name.m_color[index]);
//			arg_rasterize.ObjectRender(m_optionUI.front().m_name.m_render[index]);
//
//
//		}
//	}
//
//	//�����o����`��
//	for (auto& headline : m_headlines) {
//
//		const int CHARA_COUNT = static_cast<int>(headline.m_name.m_string.size());
//		for (int index = 0; index < CHARA_COUNT; ++index) {
//
//			//���̕�����Font�̔ԍ��𒲂ׂ�B
//			int fontNum = static_cast<int>(headline.m_name.m_string[index]) - ASCII_A;
//
//			//�t�H���g��������l�𒴂��Ă������΂��B
//			if (fontNum < 0 || static_cast<int>(m_font.size()) <= fontNum) continue;
//
//			headline.m_name.m_color[index] = KazMath::Color(255, 255, 255, m_backGroundColor.color.a);
//			//�I�𒆂���Ȃ�������F�𔖂�����B
//			if (headline.m_headlineID != m_nowSelectHeadline) {
//				headline.m_name.m_color[index] = KazMath::Color(50, 50, 50, m_backGroundColor.color.a);
//			}
//
//			//�g�p����t�H���g�̃T�C�Y�����߂�B
//			headline.m_fontSize = DEFAULT_FONTSIZE;
//			if (headline.m_headlineID == m_nowSelectHeadline) {
//				headline.m_fontSize = SELECT_FONTSIZE;
//			}
//
//			//�g�����X�t�H�[����p�ӁB
//			KazMath::Transform2D transform;
//			transform.pos = HEADLINE_BASEPOS;
//			transform.pos.x += (headline.m_fontSize / 2.0f) + (headline.m_fontSize * index);
//			transform.pos.y += headline.m_fontSize / 2.0f + (BETWEEN_LINES * headline.m_headlineID);
//			transform.scale.x = headline.m_fontSize;
//			transform.scale.y = headline.m_fontSize;
//			DrawFunc::DrawTextureIn2D(headline.m_name.m_render[index], transform, m_font[fontNum], headline.m_name.m_color[index]);
//			arg_rasterize.ObjectRender(headline.m_name.m_render[index]);
//
//
//		}
//
//	}
//
//	//�I�v�V�����̏ڍׂ�`��
//	const int CHARA_COUNT = static_cast<int>(m_optionDetails[m_nowSelectHeadline].m_name.m_string.size());
//	for (int index = 0; index < CHARA_COUNT; ++index) {
//
//		//���̕�����Font�̔ԍ��𒲂ׂ�B
//		int fontNum = static_cast<int>(m_optionDetails[m_nowSelectHeadline].m_name.m_string[index]) - ASCII_A;
//
//		//�t�H���g��������l�𒴂��Ă������΂��B
//		if (fontNum < 0 || static_cast<int>(m_font.size()) <= fontNum) continue;
//
//		m_optionDetails[m_nowSelectHeadline].m_name.m_color[index] = KazMath::Color(255, 255, 255, m_backGroundColor.color.a);
//
//		//�g�����X�t�H�[����p�ӁB
//		KazMath::Transform2D transform;
//		transform.pos = DETAIL_BASEPOS;
//		transform.pos.x += (DETAIL_FONTSIZE / 2.0f) + (DETAIL_FONTSIZE * index);
//		transform.pos.y += DETAIL_FONTSIZE / 2.0f + (BETWEEN_LINES * m_nowSelectHeadline);
//		transform.scale.x = DETAIL_FONTSIZE;
//		transform.scale.y = DETAIL_FONTSIZE;
//		DrawFunc::DrawTextureIn2D(m_optionDetails[m_nowSelectHeadline].m_name.m_render[index], transform, m_font[fontNum], m_optionDetails[m_nowSelectHeadline].m_name.m_color[index]);
//		arg_rasterize.ObjectRender(m_optionDetails[m_nowSelectHeadline].m_name.m_render[index]);
//
//	}
//	//���ݑI�𒆂̃I�v�V�����̏ڍׂ�`�悷��B
//	const int NOW_SELECT_DETAIL_ID = m_optionDetails[m_nowSelectHeadline].m_selectID;
//	const int DETAIL_CHARA_COUNT = static_cast<int>(m_optionDetails[m_nowSelectHeadline].m_selectName[NOW_SELECT_DETAIL_ID].m_string.size());
//	KazMath::Vec2<float> detailPos = KazMath::Vec2<float>();
//	for (int index = 0; index < DETAIL_CHARA_COUNT; ++index) {
//
//		//���̕�����Font�̔ԍ��𒲂ׂ�B
//		int fontNum = static_cast<int>(m_optionDetails[m_nowSelectHeadline].m_selectName[NOW_SELECT_DETAIL_ID].m_string[index]) - ASCII_A;
//
//		//�t�H���g��������l�𒴂��Ă������΂��B
//		if (fontNum < 0 || static_cast<int>(m_font.size()) <= fontNum) continue;
//
//		m_optionDetails[m_nowSelectHeadline].m_selectName[NOW_SELECT_DETAIL_ID].m_color[index] = KazMath::Color(255, 255, 255, m_backGroundColor.color.a);
//
//		//�g�����X�t�H�[����p�ӁB
//		KazMath::Transform2D transform;
//		transform.pos = DETAIL_BASEPOS;
//		transform.pos.x += (DETAIL_FONTSIZE / 2.0f) + (DETAIL_FONTSIZE * index) + DETAIL_FLAG_POS;
//		transform.pos.y += DETAIL_FONTSIZE / 2.0f + (BETWEEN_LINES * m_nowSelectHeadline);
//		transform.scale.x = DETAIL_FONTSIZE;
//		transform.scale.y = DETAIL_FONTSIZE;
//		DrawFunc::DrawTextureIn2D(m_optionDetails[m_nowSelectHeadline].m_selectName[NOW_SELECT_DETAIL_ID].m_render[index], transform, m_font[fontNum], m_optionDetails[m_nowSelectHeadline].m_selectName[NOW_SELECT_DETAIL_ID].m_color[index]);
//		arg_rasterize.ObjectRender(m_optionDetails[m_nowSelectHeadline].m_selectName[NOW_SELECT_DETAIL_ID].m_render[index]);
//
//		detailPos = transform.pos;
//
//	}
//
//	//����`�悷��B
//	if (m_nowSelectHeadline != EXIT) {
//		KazMath::Color color = KazMath::Color(255, 255, 255, m_backGroundColor.color.a);
//		KazMath::Transform2D transform;
//		transform.pos = detailPos;
//		transform.pos.x += DETAIL_FONTSIZE;
//		transform.scale.x = DETAIL_FONTSIZE;
//		transform.scale.y = DETAIL_FONTSIZE;
//		DrawFunc::DrawTextureIn2D(m_rightArrowRender, transform, m_rightArrowTexture, color);
//		arg_rasterize.ObjectRender(m_rightArrowRender);
//
//		transform.pos = detailPos;
//		transform.pos.x = DETAIL_BASEPOS.x + DETAIL_FLAG_POS;
//		transform.pos.x -= DETAIL_FONTSIZE / 2.0f;
//		transform.scale.x = DETAIL_FONTSIZE;
//		transform.scale.y = DETAIL_FONTSIZE;
//		DrawFunc::DrawTextureIn2D(m_leftArrowRender, transform, m_leftArrowTexture, color);
//		arg_rasterize.ObjectRender(m_leftArrowRender);
//
//	}
//
//	//����
//	if (m_isDisplayUI && !m_isChangeDisplayUI)
//	{
//		KazMath::Color color = KazMath::Color(255, 255, 255, 255);
//		KazMath::Transform2D transform;
//		transform.scale = KazMath::Vec2<float>(256.0f, 64.0f);
//		transform.pos = KazMath::Vec2<float>(1280.0f - transform.scale.x / 2.0f - 200.0f, 720.0f - transform.scale.y / 2.0f);
//		DrawFunc::DrawTextureIn2D(m_guideUI, transform, m_guideTex, color);
//		arg_rasterize.ObjectRender(m_guideUI);
//	}
//
//	//�w�i��`��
//	{
//		KazMath::Transform2D transform;
//		transform.pos = KazMath::Vec2<float>(1280.0f / 2.0f, 720.0f / 2.0f);
//		transform.scale = KazMath::Vec2<float>(1280.0f, 720.0f);
//		DrawFunc::DrawTextureIn2D(m_backGroundRender, transform, m_backGroundTexture, m_backGroundColor);
//		arg_rasterize.ObjectRender(m_backGroundRender);
//	}
//}
//
//void OptionUI::Input()
//{
//	//�X�^�[�g�{�^�������͂��ꂽ��B
//	if ((ControllerInputManager::Instance()->InputTrigger(XINPUT_GAMEPAD_START) || KeyBoradInputManager::Instance()->InputTrigger(DIK_ESCAPE)) && !m_isChangeDisplayUI) {
//		m_isChangeDisplayUI = true;
//
//		if (m_isDisplayUI) {
//			SoundManager::Instance()->SoundPlayerWave(m_cancelSE, 0);
//		}
//		else {
//			SoundManager::Instance()->SoundPlayerWave(m_doneSE, 0);
//		}
//
//	}
//
//	//UI���\������Ă��Ȃ���Ԃ���������͂�؂�B
//	if (!m_isDisplayUI) return;
//
//	const int DEADLINE = 30000;
//
//	//�������ɓ��͂��ꂽ��
//	bool isInputDown = ControllerInputManager::Instance()->InputStickState(ControllerStickSide::LEFT_STICK, ControllerSide::DOWN_SIDE, DEADLINE) ||
//		KeyBoradInputManager::Instance()->InputTrigger(DIK_DOWNARROW) ||
//		KeyBoradInputManager::Instance()->InputTrigger(DIK_S);
//	if (isInputDown && !m_prevInputDown) {
//
//		m_nowSelectHeadline = std::clamp(m_nowSelectHeadline + 1, 0, static_cast<int>(m_headlines.size()) - 1);
//
//		SoundManager::Instance()->SoundPlayerWave(m_selectSE, 0);
//
//	}
//	//������ɓ��͂��ꂽ��
//	bool isInputUp = ControllerInputManager::Instance()->InputStickState(ControllerStickSide::LEFT_STICK, ControllerSide::UP_SIDE, DEADLINE) ||
//		KeyBoradInputManager::Instance()->InputTrigger(DIK_UPARROW) ||
//		KeyBoradInputManager::Instance()->InputTrigger(DIK_W);
//	if (isInputUp && !m_prevInputUp) {
//
//		m_nowSelectHeadline = std::clamp(m_nowSelectHeadline - 1, 0, static_cast<int>(m_headlines.size()) - 1);
//
//		SoundManager::Instance()->SoundPlayerWave(m_selectSE, 0);
//
//	}
//
//	//�E�����ɓ��͂��ꂽ��
//	bool isInputRight = ControllerInputManager::Instance()->InputStickState(ControllerStickSide::LEFT_STICK, ControllerSide::RIGHT_SIDE, DEADLINE) ||
//		KeyBoradInputManager::Instance()->InputTrigger(DIK_RIGHTARROW) ||
//		KeyBoradInputManager::Instance()->InputTrigger(DIK_D);
//	if (isInputRight && !m_prevInputRight) {
//
//		++m_optionDetails[m_nowSelectHeadline].m_selectID;
//
//		SoundManager::Instance()->SoundPlayerWave(m_doneSE, 0);
//
//	}
//
//	//�������ɓ��͂��ꂽ��
//	bool isInputLeft = ControllerInputManager::Instance()->InputStickState(ControllerStickSide::LEFT_STICK, ControllerSide::LEFT_SIDE, DEADLINE) ||
//		KeyBoradInputManager::Instance()->InputTrigger(DIK_LEFTARROW) ||
//		KeyBoradInputManager::Instance()->InputTrigger(DIK_A);
//	if (isInputLeft && !m_prevInputLeft) {
//
//		--m_optionDetails[m_nowSelectHeadline].m_selectID;
//
//		SoundManager::Instance()->SoundPlayerWave(m_doneSE, 0);
//
//	}
//	m_optionDetails[m_nowSelectHeadline].m_selectID = std::clamp(m_optionDetails[m_nowSelectHeadline].m_selectID, 0, static_cast<int>(m_optionDetails[m_nowSelectHeadline].m_selectName.size()) - 1);
//
//	//���͂�ۑ��B
//	m_prevInputDown = isInputDown;
//	m_prevInputUp = isInputUp;
//	m_prevInputRight = isInputRight;
//	m_prevInputLeft = isInputLeft;
//
//	//EXIT��Ԃ�������B
//	m_isExit = false;
//	if (m_nowSelectHeadline == EXIT) {
//
//		bool isInput = KeyBoradInputManager::Instance()->InputState(DIK_SPACE);
//		isInput |= ControllerInputManager::Instance()->InputState(XINPUT_GAMEPAD_A);
//
//		m_isExit = isInput;
//
//	}
//}
//
//OptionUI::OptionHeadline::OptionHeadline(std::string arg_headline, KazMath::Vec2<float> arg_pos, float arg_fontSize, int arg_headlineID)
//	: m_pos(arg_pos), m_fontSize(arg_fontSize), m_headlineID(arg_headlineID)
//{
//
//	m_name.m_string = arg_headline;
//	for (auto& index : m_name.m_render) {
//		index = DrawFuncData::SetSpriteAlphaData(DrawFuncData::GetSpriteAlphaShader());
//	}
//	for (auto& index : m_name.m_color) {
//		index = KazMath::Color(0, 255, 0, 255);
//	}
//
//}
//
//OptionUI::OptionDetails::OptionDetails(std::string arg_name, std::vector<DrawStringData> arg_selectName, KazMath::Vec2<float> arg_pos, int arg_id)
//	: m_pos(arg_pos), m_id(arg_id)
//{
//
//	m_name.m_string = arg_name;
//	for (auto& index : m_name.m_render) {
//		index = DrawFuncData::SetSpriteAlphaData(DrawFuncData::GetSpriteAlphaShader());
//	}
//	for (auto& index : m_name.m_color) {
//		index = KazMath::Color(0, 255, 0, 255);
//	}
//
//	const int SELECT_COUNT = static_cast<int>(arg_selectName.size());
//	m_selectName.resize(SELECT_COUNT);
//	for (int index = 0; index < SELECT_COUNT; ++index) {
//		m_selectName[index].m_string = arg_selectName[index].m_string;
//		for (auto& index : m_selectName[index].m_render) {
//			index = DrawFuncData::SetSpriteAlphaData(DrawFuncData::GetSpriteAlphaShader());
//		}
//		for (auto& index : m_selectName[index].m_color) {
//			index = KazMath::Color(0, 255, 0, 255);
//		}
//	}
//
//	m_selectID = 0;
//
//}
