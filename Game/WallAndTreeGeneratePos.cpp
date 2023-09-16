#include "WallAndTreeGeneratePos.h"

void WallAndTreeGeneratePos::Setting()
{

	std::vector<KazMath::Vec3<float>> trees = { KazMath::Vec3<float>(131.0f, 35.0f, 160.0f), KazMath::Vec3<float>(188.0f, 35.0f, 33.0f), KazMath::Vec3<float>(136.0f, 35.0f, -74.0f), KazMath::Vec3<float>(7.0f, 35.0f, -207.0f), KazMath::Vec3<float>(-230.0f, 35.0f, 73.0f), KazMath::Vec3<float>(-387.0f, 35.0f, -124.0f), KazMath::Vec3<float>(-495.0f, 35.0f, -220.0f), KazMath::Vec3<float>(-372.0f, 35.0f, -329.0f), KazMath::Vec3<float>(-202.0f, 35.0f, -297.0f) };
	m_treePos = trees;
	m_treeCount = static_cast<int>(m_treePos.size());

	std::vector<KazMath::Vec3<float>> rocks = { KazMath::Vec3<float>(186.0f, 0.0f, 152.0f), KazMath::Vec3<float>(147.0f, 0.0f, 38.0f), KazMath::Vec3<float>(51.0f, 0.0f, -54.0f), KazMath::Vec3<float>(-221.0f, 0.0f, -72.0f), KazMath::Vec3<float>(-458.0f, 0.0f, -150.0f), KazMath::Vec3<float>(-457.0f, 0.0f, -299.0f), KazMath::Vec3<float>(-232.0f, 0.0f, -392.0f) };
	m_rockPos = rocks;
	m_rockCount = static_cast<int>(m_rockPos.size());

}
