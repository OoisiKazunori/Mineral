#pragma once
#include "../Game/DrawCallSet.h"
#include "../KazLibrary/Helper/ISinglton.h"

class EnemyRoute : public ISingleton<EnemyRoute> {

private:

	std::vector<std::vector<KazMath::Vec3<float>>> m_routes;

public:

	enum ROUTE_ID {
		A,
		B,
		C,
		MAX,
	};

public:

	void Setting();

	std::vector<KazMath::Vec3<float>> GetRoute(ROUTE_ID arg_id);

};