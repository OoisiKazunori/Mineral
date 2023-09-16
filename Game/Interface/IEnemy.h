#pragma once
#include"../Game/Interface/IOperationObject.h"
#include"../KazLibrary/Collision/CollisionManager.h"
#include"../KazLibrary/Sound/SoundManager.h"
#include"../KazLibrary/Imgui/MyImgui.h"
#include"../Enemy/EnemyData.h"
#include"../KazLibrary/Render/DrawingByRasterize.h"
#include"../KazLibrary/Raytracing/BlasVector.h"

/// <summary>
/// �G�̒��ۃN���X
/// </summary>
class IEnemy :public IOperationObject
{
public:

	/// <summary>
	/// �m���ɏ������������f�[�^������
	/// </summary>
	IEnemy();
	/// <summary>
	/// ���z�f�X�g���N�^
	/// </summary>
	virtual ~IEnemy()
	{
	};

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="POS">�������W</param>
	virtual void Init(const KazMath::Transform3D* arg_playerTransform, const EnemyGenerateData& GENERATE_DATA, bool DEMO_FLAG = false) = 0;

	/// <summary>
	/// �I������
	/// </summary>
	virtual void Finalize() = 0;
	/// <summary>
	/// �X�V����
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// �`�揈��
	/// </summary>
	virtual void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec) = 0;

	/// <summary>
	/// �G�����S�����܂�
	/// </summary>
	void Dead(KazMath::Vec3<float>* arg_playerShotEffectPos = nullptr);

	/// <summary>
	/// ���S���o
	/// </summary>
	void DeadEffect(KazMath::Vec3<float>* POS, KazMath::Vec3<float>* ROTATION, int* ALPHA);

	//���S����Ə���
	bool ProcessingOfDeath(EnemyDeathType TYPE);
	bool ProcessingOfDeathFbx(EnemyDeathType TYPE);


	/// <returns>�G�̏��</returns>
	const std::unique_ptr<EnemyData>& GetData();

	void DeadSound();
	void ShotSound();
	void InitModel(const KazMath::Transform3D& TRANSFORM, const std::string& MODEL_FILEPASS, float HITBOX_RADIUS, EnemyModelType MODEL_TYPE, bool REV_UV_FLAG = false, bool GENERATE_PARTICLE_FLAG = true);

	//�U�����m�F����p�̊֐�
	virtual void DebugShot() { debugShotFlag = true; };

	void SetLight(const KazMath::Vec3<float> LIGHT_DIR, bool OBJ_FLAG)
	{
		if (!OBJ_FLAG)
		{
			DirectX::XMFLOAT3 dir = LIGHT_DIR.ConvertXMFLOAT3();
			iEnemy_FbxModelRender->TransData(&dir, fbxLightHandle, typeid(DirectX::XMFLOAT3).name());
		}
		else
		{
			DirectX::XMFLOAT3 dir = LIGHT_DIR.ConvertXMFLOAT3();
			iEnemy_ObjModelRender->TransData(&dir, objLightHandle, typeid(DirectX::XMFLOAT3).name());
		}
	};

	void ReleaseLight()
	{
		iEnemy_ObjModelRender->Release(objLightHandle);
		iEnemy_FbxModelRender->Release(fbxLightHandle);
	}

	KazMath::Vec3<float> GetVec3(DirectX::XMVECTOR arg_target)
	{
		return KazMath::Vec3<float>(arg_target.m128_f32[0], arg_target.m128_f32[1], arg_target.m128_f32[2]);
	}

	DirectX::XMVECTOR GetXMVECTOR(KazMath::Vec3<float> arg_target)
	{
		return DirectX::XMVECTOR{ arg_target.x, arg_target.y, arg_target.z, 0.0f };
	}

	KazMath::Vec3<float> TransformVector3(KazMath::Vec3<float> arg_target, DirectX::XMVECTOR arg_quaternion)
	{
		DirectX::XMVECTOR transformVec = DirectX::XMVector3Transform(DirectX::XMVectorSet(arg_target.x, arg_target.y, arg_target.z, 0), DirectX::XMMatrixRotationQuaternion(arg_quaternion));
		return GetVec3(transformVec);
	}

	//�ǉ��ŕ`�悵�������̂���������Ē�`����
	virtual void DrawIn() {};

	//���ʏ���--------------------------------------------
	void OnInit(bool MESH_PARTICLE_FLAG)
	{
		iEnemy_EnemyStatusData->meshParticleFlag = MESH_PARTICLE_FLAG;
		m_isBeingShot = false;
	}
	void OnUpdate()
	{
		switch (modelType)
		{
		case ENEMY_MODEL_NONE:
			break;
		case ENEMY_MODEL_OBJ:
			//if (!ProcessingOfDeath(DEATH_ROLL))
			//{
			//	//�o�ꏈ��
			//	if (iEnemy_ObjModelRender->data.colorData.color.a < 255)
			//	{
			//		iEnemy_ObjModelRender->data.colorData.color.a += 5;
			//	}
			//	else
			//	{
			//		iEnemy_ObjModelRender->data.colorData.color.a = 255;
			//	}
			//}
			break;
		case ENEMY_MODEL_FBX:
			//ProcessingOfDeathFbx(DEATH_SINK);
			break;
		case ENEMY_MODEL_MESHPARTICLE:
			break;
		default:
			break;
		}
	};
	//�G�̃��f���`��
	void OnDraw()
	{
		if (1.0f <= iEnemy_ObjModelRender->data.colorData.color.a)
		{
			switch (modelType)
			{
			case ENEMY_MODEL_NONE:
				break;
			case ENEMY_MODEL_OBJ:
				iEnemy_ObjModelRender->Draw();
				break;
			case ENEMY_MODEL_FBX:
				iEnemy_FbxModelRender->Draw();
				break;
			case ENEMY_MODEL_MESHPARTICLE:
				break;
			default:
				break;
			}
			DrawIn();
			LockOnWindow(*iEnemy_EnemyStatusData->hitBox.center);
		}
	};
	//���ʏ���--------------------------------------------

	void InitMeshPartilce(std::string arg_fileDir, std::string arg_fileName, DirectX::XMMATRIX* arg_mat);

	bool GetCanLockOn() { return m_canLockOn; }
	void SetShockWaveVel(KazMath::Vec3<float> arg_shockWaveVel) { m_shockWaveVel = arg_shockWaveVel; }
	void InitShockWaveTimer() { m_shockWaveTimer = 0; }

	std::unique_ptr<EnemyData> iEnemy_EnemyStatusData;		//�G�̏�Ԃ�ۑ�����f�[�^
	ObjModelRenderPtr iEnemy_ObjModelRender;				//�G�̕`��
	FbxModelRenderPtr iEnemy_FbxModelRender;				//�G�̕`��
	KazMath::Vec3<float> lerpPos;
	float yVel;
	bool m_isDead;
	bool m_isBeingShot;		//�e��������Ă��邩
	KazMath::Transform3D m_transform;


protected:

	bool initDeadSoundFlag;
	bool demoFlag;
	bool debugShotFlag;
	const KazMath::Transform3D* m_playerTransform;
	int moveID;
	float moveIDparam;

	bool m_canLockOn;
	int m_spawnTimer;
	bool m_canSpawn;
	KazMath::Vec3<float> m_deadEffectVel;
	KazMath::Vec3<float> m_shockWaveVel;

	//�Ռ��g
	float m_shockWaveTimer;
	const float SHOCK_WAVE_TIMER = 60.0f;
	const float SHOCK_WAVE_RAIDUS = 100.0f;

private:
	int deadSoundHandle;
	int shotSoundHandle;


	DirtyFlag<short>hpDirtyFlag;
	bool hitFlag;

	RESOURCE_HANDLE fbxLightHandle;
	RESOURCE_HANDLE objLightHandle;

	KazMath::Vec3<float>initDeadRotaVel;
	KazMath::Vec3<float>initDeadYVel;

	EnemyModelType modelType;

};
