#pragma once
#include"../KazLibrary/Helper/ISinglton.h"
#include"../Game/Effect/InstanceMeshParticleData.h"
#include"../KazLibrary/Buffer/CreateMeshBuffer.h"
#include "../KazLibrary/Loader/ModelLoader.h"
#include<string>

struct MeshParticleLoadData
{
	//�p�[�e�B�N���̕΂�
	UINT bias;
	//�O�p�`���Ƃ̃p�[�e�B�N���̐�
	UINT perTriangleNum;
	//�O�p�ʂ̐�
	UINT faceCountNum;
};

class MeshParticleLoader :public ISingleton<MeshParticleLoader>
{
public:
	MeshParticleLoader();
	const InitMeshParticleData &LoadMesh(const std::string& FIRLE_DIR, const std::string& MODEL_NAME, const DirectX::XMMATRIX *MOTHER_MAT, const MeshParticleLoadData &MESH_PARTICLE_DATA, RESOURCE_HANDLE TEX_HANDLE = -1);

private:
	HandleMaker handle;
	std::vector<CreateMeshBuffer> meshBuffer;
	std::vector<std::unique_ptr<InitMeshParticleData>> meshParticleDataArray;

	std::vector<std::string>handleNameArray;
};

