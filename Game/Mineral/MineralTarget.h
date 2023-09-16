#pragma once
#include "../Game/DrawCallSet.h"

class Rock;
class EnemyMgr;
class MineKuji;
class MineTsumuri;
class DestructibleTree;
class BuildingMaterialMgr;
class BuildingMaterial;

class MineralTarget {

private:

	//��֘A
	int m_oldTargetRockIndex;
	int m_targetRockIndex;	//�^�[�Q�b�g����̏ꍇ�̃C���f�b�N�X�B�^�[�Q�b�g����Ȃ��Ƃ���-1�����邱�ƂŊ₩�G�̂ǂꂪ�^�[�Q�b�g����Ă���̂��𔻒f����B
	float m_targetRockDistance;
	//�~�l�N�W�֘A
	int m_oldTargetMineKujiIndex;
	int m_targetMinekujiIndex;	//�^�[�Q�b�g���~�l�N�W�̏ꍇ�̃C���f�b�N�X�B�^�[�Q�b�g����Ȃ��Ƃ���-1�����邱�ƂŃ~�l�N�W���G�̂ǂꂪ�^�[�Q�b�g����Ă���̂��𔻒f����B
	float m_targetMinekujiDistance;
	//�~�l�c�����֘A
	int m_oldTargetMineTsumuriIndex;
	int m_targetMineTsumuriIndex;	//�^�[�Q�b�g���~�l�N�W�̏ꍇ�̃C���f�b�N�X�B�^�[�Q�b�g����Ȃ��Ƃ���-1�����邱�ƂŃ~�l�N�W���G�̂ǂꂪ�^�[�Q�b�g����Ă���̂��𔻒f����B
	float m_targetMineTsumuriDistance;
	//�j��\�؊֘A
	int m_oldTargetTreeIndex;
	int m_targetTreeIndex;	//�^�[�Q�b�g���~�l�N�W�̏ꍇ�̃C���f�b�N�X�B�^�[�Q�b�g����Ȃ��Ƃ���-1�����邱�ƂŃ~�l�N�W���G�̂ǂꂪ�^�[�Q�b�g����Ă���̂��𔻒f����B
	float m_targetTreeDistance;
	//����
	int m_oldTargetMaterialIndex;
	int m_targetMaterialIndex;	//�^�[�Q�b�g���~�l�N�W�̏ꍇ�̃C���f�b�N�X�B�^�[�Q�b�g����Ȃ��Ƃ���-1�����邱�ƂŃ~�l�N�W���G�̂ǂꂪ�^�[�Q�b�g����Ă���̂��𔻒f����B
	float m_targetMaterialDistance;
	//���z��
	int m_oldTargetBuildingIndex;
	int m_targetBuildingIndex;	//�^�[�Q�b�g���~�l�N�W�̏ꍇ�̃C���f�b�N�X�B�^�[�Q�b�g����Ȃ��Ƃ���-1�����邱�ƂŃ~�l�N�W���G�̂ǂꂪ�^�[�Q�b�g����Ă���̂��𔻒f����B
	float m_targetBuildingDistance;

	//���b�N�I���Ώۂ�`�悷��p�̃��f��
	DrawCallSet m_model;
	DrawCallSet m_targetModel;

	KazMath::Transform3D m_transform;
	KazMath::Transform3D m_targetTransform;

	//���b�N�I���Ώۂɏo�����̕ϐ�
	float m_targetSineWaveTimer;
	const float TARGET_SINE_WAVE_TIMER = 0.25f;
	const float TARGET_SINE_WAVE_LENGTH = 1.0f;

	std::weak_ptr<EnemyMgr> m_enemyMgr;

public:

	MineralTarget();

	void Init();

	void Update(KazMath::Vec3<float> arg_playerPos, float arg_targetRange, std::weak_ptr<EnemyMgr> arg_enemyMgr);

	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec, std::weak_ptr<EnemyMgr> arg_enemyMgr);

	//�^�[�Q�b�g�\����Ԃ��B 0...���� 1...��
	int GetCanTarget();

	//�^�[�Q�b�g�\�Ȋ�̃C���f�b�N�X
	int GetTargetRockIndex() { return m_targetRockIndex; };

	//�^�[�Q�b�g�\�Ȋ�̎Q�Ƃ�Ԃ��B
	std::weak_ptr<Rock> GetTargetRock();
	std::weak_ptr<MineKuji> GetTargetMinekuji();
	std::weak_ptr<MineTsumuri> GetTargetMineTsumuri();
	std::weak_ptr<DestructibleTree> GetTargetTree();
	std::weak_ptr<BuildingMaterial> GetTargetMaterial();

	int GetTargetBuilidngIndex() { return m_targetBuildingIndex; }

};