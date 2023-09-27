#pragma once
#include"../DrawCallSet.h"
#include"../DrawCallUISet.h"

class SmokeParticle
{
public:
	SmokeParticle();
	void Init(
		const KazMath::Vec3<float>& arg_emittPos,
		const KazMath::Vec3<float>& arg_vel,
		float arg_radius,
		float arg_scale
	);
	void Update();
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	static int s_disappearMaxTime;	//�X�P�[���̏k�����n�܂�^�C�~���O�̊
	static int s_velRateTime;		//�U���͈͂܂ōL���鎞��
	static int s_scaleRateTime;		//���������鎞��
	KazMath::Transform3D m_transform;
private:
	bool m_initFlag;
	DrawCallSet m_smokeRender;
	KazMath::Vec3<float>m_emittPos;
	KazMath::Vec3<float>m_vel;
	float m_scaleMax;
	float m_velRate;		//�U���͈͂܂ōL���銄��
	float m_radius;			//�L����͈�
	int m_disappearTimer;
	int m_disappearMaxTimer;//�X�P�[���̏k�����n�܂�^�C�~���O�̊���炠����x�����_������������������
	float m_scaleRate;		//������ۂ̃X�s�[�h
};

