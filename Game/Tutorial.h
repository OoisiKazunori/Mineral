#pragma once
#include "../Game/DrawCallSet.h"
#include "../Game/DrawCallUISet.h"
#include "../KazLibrary/Helper/ISinglton.h"

class Tutorial : public ISingleton<Tutorial> {

private:


public:

	bool is_next;
	bool is_tutorial;
	int tutorial_num;
	std::array<DrawCallUISet, 12> tutorial_tex;
	KazMath::Transform2D tex_transform;
	//チュートリアルを飛ばす条件に使う変数たち
	float move_amount;
	float tutorial_timer;
	float TUTORIAL_TIME_MAX;
	bool spawn_minekuzi;
	bool spawn_m_rock;
	bool spawn_rock;
	bool break_mineral;
	bool spawn_tree;
	int tree_carry_count;
	float draw_timer;
	float DRAW_TIME_MAX = 120.0f;

	DrawCallSet m_tutorialArrow;

	void setting();

	void Update();

	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);


};
