#pragma once
#include "DrawCallSet.h"
#include "DrawCallUISet.h"
#include "../KazLibrary/Sound/SoundManager.h"

class Player {

private:
	SoundData slap_se;
	SoundData slap_strong_se;
	SoundData damage;
	SoundData down;
	//�v���C���[�`��֘A
	DrawCallSet m_model;
	DrawCallSet m_attackModel;

	KazMath::Transform3D m_oldTransform;
	KazMath::Transform3D m_transform;
	KazMath::Vec3<float> m_daipanPos;	//��p�����ɃJ�������Œ肷�邽�߂̍��W
	KazMath::Vec3<float> m_forwardVec;			//���ʃx�N�g��
	DirectX::XMVECTOR m_forwardQ;

	//����֘A
	KazMath::Vec3<float> m_mineralCenterBasePos;	//�~�l�����̒��S���W�B������ړ������鎞�Ɏg���B
	KazMath::Vec3<float> m_mineralCenterPos;		//�~�l�����̒��S���W�B������ړ������鎞�Ɏg���B
	const float MINERAL_CENTER_DISTANCE = 10.0f;
	const float MINERAL_MOVE_SPEED = 0.8f;

	//��p���͈̔͂̃��f���֘A
	DrawCallSet m_daipanModel;
	DrawCallSet m_daipanStrongModel;

	//�_���[�W���󂯂��Ƃ��̃V�F�C�N
	float m_damageShake;
	KazMath::Vec3<float> m_damageShakePos;
	const float DAMAGE_SHAKE = 3.0f;

	//HP��0�ɂȂ��ăX�^����ԂɂȂ����ۂɂ��ꂪ�񕜂���܂ł̃^�C�}�[
	float m_hpRecoveryTimer;
	const float HP_RECOVERY_TIMER = 300.0f;

	//��p���֘A
	bool m_isInputDaipan;	//��p���̃L�[�����͂���Ă��邩
	bool m_isOldInputDaipan;//��p���̃L�[�̓��͂̑O�t���[��
	enum DAIPAN_STATE {
		NONE,
		CHARGE,			//���͂��`���[�W���Ă���ԁB
		DAIPAN_NOW,		//��p����
		RETURN,

	}m_daipanStatus;
	float m_daipanXAngle;

	//����p���̕ϐ�
	bool m_isDaipanStrong;
	float m_daipanStrongTimer;
	const float DAIPAN_STRONG_TIMER = 40.0f;
	KazMath::Vec3<float> m_daipanShakePos;
	const float DAIPAN_SHAKE_POS = 0.5f;

	//��p�����̕ϐ�
	float m_daipanStartPosY;
	float m_daipanTimer;
	const float DAIPAN_NOW_TIMER = 7.0f;
	const float DAIPAN_NOW_STRONG_TIMER = 4.0f;
	const float DAIPAN_SHAKE = 1.5f;
	const float DAIPAN_STRONG_SHAKE = 3.8f;

	//��p������A���Ă���r���̕ϐ��B
	float m_daipanReturnTimer;
	const float DAIPAN_RETURN_TIMER = 8.0f;

	//Y���֘A
	const float DEFAULT_Y = 30.0f;
	const float DAIPAN_Y = DEFAULT_Y + 10.0f;

	//�~�l�������W��������͈�
	const float MINERAL_AFFECT_RANGE = 60.0f;
	const float MINERAL_AFFECT_STRONG_RANGE = 15.0f;

	//HP�֘A
	float m_hp;
	bool m_isStun;
	const float HP = 10;
	const float HIT_SCALE = 5.0f;
	float m_autoRecoveryStartTimer;
	const float AUTO_RECOVERY_START_TIMER = 300.0f;
	float m_autoRecoveryDelayTimer;
	const float AUTO_RECOVERY_DELAY_TIMER = 10.0f;

	//�~�l�������U�֘A�B
	bool m_isBreakUp;

	//�����G��������^�C�}�[
	int m_damageChangeDadanUITimer;
	const int DAMAGE_CHANGE_DADAN_UI_TIMER = 60;


	//UI�֘A
	DrawCallUISet m_hpFrameUI;
	DrawCallUISet m_hpUI;
	DrawCallUISet m_hpBackGroundUI;
	DrawCallUISet m_sousaUI;
	std::array<DrawCallUISet, 3> m_dadanUI;
	DrawCallUISet m_dadanBackGroundUI;



	KazMath::Vec2<float> UI_BASE_POS = { 218.0f, 592.0f };
	KazMath::Vec2<float> UI_BASE_SCALE = { 430.0f, 250.0f };
	KazMath::Vec2<float> UI_HPBAR_POS = { 183.0f, 666.0f };
	KazMath::Vec2<float> UI_HPBAR_SCALE = { 282.0f, 32.0f };


public:

	const int DAIPAN_DAMAGE = 2;
	const int STRONG_DAIPAN_DAMAGE = 10;

	Player();

	void Init();

	void Update();

	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	//���W���擾����B
	KazMath::Transform3D GetTransform() { return m_transform; }
	//Y����0�ɂ������W���擾����B
	KazMath::Vec3<float> GetPosZeroY() { return KazMath::Vec3<float>(m_transform.pos.x, 0.0f, m_transform.pos.z); }
	//���W���擾����B
	KazMath::Vec3<float> GetMineralCenterPos() { return m_mineralCenterBasePos + m_mineralCenterPos; }
	//���ʃx�N�g���������Ă���A�B
	KazMath::Vec3<float> GetForwardVec() { return m_forwardVec; }
	//Y�����f�t�H���g�̍����ɂ������ݍ��W���擾����B
	KazMath::Vec3<float> GetTransformDefaultY() { return KazMath::Vec3<float>(m_daipanPos.x, DEFAULT_Y, m_daipanPos.z); }
	//�~�l�����ɉe����^����͈́B
	float GetMineralAffectRange() { return MINERAL_AFFECT_RANGE; }
	float GetMineralAffectStrongRange() { return MINERAL_AFFECT_STRONG_RANGE; }
	float GetMineralBreakUpRange() { return MINERAL_AFFECT_RANGE * 1.5f; }
	//��p���̃^�C�~���O��
	bool GetIsDaipanTrigger() { return (m_isDaipanStrong ? DAIPAN_NOW_STRONG_TIMER : DAIPAN_NOW_TIMER) - 1.0f == m_daipanTimer; }
	//����p�����H
	bool GetIsStrongDaipan() { return m_isDaipanStrong; }
	//���U��Ԃ��H
	bool GetIsBreakUP() { return m_isBreakUp; }

	void Damage(int arg_damage);
	bool GetIsStun() { return m_isStun; }
	float GetHitScale() { return HIT_SCALE; }

	int GetDaipanDamage() {
		return m_isDaipanStrong ? STRONG_DAIPAN_DAMAGE : DAIPAN_DAMAGE;
	}

	KazMath::Vec3<float> TransformVec3(KazMath::Vec3<float> arg_value, DirectX::XMVECTOR arg_quaternion);

};