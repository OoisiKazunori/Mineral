#pragma once
#include "../DrawCallSet.h"
#include "../KazLibrary/Helper/ISinglton.h"
#include "../DrawCallUISet.h"
#include "../KazLibrary/Sound/SoundManager.h"

class Wave;
class EnemyMgr;
class Core;

class WaveMgr : public ISingleton<WaveMgr> {

public:
	SoundData m_BGM;
	SoundData start_morning;
	float volume;
	bool start_bgm;

private:

	std::vector<std::shared_ptr<Wave>> m_waves;		//�E�F�[�u�̔z��
	int m_waveCount;
	int m_nowWaveIndex;
	bool m_isFinishAllWave;

	//�E�F�[�u
	std::shared_ptr<Wave> m_wave1;	//�ʏ��1Wave��
	std::shared_ptr<Wave> m_tutorialWave;	//�`���[�g���A���p�̃E�F�[�u1

	//�^�C�}�[��UI
	DrawCallUISet m_timerUI;
	DrawCallUISet m_frameUI;
	//������UI
	DrawCallUISet m_daysUI;
	DrawCallUISet m_dayNumerUI;

public:

	void Setting(std::weak_ptr<Core> m_core);

	void Init(std::weak_ptr<EnemyMgr> arg_enemyMgr);

	void Update(std::weak_ptr<EnemyMgr> arg_enemyMgr);

	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec);

	//1Wave�ڂ�L��������B
	void GameStart();

	bool GetIsNight();

	void SetTime(int wave_index, int wave_time);

	bool GetIsFinishAllWave() { return m_isFinishAllWave; }

	int GetNowWaveIndex() { return m_nowWaveIndex; }
	int GetWaveCount() { return m_nowWaveIndex; }

};