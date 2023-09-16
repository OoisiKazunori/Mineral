#pragma once
#include "../Game/DrawCallSet.h"

struct CollisionVertex {
	KazMath::Vec3<float> m_pos;
	KazMath::Vec3<float> m_normal;
};

struct MeshData {
	bool m_isActive;
	CollisionVertex m_vertexA;
	CollisionVertex m_vertexB;
	CollisionVertex m_vertexC;
};

class MeshCollision {

private:

	VertexData m_defaultVertexData;
	std::vector<MeshData> m_collisionMeshData;

public:

	struct CheckHitResult {
		bool m_isHit;
		float m_distance;
		KazMath::Vec3<float> m_normal;
		KazMath::Vec3<float> m_position;
	};

public:

	void Setting(VertexData arg_defaultVertexdata, KazMath::Transform3D arg_transform);

	void Transform(KazMath::Transform3D arg_transform);

	CheckHitResult CheckHitRay(KazMath::Vec3<float> arg_origin, KazMath::Vec3<float> arg_direction);

private:

	void BuildCollisionMesh(VertexData arg_defaultVertexData);

	KazMath::Vec3<float> TransformVec3(KazMath::Vec3<float> arg_value, DirectX::XMVECTOR arg_quaternion);
	KazMath::Vec3<float> TransformVec3(KazMath::Vec3<float> arg_value, DirectX::XMMATRIX arg_mat);

};