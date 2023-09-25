#include "Core.h"
#include "../KazLibrary/Easing/easing.h"
#include "../Game/Wave/WaveMgr.h"
#include "../Game/TitleFlag.h"
#include "../Game/Tutorial.h"
#include "../KazLibrary/Input/KeyBoradInputManager.h"

Core::Core()
{

	m_model.LoadOutline("Resource/Core/", "Core.gltf");
	m_crownModel.LoadOutline("Resource/Core/", "Crown.gltf");
	m_hpBoxModel.LoadNoLighting("Resource/HpBox/", "Hp_Box.gltf");
	m_crownPos = KazMath::Vec3<float>(0.6f, 17.6f, -5.2f);
	m_hp = MAX_HP;
	wall_break01 = SoundManager::Instance()->SoundLoadWave("Resource/Sound/break.wav");
	wall_break01.volume = 0.1f;
	wall_break02 = SoundManager::Instance()->SoundLoadWave("Resource/Sound/rock.wav");
	wall_break02.volume = 0.1f;
	damage = SoundManager::Instance()->SoundLoadWave("Resource/Sound/Object_Damage.wav");
	damage.volume = 0.04f;
	m_coreDamageUI.Load("Resource/UI/CoreDamageUI.png");
	m_coreDamageUI.m_transform.pos = { 1280.0f / 2.0f, 720.0f / 2.0f };
	m_coreDamageUI.m_transform.scale = { 375.0f, 200.0f };

}

void Core::Init()
{

	m_hp = MAX_HP;
	m_uiAppearCount = 0;
	m_damageUITimer = 0.0f;
	m_uiState = UISTATE::APPEAR;
	m_isActiveDamageUI = false;
	m_coreDamageUI.m_color.color.a = 0;
	m_damageUIAgainTimer = 0;

	/*オカモトゾーン*/
	isDrawHpBox = false;
	hpBoxScaleStart = 0.0f;
	hpBoxScaleEnd = 0.0f;
	m_hpBoxDrawTimer = HP_BOX_DRAW_TIME_MAX;
	hpBoxEaseTime = HP_BOX_EASE_TIME_MAX;
	ease_scale = 0.0f;
	m_hpBoxTransform.scale.y = 2.0f;
	m_hpBoxTransform.scale.z = 2.0f;
	m_hpBoxTransform.scale.x = m_hp * 1.5f;
	/*オカモトゾーン*/
}

void Core::Update()
{

	m_transform.scale = { SCALE ,SCALE ,SCALE };
	m_transform.rotation.y = 270.0f;

	//昼のとき
	if (!WaveMgr::Instance()->GetIsNight())
	{
		m_hp = MAX_HP;
	}


	//「コアがダメージを受けています」の表示の更新
	if (m_isActiveDamageUI) {

		switch (m_uiState)
		{
		case Core::UISTATE::APPEAR:
		{

			//タイマーを更新。
			m_damageUITimer = std::clamp(m_damageUITimer + 1.0f, 0.0f, APPEAR_TIME);

			//イージングを計算。
			float easingAmount = EasingMaker(Out, Exp, m_damageUITimer / APPEAR_TIME);
			m_coreDamageUI.m_color.color.a = static_cast<int>(255 * easingAmount);

			//タイマーが規定値を超えていたら。
			if (APPEAR_TIME <= m_damageUITimer) {

				m_damageUITimer = 0.0f;
				m_uiState = UISTATE::EXIT;

			}


		}
		break;
		case Core::UISTATE::EXIT:
		{

			//タイマーを更新。
			m_damageUITimer = std::clamp(m_damageUITimer + 1.0f, 0.0f, EXIT_TIME);

			//イージングを計算。
			float easingAmount = EasingMaker(In, Quart, m_damageUITimer / EXIT_TIME);
			m_coreDamageUI.m_color.color.a = static_cast<int>(255 - 255 * easingAmount);

			//タイマーが規定値を超えていたら。
			if (EXIT_TIME <= m_damageUITimer) {

				m_damageUITimer = 0.0f;
				m_uiState = UISTATE::APPEAR;
				++m_uiAppearCount;
				if (UI_APPEAR_COUNT <= m_uiAppearCount) {

					m_isActiveDamageUI = false;

				}

			}


		}
		break;
		default:
			break;
		}


	}

	m_damageUIAgainTimer = std::clamp(m_damageUIAgainTimer - 1, 0, DAMAGEUI_AGAIN_TIMER);


	/*オカモトゾーン*/
	if (isDrawHpBox)
	{
		if (m_hpBoxDrawTimer >= 0)
		{
			m_hpBoxDrawTimer--;
			//スケールのイージング
			if (hpBoxEaseTime > 0)
			{
				ease_scale = hpBoxScaleStart - (1.0f - EasingMaker(In, Circ, hpBoxEaseTime / HP_BOX_EASE_TIME_MAX));
				ease_scale *= 1.5f;
				m_hpBoxTransform.scale.x = ease_scale;
				hpBoxEaseTime--;
			}

			else
			{
				m_hpBoxTransform.scale.x = hpBoxScaleEnd * 1.5f;
			}
		}
		else
		{
			isDrawHpBox = false;
		}
	}
	/*オカモトゾーン*/
}

void Core::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{
	DessolveOutline outline;
	outline.m_outline = KazMath::Vec4<float>(0.1f, 0, 0, 1);
	m_model.m_model.extraBufferArray[4].bufferWrapper->TransData(&outline, sizeof(DessolveOutline));

	m_model.m_model.extraBufferArray.back() = GBufferMgr::Instance()->m_outlineBuffer;
	m_model.m_model.extraBufferArray.back().rangeType = GRAPHICS_RANGE_TYPE_UAV_DESC;
	m_model.m_model.extraBufferArray.back().rootParamType = GRAPHICS_PRAMTYPE_TEX;

	m_model.Draw(arg_rasterize, arg_blasVec, m_transform);

	outline.m_outline = KazMath::Vec4<float>(0.1f, 0, 0, 1);
	m_crownModel.m_model.extraBufferArray[4].bufferWrapper->TransData(&outline, sizeof(DessolveOutline));

	m_crownModel.m_model.extraBufferArray.back() = GBufferMgr::Instance()->m_outlineBuffer;
	m_crownModel.m_model.extraBufferArray.back().rangeType = GRAPHICS_RANGE_TYPE_UAV_DESC;
	m_crownModel.m_model.extraBufferArray.back().rootParamType = GRAPHICS_PRAMTYPE_TEX;

	KazMath::Transform3D crownTransform = m_transform;
	crownTransform.pos += m_crownPos;
	m_crownModel.Draw(arg_rasterize, arg_blasVec, crownTransform);

	m_coreDamageUI.Draw(arg_rasterize);

	if (!TitleFlag::Instance()->m_isTitle && !Tutorial::Instance()->is_tutorial) {

		/*オカモトゾーン*/
		m_hpBoxTransform.pos = m_transform.pos;
		m_hpBoxTransform.pos.y += 30.0f;
		m_hpBoxTransform.rotation.y = 45.0f;

		m_hpBoxModel.Draw(arg_rasterize, arg_blasVec, m_hpBoxTransform, 0, false);
		/*オカモトゾーン*/

	}

}

void Core::Damage(int arg_damage)
{
	SoundManager::Instance()->SoundPlayerWave(damage, 0);
	/*オカモトゾーン*/
	isDrawHpBox = true;
	m_hpBoxDrawTimer = HP_BOX_DRAW_TIME_MAX;
	hpBoxScaleStart = static_cast <float>(m_hp);
	hpBoxScaleEnd = static_cast<float> (m_hp - arg_damage);
	hpBoxEaseTime = HP_BOX_EASE_TIME_MAX;
	/*オカモトゾーン*/

	m_hp = std::clamp(m_hp - arg_damage, 0, MAX_HP);

	if (!m_isActiveDamageUI && m_damageUIAgainTimer <= 0) {

		m_isActiveDamageUI = true;
		m_uiState = UISTATE::APPEAR;
		m_damageUITimer = 0.0f;
		m_uiAppearCount = 0;

		m_damageUIAgainTimer = DAMAGEUI_AGAIN_TIMER;

	}
}
