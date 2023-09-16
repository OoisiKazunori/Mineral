#pragma once
#include"../DirectXCommon/Base.h"
#include"../Scene/SceneBase.h"
#include"TitleScene.h"
#include"GameScene.h"
#include"../Scene/SceneChange.h"
#include"../RenderTarget/GaussianBuler.h"
#include"../RenderTarget/RenderTargetStatus.h"
#include<memory>
#include"../KazLibrary/Render/DrawingByRasterize.h"
#include "../KazLibrary/Sound/SoundManager.h"
#include "../KazLibrary/Raytracing/RayPipeline.h"


class SceneManager
{
public:
	SceneManager();
	~SceneManager();
	void Update();
	void Draw();

	bool endGameFlag;
private:
	std::vector<std::unique_ptr<SceneBase>> scene;
	std::unique_ptr<ChangeScene::SceneChange> change;
	int nowScene, nextScene;
	bool initGameFlag;

	bool sceneChangeFlag;

	bool itisInArrayFlag;
	bool gameFirstInitFlag;

	//SE
	SoundData m_Title;



	//�{�����[���t�H�O�p3D�e�N�X�`��
	KazBufferHelper::BufferData m_volumeFogTextureBuffer;
	ComputeShader m_volumeNoiseShader;
	struct NoiseParam
	{
		KazMath::Vec3<float> m_worldPos;
		float m_timer;
		float m_windSpeed;
		float m_windStrength;
		float m_threshold;
		float m_skydormScale;
		int m_octaves;
		float m_persistence;
		float m_lacunarity;
		float m_pad;
	}m_noiseParam;
	KazBufferHelper::BufferData m_noiseParamData;

	//�{�����[���t�H�O�p�萔�o�b�t�@
	struct RaymarchingParam
	{
		KazMath::Vec3<float> m_pos; //�{�����[���e�N�X�`���̃T�C�Y
		float m_gridSize; //�T���v�����O����O���b�h�̃T�C�Y
		KazMath::Vec3<float> m_color; //�t�H�O�̐F
		float m_wrapCount; //�T���v�����O���W���͂ݏo�����ۂɉ���܂�Wrap���邩
		float m_sampleLength; //�T���v�����O����
		float m_density; //�Z�x�W��
		int m_isSimpleFog;
		int m_isActive;
	}m_raymarchingParam;
	KazBufferHelper::BufferData m_raymarchingParamData;

	//OnOff�f�o�b�O�p
	struct DebugRaytracingParam
	{
		int m_debugReflection;
		int m_debugShadow;
		float m_sliderRate;
		float m_skyFacter;
	}m_debugRaytracingParam;
	KazBufferHelper::BufferData m_debugRaytracingParamData;

	//�f�o�b�O�p��OnOff�؂�ւ����C��
	bool m_isOldDebugRaytracing;
	float m_debugLineScale;

	//�f�o�b�O�֘A�̕ϐ�
	bool m_isDebugCamera;
	bool m_isDebugRaytracing;
	bool m_isDebugTimeZone;
	bool m_isDebugVolumeFog;
	bool m_isDebugSea;


	//���X�^���C�U�`��
	DrawingByRasterize m_rasterize;

	//���C�g���Ƀ��f����g�ݍ��ޗp�̔z��N���X
	Raytracing::BlasVector m_blasVector;
	//���C�g���ŕ`�悷�邽�߂̏��
	Raytracing::Tlas m_tlas;
	//���C�g���p�p�C�v���C��
	std::vector<Raytracing::RayPipelineShaderData> m_pipelineShaders;
	std::unique_ptr<Raytracing::RayPipeline> m_rayPipeline;
};