#pragma once
#include"../KazLibrary/Render/KazRender.h"
#include"../Game/Interface/IEnemy.h"
#include"../KazLibrary/Render/MeshParticleRender.h"
#include"../KazLibrary/Animation/AnimationInRaytracing.h"
#include"../Game/Effect/EnemyDeadParticle.h"
#include "../KazLibrary/Sound/SoundManager.h"

class VirusEnemy:public IEnemy
{
public:
	VirusEnemy(int arg_moveID, float arg_moveIDparam);

	void Init(const KazMath::Transform3D* arg_playerTransform, const EnemyGenerateData& arg_generateData, bool arg_demoFlag);
	void Finalize();
	void Update();
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

private:

	//���f���A�A�j���[�V�����֘A
	DrawFuncData::DrawCallData m_model;
	AnimationInRaytracing m_computeAnimation;
	std::shared_ptr<ModelAnimator> m_animation;
	std::shared_ptr<ModelInfomation>m_modelData;
	KazMath::Vec3<float> m_prevPlayerPos;
	KazMath::Vec3<float> m_initPos;	//�����n�_ ���������Ȃ�
	KazMath::Vec3<float> m_basePos; //�T�C���g�œ������Ƃ��̒��S�_ �����n�_������B


	//���S���֘A
	SoundData m_dispperSE;
	SoundData m_spawnSE;
	struct DeadEffectData {
		KazMath::Vec4<float> m_dissolve;
		KazMath::Vec4<float> m_outlineColor;
	}m_deadEffectData;
	const float DEAD_EFFECT_VEL = 2.5f;	//���S�G�t�F�N�g���̏����x
	const KazMath::Vec4<float> OUTLINE_COLOR = KazMath::Vec4<float>(0.35f, 0.91f, 0.55f, 1.0f);


	//�p�[�e�B�N���֘A
	std::unique_ptr<MeshParticleRender>m_meshParticleRender;
	std::unique_ptr<EnemyDeadParticle>m_deadParticle;
	std::unique_ptr<EnemyDeadParticle>m_knockBackParticle;
	DirectX::XMMATRIX m_motherMat;
	const float VIRUS_SCALE = 2.0f;
	bool m_initDeadParticleFlag;
	float m_alpha;

	
	//���̑��ϐ�
	enum STATUS {
		APPEAR,
		STAY,
		DEAD,
	}m_status;

	//�o���Ɋւ���C�[�W���O�^�C�}�[
	float m_appearEasingTimer;
	const float APPEAR_EASING_TIMER = 40.0f;

	//STAY���̍X�V����
	float m_moveSineTimer;	//�T�C���g�œ������^�C�}�[
	const float MOVE_SINE_TIMER = 30.0f;

	//���S���o�p
	float m_gravity;
	const float GRAVITY = 0.05f;

	//HP
	int m_hp;
	int m_prevhp;


};

