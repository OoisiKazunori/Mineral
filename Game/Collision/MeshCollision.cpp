#include "MeshCollision.h"

void MeshCollision::Setting(VertexData arg_defaultVertexdata, KazMath::Transform3D arg_transform)
{

	BuildCollisionMesh(arg_defaultVertexdata);
	Transform(arg_transform);

}

void MeshCollision::Transform(KazMath::Transform3D arg_transform)
{

	//���[���h�s��
	DirectX::XMMATRIX targetWorldMat = arg_transform.GetMat();
	//��]�𔲂����B
	DirectX::XMVECTOR rotate, scale, position;
	DirectX::XMMatrixDecompose(&scale, &rotate, &position, targetWorldMat);
	for (auto& index : m_collisionMeshData) {
		//���_��ϊ�
		index.m_vertexA.m_pos = TransformVec3(index.m_vertexA.m_pos, targetWorldMat);
		index.m_vertexB.m_pos = TransformVec3(index.m_vertexB.m_pos, targetWorldMat);
		index.m_vertexC.m_pos = TransformVec3(index.m_vertexC.m_pos, targetWorldMat);
		//�@������]�s�񕪂����ϊ�
		index.m_vertexA.m_normal = TransformVec3(index.m_vertexA.m_normal, rotate);
		index.m_vertexA.m_normal.Normalize();
		index.m_vertexB.m_normal = TransformVec3(index.m_vertexB.m_normal, rotate);
		index.m_vertexB.m_normal.Normalize();
		index.m_vertexC.m_normal = TransformVec3(index.m_vertexC.m_normal, rotate);
		index.m_vertexC.m_normal.Normalize();
	}

}

MeshCollision::CheckHitResult MeshCollision::CheckHitRay(KazMath::Vec3<float> arg_origin, KazMath::Vec3<float> arg_direction)
{
	/*-- �@ �|���S����@���������ƂɃJ�����O���� --*/

	//�@���ƃ��C�̕����̓��ς�0���傫�������ꍇ�A���̃|���S���͔w�ʂȂ̂ŃJ�����O����B
	for (auto& index : m_collisionMeshData) {

		//���̃f�[�^�͎��̓����蔻��̂Ƃ��������p�������̂ŁA��U�����ŏ��������Ă����B
		index.m_isActive = true;

		//���Α��������Ă����疳�����B
		if (-0.0001f < index.m_vertexA.m_normal.Dot(arg_direction)) {

			index.m_isActive = false;
			continue;

		}

	}

	/*-- �A �|���S���ƃ��C�̓����蔻����s���A�e�����L�^���� --*/

	//�L�^�p�f�[�^
	std::vector<CheckHitResult> hitDataContainer;

	for (auto& index : m_collisionMeshData) {

		//�|���S��������������Ă����玟�̏�����
		if (!index.m_isActive) continue;

		//���C�̊J�n�n�_���畽�ʂɂ��낵�������̒��������߂�
		//KazMath::Vec3<float> planeNorm = -index.m_vertexA.normal;
		KazMath::Vec3<float> planeNorm = KazMath::Vec3<float>(KazMath::Vec3<float>(index.m_vertexA.m_pos - index.m_vertexC.m_pos).GetNormal()).Cross(KazMath::Vec3<float>(index.m_vertexA.m_pos - index.m_vertexB.m_pos).GetNormal());
		float rayToOriginLength = arg_origin.Dot(planeNorm);
		float planeToOriginLength = index.m_vertexA.m_pos.Dot(planeNorm);
		//���_���畽�ʂɂ��낵�������̒���
		float perpendicularLine = rayToOriginLength - planeToOriginLength;

		//�O�p�֐��𗘗p���Ď��_����Փ˓_�܂ł̋��������߂�
		float dist = planeNorm.Dot(arg_direction);
		float impDistance = perpendicularLine / -dist;

		if (std::isnan(impDistance))continue;

		//�Փ˒n�_
		KazMath::Vec3<float> impactPoint = arg_origin + arg_direction * impDistance;

		/*----- �Փ˓_���|���S���̓����ɂ��邩�𒲂ׂ� -----*/

		/* ��1�{�� */
		KazMath::Vec3<float> P1ToImpactPos = (impactPoint - index.m_vertexA.m_pos).GetNormal();
		KazMath::Vec3<float> P1ToP2 = (index.m_vertexB.m_pos - index.m_vertexA.m_pos).GetNormal();
		KazMath::Vec3<float> P1ToP3 = (index.m_vertexC.m_pos - index.m_vertexA.m_pos).GetNormal();

		//�Փ˓_�ƕ�1�̓���
		float impactDot = P1ToImpactPos.Dot(P1ToP2);
		//�_1�Ɠ_3�̓���
		float P1Dot = P1ToP2.Dot(P1ToP3);

		//�Փ˓_�ƕ�1�̓��ς��_1�Ɠ_3�̓��ς�菬����������A�E�g
		if (impactDot < P1Dot) {
			index.m_isActive = false;
			continue;
		}

		/* ��2�{�� */
		KazMath::Vec3<float> P2ToImpactPos = (impactPoint - index.m_vertexB.m_pos).GetNormal();
		KazMath::Vec3<float> P2ToP3 = (index.m_vertexC.m_pos - index.m_vertexB.m_pos).GetNormal();
		KazMath::Vec3<float> P2ToP1 = (index.m_vertexA.m_pos - index.m_vertexB.m_pos).GetNormal();

		//�Փ˓_�ƕ�2�̓���
		impactDot = P2ToImpactPos.Dot(P2ToP3);
		//�_2�Ɠ_1�̓���
		float P2Dot = P2ToP3.Dot(P2ToP1);

		//�Փ˓_�ƕ�2�̓��ς��_2�Ɠ_1�̓��ς�菬����������A�E�g
		if (impactDot < P2Dot) {
			index.m_isActive = false;
			continue;
		}

		/* ��3�{�� */
		KazMath::Vec3<float> P3ToImpactPos = (impactPoint - index.m_vertexC.m_pos).GetNormal();
		KazMath::Vec3<float> P3ToP1 = (index.m_vertexA.m_pos - index.m_vertexC.m_pos).GetNormal();
		KazMath::Vec3<float> P3ToP2 = (index.m_vertexB.m_pos - index.m_vertexC.m_pos).GetNormal();

		//�Փ˓_�ƕ�3�̓���
		impactDot = P3ToImpactPos.Dot(P3ToP1);
		//�_3�Ɠ_2�̓���
		float P3Dot = P3ToP1.Dot(P3ToP2);

		//�Փ˓_�ƕ�3�̓��ς��_3�Ɠ_2�̓��ς�菬����������A�E�g
		if (impactDot < P3Dot) {
			index.m_isActive = false;
			continue;
		}

		/* �����܂ŗ�����|���S���ɏՓ˂��Ă�I */
		CheckHitResult data;
		data.m_isHit = true;
		data.m_position = impactPoint;
		data.m_distance = impDistance;
		data.m_normal = index.m_vertexA.m_normal;
		hitDataContainer.emplace_back(data);

	}


	/*-- �B �L�^������񂩂�ŏI�I�ȏՓ˓_�����߂� --*/

	//hitPorygon�̒l��1�ȏゾ�����狗�����ŏ��̗v�f������
	if (0 < hitDataContainer.size()) {

		//�������ŏ��̗v�f������
		int min = 0;
		float minDistance = 1000000.0f;
		for (auto& index : hitDataContainer) {
			if (0 < index.m_distance && index.m_distance < minDistance) {
				minDistance = index.m_distance;
				min = static_cast<int>(&index - &hitDataContainer[0]);
			}
		}

		return hitDataContainer[min];
	}
	else {

		CheckHitResult data;
		data.m_isHit = false;
		return data;

	}
}

void MeshCollision::BuildCollisionMesh(VertexData arg_defaultVertexData)
{

	//�����蔻��p�̃��b�V�����쐬����B
	m_collisionMeshData.clear();
	const int MESH_COUNT = static_cast<int>(arg_defaultVertexData.indexArray.size()) / 3;

	for (int index = 0; index < MESH_COUNT; ++index) {

		//��������Index
		int searchIndex = index * 3;

		MeshData meshData;
		meshData.m_isActive = true;
		meshData.m_vertexA.m_pos = arg_defaultVertexData.verticesArray[arg_defaultVertexData.indexArray[searchIndex + 0]];
		meshData.m_vertexA.m_normal = arg_defaultVertexData.normalArray[arg_defaultVertexData.indexArray[searchIndex + 0]];
		meshData.m_vertexB.m_pos = arg_defaultVertexData.verticesArray[arg_defaultVertexData.indexArray[searchIndex + 1]];
		meshData.m_vertexB.m_normal = arg_defaultVertexData.normalArray[arg_defaultVertexData.indexArray[searchIndex + 1]];
		meshData.m_vertexC.m_pos = arg_defaultVertexData.verticesArray[arg_defaultVertexData.indexArray[searchIndex + 2]];
		meshData.m_vertexC.m_normal = arg_defaultVertexData.normalArray[arg_defaultVertexData.indexArray[searchIndex + 2]];

		m_collisionMeshData.emplace_back(meshData);

	}

}

KazMath::Vec3<float> MeshCollision::TransformVec3(KazMath::Vec3<float> arg_value, DirectX::XMVECTOR arg_quaternion)
{
	auto val = DirectX::XMVectorSet(arg_value.x, arg_value.y, arg_value.z, 1.0f);
	val = DirectX::XMVector3Rotate(val, arg_quaternion);
	return KazMath::Vec3<float>(val.m128_f32[0], val.m128_f32[1], val.m128_f32[2]);
}

KazMath::Vec3<float> MeshCollision::TransformVec3(KazMath::Vec3<float> arg_value, DirectX::XMMATRIX arg_mat)
{
	DirectX::XMVECTOR valVec = DirectX::XMVectorSet(arg_value.x, arg_value.y, arg_value.z, 1.0f);
	valVec = DirectX::XMVector4Transform(valVec, arg_mat);
	return KazMath::Vec3<float>(valVec.m128_f32[0], valVec.m128_f32[1], valVec.m128_f32[2]);
}
