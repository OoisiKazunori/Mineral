#pragma once
#include "PlayerShotEffect.h"
#include "../KazLibrary/Helper/ISinglton.h"
#include <array>
#include <memory>
#include <Math/KazMath.h>
#include "../Interface/IEnemy.h"
#include "../KazLibrary/Sound/SoundManager.h"

class PlayerShotEffect {

private:

	const KazMath::Vec3<float>* m_refPlayerPos;
	KazMath::Vec3<float> m_controlPointVec;
	const float CONTROL_POINT_R = 25.0f;
	static const int POINT_COUNT = 15;
	std::array<KazMath::Vec3<float>, POINT_COUNT> m_points;
	shared_ptr<IEnemy> m_refEnemy;
	std::array<std::shared_ptr<IEnemy>, 7> m_otherEnemy;
	int m_frame;
	const int EFFECT_FRAME = 20;

	bool m_isFinish;
	KazMath::Vec3<float> m_finishVel;

	DrawFuncData::DrawCallData m_model;
	KazMath::Transform3D m_transform;
	KazMath::Vec3<float> m_prevPos;
	KazMath::Vec4<float> m_emissive;

	//���̕`��ɕK�v�ȕϐ���
	int m_vertexBufferHandle;//�f�o�b�N�p�̃X�v���C���Ȑ��`��̒��_�o�b�t�@
	DrawFuncData::DrawCallData m_splineDrawCall;
	std::vector<KazMath::Vec3<float>>m_splineRailPosArray;

	//���ˉ��̃f�[�^
	SoundData m_shotSE;
	SoundData m_hitSE;
	

public:

	bool m_isActive;

public:

	PlayerShotEffect();

	void Init();

	void Generate(const KazMath::Vec3<float>* arg_refPlayerPoint, shared_ptr<IEnemy> arg_refEnemy, std::array<std::shared_ptr<IEnemy>, 7> arg_refOtherEnemy);

	void Update(std::array<bool, 16>& arg_hitArray, int* arg_hitNum);
	
	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

private:

	KazMath::Vec3<float> EvaluateBezierCurve(const std::array<KazMath::Vec3<float>, 4>& arg_controlPoints, float arg_t);

};