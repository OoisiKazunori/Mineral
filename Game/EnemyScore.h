#pragma once
#include "../KazLibrary/Helper/ISinglton.h"

class EnemyScore : public ISingleton<EnemyScore>{

public:

	int m_score;

};