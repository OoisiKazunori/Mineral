#include "MeshCollision.h"

void MeshCollision::Setting(VertexData arg_defaultVertexdata, KazMath::Transform3D arg_transform)
{

	BuildCollisionMesh(arg_defaultVertexdata);
	Transform(arg_transform);

}

void MeshCollision::Transform(KazMath::Transform3D arg_transform)
{

	//ワールド行列
	DirectX::XMMATRIX targetWorldMat = arg_transform.GetMat();
	//回転を抜き取る。
	DirectX::XMVECTOR rotate, scale, position;
	DirectX::XMMatrixDecompose(&scale, &rotate, &position, targetWorldMat);
	for (auto& index : m_collisionMeshData) {
		//頂点を変換
		index.m_vertexA.m_pos = TransformVec3(index.m_vertexA.m_pos, targetWorldMat);
		index.m_vertexB.m_pos = TransformVec3(index.m_vertexB.m_pos, targetWorldMat);
		index.m_vertexC.m_pos = TransformVec3(index.m_vertexC.m_pos, targetWorldMat);
		//法線を回転行列分だけ変換
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
	/*-- ① ポリゴンを法線情報をもとにカリングする --*/

	//法線とレイの方向の内積が0より大きかった場合、そのポリゴンは背面なのでカリングする。
	for (auto& index : m_collisionMeshData) {

		//このデータは次の当たり判定のときも引き継ぎされるので、一旦ここで初期化しておく。
		index.m_isActive = true;

		//反対側を向いていたら無効化。
		if (-0.0001f < index.m_vertexA.m_normal.Dot(arg_direction)) {

			index.m_isActive = false;
			continue;

		}

	}

	/*-- ② ポリゴンとレイの当たり判定を行い、各情報を記録する --*/

	//記録用データ
	std::vector<CheckHitResult> hitDataContainer;

	for (auto& index : m_collisionMeshData) {

		//ポリゴンが無効化されていたら次の処理へ
		if (!index.m_isActive) continue;

		//レイの開始地点から平面におろした垂線の長さを求める
		//KazMath::Vec3<float> planeNorm = -index.m_vertexA.normal;
		KazMath::Vec3<float> planeNorm = KazMath::Vec3<float>(KazMath::Vec3<float>(index.m_vertexA.m_pos - index.m_vertexC.m_pos).GetNormal()).Cross(KazMath::Vec3<float>(index.m_vertexA.m_pos - index.m_vertexB.m_pos).GetNormal());
		float rayToOriginLength = arg_origin.Dot(planeNorm);
		float planeToOriginLength = index.m_vertexA.m_pos.Dot(planeNorm);
		//視点から平面におろした垂線の長さ
		float perpendicularLine = rayToOriginLength - planeToOriginLength;

		//三角関数を利用して視点から衝突点までの距離を求める
		float dist = planeNorm.Dot(arg_direction);
		float impDistance = perpendicularLine / -dist;

		if (std::isnan(impDistance))continue;

		//衝突地点
		KazMath::Vec3<float> impactPoint = arg_origin + arg_direction * impDistance;

		/*----- 衝突点がポリゴンの内側にあるかを調べる -----*/

		/* 辺1本目 */
		KazMath::Vec3<float> P1ToImpactPos = (impactPoint - index.m_vertexA.m_pos).GetNormal();
		KazMath::Vec3<float> P1ToP2 = (index.m_vertexB.m_pos - index.m_vertexA.m_pos).GetNormal();
		KazMath::Vec3<float> P1ToP3 = (index.m_vertexC.m_pos - index.m_vertexA.m_pos).GetNormal();

		//衝突点と辺1の内積
		float impactDot = P1ToImpactPos.Dot(P1ToP2);
		//点1と点3の内積
		float P1Dot = P1ToP2.Dot(P1ToP3);

		//衝突点と辺1の内積が点1と点3の内積より小さかったらアウト
		if (impactDot < P1Dot) {
			index.m_isActive = false;
			continue;
		}

		/* 辺2本目 */
		KazMath::Vec3<float> P2ToImpactPos = (impactPoint - index.m_vertexB.m_pos).GetNormal();
		KazMath::Vec3<float> P2ToP3 = (index.m_vertexC.m_pos - index.m_vertexB.m_pos).GetNormal();
		KazMath::Vec3<float> P2ToP1 = (index.m_vertexA.m_pos - index.m_vertexB.m_pos).GetNormal();

		//衝突点と辺2の内積
		impactDot = P2ToImpactPos.Dot(P2ToP3);
		//点2と点1の内積
		float P2Dot = P2ToP3.Dot(P2ToP1);

		//衝突点と辺2の内積が点2と点1の内積より小さかったらアウト
		if (impactDot < P2Dot) {
			index.m_isActive = false;
			continue;
		}

		/* 辺3本目 */
		KazMath::Vec3<float> P3ToImpactPos = (impactPoint - index.m_vertexC.m_pos).GetNormal();
		KazMath::Vec3<float> P3ToP1 = (index.m_vertexA.m_pos - index.m_vertexC.m_pos).GetNormal();
		KazMath::Vec3<float> P3ToP2 = (index.m_vertexB.m_pos - index.m_vertexC.m_pos).GetNormal();

		//衝突点と辺3の内積
		impactDot = P3ToImpactPos.Dot(P3ToP1);
		//点3と点2の内積
		float P3Dot = P3ToP1.Dot(P3ToP2);

		//衝突点と辺3の内積が点3と点2の内積より小さかったらアウト
		if (impactDot < P3Dot) {
			index.m_isActive = false;
			continue;
		}

		/* ここまで来たらポリゴンに衝突してる！ */
		CheckHitResult data;
		data.m_isHit = true;
		data.m_position = impactPoint;
		data.m_distance = impDistance;
		data.m_normal = index.m_vertexA.m_normal;
		hitDataContainer.emplace_back(data);

	}


	/*-- ③ 記録した情報から最終的な衝突点を求める --*/

	//hitPorygonの値が1以上だったら距離が最小の要素を検索
	if (0 < hitDataContainer.size()) {

		//距離が最小の要素を検索
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

	//当たり判定用のメッシュを作成する。
	m_collisionMeshData.clear();
	const int MESH_COUNT = static_cast<int>(arg_defaultVertexData.indexArray.size()) / 3;

	for (int index = 0; index < MESH_COUNT; ++index) {

		//検索するIndex
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
