#pragma once
#include "../Game/DrawCallSet.h"
#include "../Game/Collision/MeshCollision.h"
#include "../KazLibrary/Sound/SoundManager.h"

class Player;

class Wall {

public:
	SoundData wall_drop;
	SoundData wall_build;
	SoundData wall_break01;
	SoundData wall_break02;
	SoundData damage;

	//��{���
	std::array<DrawCallSet, 3> m_model;
	int m_modelIndex;
	DrawCallSet m_buildingBoxModel;
	KazMath::Transform3D m_transform;
	KazMath::Transform3D m_boxTransform;
	KazMath::Vec3<float> m_initPos;
	bool m_isActive;
	bool m_isOldReady;
	bool m_isReady;		//���z�\���
	bool m_isBuild;

	/*�I�J���g�]�[��*/
	float damageAmount;
	DrawCallSet m_hpBoxModel;
	KazMath::Transform3D m_hpBoxTransform;
	int m_hpBoxDrawTimer;
	const int HP_BOX_DRAW_TIME_MAX = 90;
	float hpBoxScaleStart;
	float hpBoxScaleEnd;
	bool isDrawHpBox;
	float hpBoxEaseTime;
	const float HP_BOX_EASE_TIME_MAX = 30.0f;
	float ease_scale;
	/*�I�J���g�]�[��*/

	int m_hp;
	const int HP = 15;

	//���
	enum class NOREADY_STATUS {
		APPEAR,
		STAY,
		EXIT,
	}m_noreadyStatus;

	//�e�C�[�W���O�̃^�C�}�[
	float m_easingTimer;
	const float APPEAR_EASING_TIMER = 20.0f;
	const float EXIT_EASING_TIMER = 20.0f;

	//���z�֌W�̃^�C�}�[
	enum class BUILD_STATUS {
		UPPER,
		STAY,
		DOWN,
		COMPLETE,
	}m_buildStatus;
	const float UPPER_EASING_TIMER = 30.0f;
	const float BUILD_STAY_EASING_TIMER = 10.0f;
	const float DOWN_EASING_TIMER = 10.0f;
	const float UPPER_DISTANCE = 40.0f;

	//Y����]��
	float m_rotateY;
	float m_initRotateY;

	//�T�C���g�ɕK�v�ȕϐ�
	float m_sineWaveTimer;
	const float ADD_SINE_WAVE_TIMER = 0.1f;
	const float SINE_WAVE_MOVE = 1.0f;
	const float BOX_SINE_WAVE_SCALE = 0.3f;

	//���z�ɕK�v�ȕϐ�
	int m_materialCounter;
	const int MATERIAL_COUNT = 3;

	DrawCallSet m_numberModel;
	KazMath::Transform3D m_numberModelTransform;

	//���b�V���̓����蔻��
	std::array<DrawCallSet, 3> m_meshCollisionModel;
	std::array<MeshCollision, 3> m_meshCollider;
	KazMath::Transform3D m_wallTransform;

public:

	Wall();

	void Init();

	void Genrate(KazMath::Vec3<float> arg_generatePos, float arg_rotateY, int arg_modelIndex);

	void Update(std::weak_ptr<Player> arg_player);

	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	void AddMaterial() { m_materialCounter = std::clamp(m_materialCounter + 1, 0, MATERIAL_COUNT); }
	int GetMaterialCounter() { return m_materialCounter; }
	bool IsFullMaterial() { return m_materialCounter < MATERIAL_COUNT; }

	MeshCollision::CheckHitResult CheckHitMesh(KazMath::Vec3<float> arg_pos, KazMath::Vec3<float> arg_dir);

	void Damage(int arg_damage = 1);
	int GetHP() { return m_hp; }

	bool GetIsActive() { return m_isActive; }
	bool GetIsReady() { return m_isReady; }
	bool GetIsBuild() { return m_isBuild; }
	KazMath::Vec3<float> GetPosZeroY();
	KazMath::Vec3<float> GetScale() { return m_transform.scale; }

};