#include "EnemyRoute.h"

void EnemyRoute::Setting()
{

	m_routes.resize(static_cast<int>(ROUTE_ID::MAX));

	//ルートAを設定。
	std::vector<KazMath::Vec3<float>> route;
	route.emplace_back(KazMath::Vec3<float>(-87.0f, 0.0f, 207.0f));
	route.emplace_back(KazMath::Vec3<float>(19.0f, 0.0f, -21.0f));
	route.emplace_back(KazMath::Vec3<float>(90.0f, 0.0f, 119.0f));
	route.emplace_back(KazMath::Vec3<float>(170.0f, 0.0f, 106.0f));
	m_routes[static_cast<int>(ROUTE_ID::A)] = route;

	//ルートBを設定。
	route = {};
	route.emplace_back(KazMath::Vec3<float>(-266.0f, 0.0f, 113.0f));
	route.emplace_back(KazMath::Vec3<float>(-260.0f, 0.0f, -176.0f));
	route.emplace_back(KazMath::Vec3<float>(-23.0f, 0.0f, -154.0f));
	route.emplace_back(KazMath::Vec3<float>(9.0f, 0.0f, -4.0f));
	route.emplace_back(KazMath::Vec3<float>(90.0f, 0.0f, 119.0f));
	route.emplace_back(KazMath::Vec3<float>(170.0f, 0.0f, 106.0f));
	m_routes[static_cast<int>(ROUTE_ID::B)] = route;

	//ルートCを設定。
	route = {};
	route.emplace_back(KazMath::Vec3<float>(-201.0f, 0.0f, -363.0f));
	route.emplace_back(KazMath::Vec3<float>(-320.0f, 0.0f, -204.0f));
	route.emplace_back(KazMath::Vec3<float>(-227.0f, 0.0f, -134.0f));
	route.emplace_back(KazMath::Vec3<float>(-53.0f, 0.0f, -164.0f));
	route.emplace_back(KazMath::Vec3<float>(15.0f, 0.0f, 0.0f));
	route.emplace_back(KazMath::Vec3<float>(9.0f, 0.0f, -3.0f));
	route.emplace_back(KazMath::Vec3<float>(90.0f, 0.0f, 119.0f));
	route.emplace_back(KazMath::Vec3<float>(170.0f, 0.0f, 106.0f));
	m_routes[static_cast<int>(ROUTE_ID::C)] = route;

}

std::vector<KazMath::Vec3<float>> EnemyRoute::GetRoute(ROUTE_ID arg_id)
{
	return m_routes[static_cast<int>(arg_id)];
}
