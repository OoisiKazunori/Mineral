#include "EnemyRoute.h"

void EnemyRoute::Setting()
{

	m_routes.resize(static_cast<int>(ROUTE_ID::MAX));

	//ルートAを設定。
	std::vector<KazMath::Vec3<float>> route = { KazMath::Vec3<float>(-103.0f, 0.0f, 277.0f), KazMath::Vec3<float>(14.0f, 0.0f, 11.0f), KazMath::Vec3<float>(87.0f, 0.0f, 98.0f), KazMath::Vec3<float>(167.0f, 0.0f, 104.0f) };
	m_routes[static_cast<int>(ROUTE_ID::A)] = route;

	//ルートBを設定。
	route = { KazMath::Vec3<float>(-295.0f, 0.0f, 212.0f), KazMath::Vec3<float>(-242.0f, 0.0f, 56.0f), KazMath::Vec3<float>(-221.0f, 0.0f, -119.0f), KazMath::Vec3<float>(-11.0f, 0.0f, -137.0f), KazMath::Vec3<float>(11.0f, 0.0f, 9.0f), KazMath::Vec3<float>(98.0f, 0.0f, 112.0f), KazMath::Vec3<float>(167.0f, 0.0f, 104.0f) };
	m_routes[static_cast<int>(ROUTE_ID::B)] = route;

	//ルートCを設定。
	route = { KazMath::Vec3<float>(-128.0f, 0.0f, -462.0f), KazMath::Vec3<float>(-313.0f, 0.0f, -222.0f), KazMath::Vec3<float>(-190.0f, 0.0f, -166.0f), KazMath::Vec3<float>(-17.0f, 0.0f, -124.0f), KazMath::Vec3<float>(14.0f, 0.0f, 14.0f), KazMath::Vec3<float>(92.0f, 0.0f, 107.0f), KazMath::Vec3<float>(167.0f, 0.0f, 104.0f) };
	m_routes[static_cast<int>(ROUTE_ID::C)] = route;

}

std::vector<KazMath::Vec3<float>> EnemyRoute::GetRoute(ROUTE_ID arg_id)
{
	return m_routes[static_cast<int>(arg_id)];
}
