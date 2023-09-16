#pragma once
#include "DrawCallSet.h"
#include "../KazLibrary/Helper/ISinglton.h"

class WallAndTreeGeneratePos : public ISingleton<WallAndTreeGeneratePos> {

public:

	std::vector<KazMath::Vec3<float>> m_treePos;
	std::vector<KazMath::Vec3<float>> m_rockPos;

	int m_treeCount;	//¶¬ˆÊ’u‚Ì”
	int m_rockCount;	//¶¬ˆÊ’u‚Ì”

public:

	void Setting();

};