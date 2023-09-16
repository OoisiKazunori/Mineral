#include "CPUParticle.h"
#include "CPUParticle.h"

CPUParticle::CPUParticle()
{
	//�p�[�e�B�N���̃V�X�e���S�̂̏���������

	//�p�[�e�B�N���̃N���A����
	for (int i = 0; i < EMITTER_MAX_NUM; ++i)
	{
		m_emitterTableArray[i] = INVAILD_NUM;
	}
	for (int i = 0; i < m_particleTransformDataArray.size(); ++i)
	{
		m_particleTransformDataArray[i].m_isEnableFlag = false;
	}
	for (int i = 0; i < m_particleHeaderArray.size(); ++i)
	{
		m_particleHeaderArray[i].Release();
	}

}

void CPUParticle::GenerateEffect(int arg_particleNum)
{
	const int emitterID = m_emitterHandle.GetHandle();

	//�G�~�b�^�[��������---------------------------------------
	//�p�[�e�B�N���͈̔͂����߂�
	//m_emitterHeaderArray[emitterID].m_particleBinarySize = { 0,arg_particleNum };
	//�g�p����p�[�e�B�N���̃C���f�b�N�X�����炤(���h�~�����܂�)
	int count = 0;
	for (int i = 0; i < m_particleHeaderArray.size(); ++i)
	{
		if (m_particleHeaderArray[i].m_emitterID != INVAILD_NUM)
		{
			continue;
		}
		m_particleHeaderArray[i].m_emitterID = emitterID;
		if (1 <= KazMath::Rand<int>(2, 0))
		{
			m_particleHeaderArray[i].m_isAliveFlag = true;
		}
		else
		{
			m_particleHeaderArray[i].m_isAliveFlag = false;
		}
		m_particleHeaderArray[i].m_depth = KazMath::Rand<float>(1.0f, 0.0f);

		//���g�p�̃p�[�e�B�N����������
		for (int particleIndex = 0; particleIndex < m_particleTransformDataArray.size(); ++particleIndex)
		{
			if (m_particleTransformDataArray[particleIndex].m_isEnableFlag)
			{
				continue;
			}
			m_particleTransformDataArray[particleIndex].m_isEnableFlag = true;
			m_particleHeaderArray[i].m_particleIndex = particleIndex;
			break;
		}
		++count;

		if (arg_particleNum <= count)
		{
			break;
		}
	}

	bool debug = false;
	//�G�~�b�^�[��������---------------------------------------
}

void CPUParticle::ReleaseEffect(int arg_emitterID)
{
	//m_emitterHeaderArray[arg_emitterID].m_particleBinarySize = {};
	//�G�~�b�^�[�̃p�[�e�B�N���͈͂���g�p���Ă���p�[�e�B�N���C���f�b�N�X�����
	//DataSize size = m_emitterHeaderArray[arg_emitterID].m_particleBinarySize;
	for (int i = 0; i < m_particleHeaderArray.size(); ++i)
	{
		if (m_particleHeaderArray[i].m_emitterID != arg_emitterID)
		{
			continue;
		}
		int particleIndex = m_particleHeaderArray[i].m_particleIndex;
		m_particleTransformDataArray[particleIndex].m_isEnableFlag = false;
		m_particleHeaderArray[i].Release();
	}

	m_emitterHandle.DeleteHandle(arg_emitterID);
}

void CPUParticle::UpdateEmitter()
{
}

void CPUParticle::UpdateEmitterAndParticleOnGPU()
{
	DataSize enableSize;
	enableSize.m_head = 0;
	enableSize.m_size = 0;
	//�X�V
	for (int particleIndex = 0; particleIndex < m_particleHeaderArray.size(); ++particleIndex)
	{
		if (!m_particleHeaderArray[particleIndex].m_isAliveFlag)
		{
			continue;
		}
		//�p�[�e�B�N���̍X�V����
		int id = m_particleHeaderArray[particleIndex].m_particleIndex;
		m_particleTransformDataArray[id].m_transform.pos = { 100.0f,100.0f,100.0f };
		enableSize.m_size = particleIndex;
	}

	//�����\�[�g��������
	QuickSortAliveFlag(m_particleHeaderArray, enableSize.m_head, enableSize.m_size);

	DataSize aliveRange, deadRange;
	aliveRange.m_head = 0;
	//����
	for (int i = 0; i < m_particleHeaderArray.size(); ++i)
	{
		if (!m_particleHeaderArray[i].m_isAliveFlag)
		{
			aliveRange.m_size = i - 1;
			deadRange.m_head = i;
			break;
		}
	}
	//���S
	for (int i = deadRange.m_head; i < m_particleHeaderArray.size(); ++i)
	{
		if (m_particleHeaderArray[i].m_emitterID == INVAILD_NUM)
		{
			deadRange.m_size = i - 1;
			break;
		}
	}

	//�G�~�b�^�[ID�̃\�[�g��������
	//�����ł̃\�[�g
	QuickSortEmitterID(m_particleHeaderArray, aliveRange.m_head, aliveRange.m_size);
	//���S�ł̃\�[�g
	QuickSortEmitterID(m_particleHeaderArray, deadRange.m_head, deadRange.m_size);


	std::vector<DataSize>emitterDataSize;
	int prevHandle = -1;
	for (int i = 0; i < m_particleHeaderArray.size(); ++i)
	{
		if (m_particleHeaderArray[i].m_emitterID == INVAILD_NUM)
		{
			break;
		}
		if (m_particleHeaderArray[i].m_emitterID != prevHandle)
		{
			emitterDataSize.emplace_back();
			emitterDataSize.back().m_head = i;
			prevHandle = m_particleHeaderArray[i].m_emitterID;
		}
		emitterDataSize.back().m_size = i;
	}

	//�[�x�\�[�g��������
	for (int i = 0; i < emitterDataSize.size(); ++i)
	{
		//�G�~�b�^�[�P�ʂ�Z�\�[�g��������
		QuickSortDepth(m_particleHeaderArray, emitterDataSize[i].m_head, emitterDataSize[i].m_size);
	}

	bool debug = false;

	/*
	Begin Update
	�O�t���[���Ɣ����\��̃p�[�e�B�N���������đ������v�Z����
	*/

	/*
	Fill Unused Index
	�V�K�G�~�b�^�[���ǉ����ꂽ��APart�V�������蓖�Ă�̈�ɏ�������������(INVAILD_NUM�Ŗ��߂�)
	�̈�͖�������ǉ����Ă��� -> emplance_back����ƌ�����
	*/

	/*
	Spawn Particles
	�V������������p�[�e�B�N�����̌���@���@ParticleHeader�ɖ�������ǉ�����B
	EmitterRange�̏�����
	(�ǂ������Head�̈ʒu���������)
	*/

	/*
	Initialize Particles

	Dispatch(�G�~�b�^�[�̐�,1,1)
	*/

	/*
	Update Particle
	1�X���b�h��1�p�[�e�B�N���̍X�V
	ParticleHeader���Q�Ƃ��čX�V�������鎖�Ő������Ă���p�[�e�B�N���̂ݏ����ł���
	UberShader(�����̋@�\����̃V�F�[�_�[�ɓZ�߂�)�I�ɑS�A�C�e���̎��s

	��R�̊֐���p�ӂ��Ď��s���Ă���...Item�̎�ނ��L���݂������B�p�ӂ��ꂽ�֐��������o���Ȃ����낤�B
	�ړ��ʂ̏��������s���Ă����肷��̂�
	*/

	/*
	Bitonic sort
	�p�[�e�B�N����Alive,EmitterID,Depth�̏��Ń\�[�g��������
	*/

	/*
	Range Particle
	�G�~�b�^�[�P�ʂŐ����Ă���p�[�e�B�N���͈̔͂Ǝ���ł���p�[�e�B�N���͈̔͂����߂�
	*/

	/*
	Terminate Particles
	���񂾃p�[�e�B�N����Index�̕ԋp
	Particle Index List�Ɏg���Ă鏊�ɏ�������
	*/

	/*
	Build Emitter Draw Arg
	EmitterRange����p�[�e�B�N���̐������߂�EmitterData���X�V
	*/

	/*
	Build Primitive
	�p�[�e�B�N����񂩂璸�_�𐶐�����B
	1�X���b�h1�p�[�e�B�N��
	*/
}
