#pragma once
#include "../KazLibrary/Helper/ISinglton.h"

class ResultFlag : public ISingleton<ResultFlag> {

public:

	bool m_isResult;
	bool m_isQuit;
	bool m_isStart;
	bool m_isReadyResult;

	//�^�C�g���̕`��֘A
	bool m_isDraw;	//�^�C�g����UI�̃A���t�@�����Ă��邩�B

	int m_uiDeleteTime;
	const int UI_DELETE_TIME = 30;

public:

	void Init() {
		m_isResult = false;
		m_isQuit = false;
		m_isStart = false;
		m_isDraw = false;
		m_isReadyResult = false;
		m_uiDeleteTime = 0;
	};

};