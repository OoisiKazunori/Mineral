#include "Tutorial.h"
#include "../KazLibrary/Input/KeyBoradInputManager.h"
#include"../Game/TitleFlag.h"
#include "../Game/Wave/WaveMgr.h"

void Tutorial::setting()
{
	//�e�N�X�`���̃��[�h
	tutorial_tex[0].Load("Resource/TutorialTex/UI_Tutorial1.png");
	tutorial_tex[1].Load("Resource/TutorialTex/UI_Tutorial2.png");
	tutorial_tex[2].Load("Resource/TutorialTex/UI_Tutorial3.png");
	tutorial_tex[3].Load("Resource/TutorialTex/UI_Tutorial4.png");
	tutorial_tex[4].Load("Resource/TutorialTex/UI_Tutorial5.png");
	tutorial_tex[5].Load("Resource/TutorialTex/UI_Tutorial6.png");
	tutorial_tex[6].Load("Resource/TutorialTex/UI_Tutorial7.png");
	tutorial_tex[7].Load("Resource/TutorialTex/UI_Tutorial8.png");
	tutorial_tex[8].Load("Resource/TutorialTex/UI_Tutorial9.png");
	tutorial_tex[9].Load("Resource/TutorialTex/UI_Tutorial10.png");
	tutorial_tex[10].Load("Resource/TutorialTex/UI_Tutorial11.png");
	tutorial_tex[11].Load("Resource/TutorialTex/UI_Tutorial12.png");

	is_tutorial = true;
	tutorial_num = 0;
	tex_transform.pos = { 1280.0f / 2.0f, 720.0f / 2.0f };
	tex_transform.scale = { 1280.0f, 720.0f };
	move_amount = 0.0f;
	spawn_minekuzi = false;
	spawn_m_rock = false;
	spawn_rock = false;
	break_mineral = false;
	draw_timer = 0;
	tree_carry_count = 0;
	tutorial_timer = 0;
	TUTORIAL_TIME_MAX = 400.0f;
	for (int i = 0; i < static_cast<int>(tutorial_tex.size()); i++)
	{
		tutorial_tex[i].m_transform = tex_transform;
		if (i != 0)
		{
			tutorial_tex[i].m_transform.pos.y = -1000.0f;
		}
	}
}

void Tutorial::Update()
{
	if (!TitleFlag::Instance()->m_isTitle)
	{
		//�摜�𓮂�������
		for (int i = 0; i < static_cast<int>(tutorial_tex.size()); i++)
		{
			float tex_base_y = -200.0f;
			if (tutorial_num == i)
			{
				tex_base_y = 720.0f / 2.0f;
			}
			tutorial_tex[i].m_transform.pos.y += (tex_base_y - tutorial_tex[i].m_transform.pos.y) / 20.0f;
		}

		//���Ԍo�߂Ń`���[�g���A�����΂��Ƃ��̏���
		if (tutorial_num == 0 || tutorial_num == 4 || tutorial_num == 6 || tutorial_num == 11)
		{
			if (tutorial_timer <= TUTORIAL_TIME_MAX)
			{
				tutorial_timer++;
			}

			else
			{
				is_next = true;
				tutorial_timer = 0;
			}
		}

		if (tutorial_num == 7)
		{
			if (draw_timer < DRAW_TIME_MAX)
			{
				draw_timer++;
			}

			else
			{
				//�����~�l�����������Ă����玟�̃t�F�[�Y�ɔ�΂�
				if (break_mineral)
				{
					is_next = true;
				}
			}
		}

		if (tutorial_num == 9)
		{
			if (tree_carry_count >= 3)
			{
				is_next = true;
			}
		}

		//���̃`���[�g���A���ɔ�΂�
		if (is_next)
		{
			tutorial_num++;
			tutorial_timer = 0;
			switch (tutorial_num)
			{
			case 4:
				TUTORIAL_TIME_MAX = 400.0f;
				break;

			case 6:
				TUTORIAL_TIME_MAX = 400.0f;
				break;

			case 11:
				TUTORIAL_TIME_MAX = 300.0f;
				break;

			default:
				TUTORIAL_TIME_MAX = 100.0f;
				break;
			}

			//�~�l�N�W��N������
			if (spawn_minekuzi)
			{
				spawn_minekuzi = false;
			}
			//�~�l��������킩����
			if (spawn_m_rock)
			{
				spawn_m_rock = false;
			}
			//����킩����
			if (spawn_rock)
			{
				spawn_rock = false;
			}
			//�؂��킩����
			if (spawn_tree)
			{
				spawn_tree = false;
			}

			is_next = false;

			if (tutorial_num == 12)
			{
				is_tutorial = false;
			}
		}
	}
}

void Tutorial::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{
	if (!TitleFlag::Instance()->m_isTitle)
	{
		for (int i = 0; i < static_cast<int>(tutorial_tex.size()); i++)
		{
			tutorial_tex[i].Draw(arg_rasterize);
		}
	}
}
