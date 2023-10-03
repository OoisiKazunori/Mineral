#pragma once
#include"../KazLibrary/Helper/ISinglton.h"
#include"../KazLibrary/Helper/KazBufferHelper.h"
#include"../KazLibrary/Math/KazMath.h"
#include<memory>
/// <summary>
/// �q�b�g�X�g�b�v�ݒ�p�\����
/// </summary>
struct HitStopParam
{
	//�K�p���鎞��
	int m_time;
	//�K�p���鑬�x
	float m_speed;

	//�R���X�g���N�^
	HitStopParam(int time = 0, float speed = 1.0f);
};

/// <summary>
/// �Q�[���X�s�[�h�𑀂�N���X
/// </summary>
class StopMgr :public ISingleton<StopMgr>
{

private:

	//�Q�[�����ŗ���鑬�x�̊���
	float m_gameSpeed;

	//���x���ω����Ă��鎞��
	int m_slowTimer;
public:

	void Init();

	void Update();

	/// <summary>
	/// �����œn���ꂽ�l�ɍ��킹�ăq�b�g�X�g�b�v�����s����
	/// </summary>
	/// <param name="param">�q�b�g�X�g�b�v�𔭐�������l</param>
	void HitStopStart(const HitStopParam& param);

	//�q�b�g�X�g�b�v�K�p����ۂ̃Q�[���X�s�[�h���󂯎��
	float GetGameSpeed();

	void HitStopEnemy()
	{

	}
	bool IsHitStop() {
		return 0 < m_slowTimer;
	}
};