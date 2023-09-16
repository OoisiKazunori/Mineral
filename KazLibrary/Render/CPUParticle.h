#pragma once
#include"../KazLibrary/Math/KazMath.h"
#include"../KazLibrary/Helper/HandleMaker.h"
#include<stdlib.h>
#include<stdio.h>

static const int INVAILD_NUM = 9999;

///<summary>
///�o�C�i���[�ɂǂꂭ�炢�͈͕̔��m�ۂ�����
///</summary>
struct DataSize
{
	int m_head, m_size;
};

///<summary>
///�p�[�e�B�N���͈̔�
///</summary>
struct EmitterHeaderData
{
	DataSize m_particleBinarySize;
};

/// <summary>
/// �p�[�e�B�N���̊Ǘ����
/// </summary>
struct ParticleHeaderData
{
	bool m_isAliveFlag;
	int m_emitterID;
	int m_particleIndex;
	float m_depth;

	void Release()
	{
		m_isAliveFlag = false;
		m_emitterID = INVAILD_NUM;
		m_particleIndex = INVAILD_NUM;
		m_depth = -1.0f;
	}
};

/// <summary>
/// �p�[�e�B�N���̖{��
/// </summary>
struct ParticleBinary
{
	bool m_isEnableFlag;
	KazMath::Transform3D m_transform;
};

///<summary>
///���̎����̗�������ׂ̃A���S���Y��
///https://www.docswell.com/s/CAPCOM_RandD/Z7DPJK-2022-07-15-133910#p25
///</summary>
class CPUParticle
{
public:
	CPUParticle();

	///<summary>
	///�G�t�F�N�g��������(CPU)
	///</summary>
	void GenerateEffect(int arg_particleNum);

	void ReleaseEffect(int arg_emitterID);

	///<summary>
	///�G�~�b�^�[�X�V(CPU)
	///</summary>
	void UpdateEmitter();

	///<summary>
	///GPU�ł̃G�~�b�^�[�ƃp�[�e�B�N���̍X�V(GPU)
	///</summary>
	void UpdateEmitterAndParticleOnGPU();

private:
	static const int EMITTER_MAX_NUM = 3;
	static const int PARTICLE_MAX_NUM = 3;


	//CPU����GPU�֏������܂��o�b�t�@---------------------------------------

	std::array<int, EMITTER_MAX_NUM>m_emitterTableArray;
	std::array<EmitterHeaderData, EMITTER_MAX_NUM>m_emitterHeaderArray;

	//CPU����GPU�֏������܂��o�b�t�@---------------------------------------

	//GPU���݂̂ŏ������܂��o�b�t�@---------------------------------------

	//std::array<std::vector<int>, EMITTER_MAX_NUM>m_particleIndexListArray;
	std::array<ParticleHeaderData, 100> m_particleHeaderArray;
	//�p�[�e�B�N���̖{��
	std::array<ParticleBinary, 100>m_particleTransformDataArray;

	//GPU���݂̂ŏ������܂��o�b�t�@---------------------------------------

	HandleMaker m_emitterHandle;

	void QuickSortEmitterID(std::array<ParticleHeaderData, 100>& arg_particle, int arg_left, int arg_right)
	{
		int pl = arg_left;
		int pr = arg_right;

		int x = arg_particle[(pl + pr) / 2].m_emitterID;


		bool isSameFlag = true;
		for (auto obj : arg_particle)
		{
			if (obj.m_emitterID != x)
			{
				isSameFlag = false;
			}
		}
		//���ׂē����l�Ȃ�\�[�g���Ȃ�
		if (isSameFlag)
		{
			return;
		}

		do
		{
			while (arg_particle[pl].m_emitterID < x)pl++;
			while (x < arg_particle[pr].m_emitterID)pr--;

			if (pl <= pr)
			{
				std::swap(arg_particle[pl].m_emitterID, arg_particle[pr].m_emitterID);
				std::swap(arg_particle[pl].m_isAliveFlag, arg_particle[pr].m_isAliveFlag);
				std::swap(arg_particle[pl].m_depth, arg_particle[pr].m_depth);
				std::swap(arg_particle[pl].m_particleIndex, arg_particle[pr].m_particleIndex);
				pl++;
				pr--;
			}

		} while (pl <= pr);

		if (arg_left < pr)QuickSortEmitterID(arg_particle, arg_left, pr);
		if (pl < arg_right)QuickSortEmitterID(arg_particle, pl, arg_right);

	};

	void QuickSortDepth(std::array<ParticleHeaderData, 100>& arg_particle, int arg_left, int arg_right)
	{
		int pl = arg_left;
		int pr = arg_right;

		float x = arg_particle[(pl + pr) / 2].m_depth;


		bool isSameFlag = true;
		for (auto obj : arg_particle)
		{
			if (obj.m_depth != x)
			{
				isSameFlag = false;
			}
		}
		//���ׂē����l�Ȃ�\�[�g���Ȃ�
		if (isSameFlag)
		{
			return;
		}

		do
		{
			while (arg_particle[pl].m_depth < x)pl++;
			while (x < arg_particle[pr].m_depth)pr--;

			if (pl <= pr)
			{
				std::swap(arg_particle[pl].m_emitterID, arg_particle[pr].m_emitterID);
				std::swap(arg_particle[pl].m_isAliveFlag, arg_particle[pr].m_isAliveFlag);
				std::swap(arg_particle[pl].m_depth, arg_particle[pr].m_depth);
				std::swap(arg_particle[pl].m_particleIndex, arg_particle[pr].m_particleIndex);
				pl++;
				pr--;
			}

		} while (pl <= pr);

		if (arg_left < pr)QuickSortDepth(arg_particle, arg_left, pr);
		if (pl < arg_right)QuickSortDepth(arg_particle, pl, arg_right);

	};

	void QuickSortAliveFlag(std::array<ParticleHeaderData, 100>& arg_particle, int arg_left, int arg_right)
	{
		int pl = arg_left;
		int pr = arg_right;

		bool x = !arg_particle[(pl + pr) / 2].m_isAliveFlag;


		bool isSameFlag = true;
		for (auto obj : arg_particle)
		{
			if (!obj.m_isAliveFlag != x)
			{
				isSameFlag = false;
			}
		}
		//���ׂē����l�Ȃ�\�[�g���Ȃ�
		if (isSameFlag)
		{
			return;
		}

		do
		{
			while (!arg_particle[pl].m_isAliveFlag < x)pl++;
			while (x < !arg_particle[pr].m_isAliveFlag)pr--;

			if (pl <= pr)
			{
				std::swap(arg_particle[pl].m_emitterID, arg_particle[pr].m_emitterID);
				std::swap(arg_particle[pl].m_isAliveFlag, arg_particle[pr].m_isAliveFlag);
				std::swap(arg_particle[pl].m_depth, arg_particle[pr].m_depth);
				std::swap(arg_particle[pl].m_particleIndex, arg_particle[pr].m_particleIndex);
				pl++;
				pr--;
			}

		} while (pl <= pr);

		if (arg_left < pr)QuickSortAliveFlag(arg_particle, arg_left, pr);
		if (pl < arg_right)QuickSortAliveFlag(arg_particle, pl, arg_right);

	};

	//emitterBinary��particleBinary�͔��f���ނ����B�ǂ�����đg�ݍ��ނ񂾂낤�B
	//vector�͋֎~�B�V�F�[�_�[�ł͉ϒ��ł͖�������(�������������Ă�)

};

