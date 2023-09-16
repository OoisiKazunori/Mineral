#pragma once
#include "../Game/DrawCallSet.h"
#include "../KazLibrary/Helper/ISinglton.h"
#include "../Game/Collision/MeshCollision.h"

class StageCollision : public ISingleton <StageCollision> {

public:

	MeshCollision m_stageCollision;

public:

	void Setting(std::string arg_filePath, std::string arg_fileName, KazMath::Transform3D arg_transform);

};