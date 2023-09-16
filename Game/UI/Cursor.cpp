#include "Cursor.h"
#include"../KazLibrary/Helper/ResourceFilePass.h"
#include"../KazLibrary/Imgui/MyImgui.h"
#include"../KazLibrary/Sound/SoundManager.h"
#include"../UI/OptionUI.h"

DirectX::XMFLOAT2 Cursor::KOCKBACK_MAX_VALUE = { 200.0f,200.0f };
DirectX::XMFLOAT2 Cursor::KOCKBACK_VELOCITY = { 5.0f,5.0f };
KazMath::Vec2<float> Cursor::NO_MOVE_DISTANCE = { 150.0f,50.0f };

Cursor::Cursor()
{
	enableLockOnTimer = 0;
	notEnableLockOnFlag = false;

	cursorFlameTex = std::make_unique<Sprite2DRender>();
	numberTex = std::make_unique<Sprite2DRender>();

	baseSpeed = 10.0f;
	flameHandle = TextureResourceMgr::Instance()->LoadGraph(KazFilePathName::CursorPath + "CursorBase.png");
	cursorFlameTex->data.handleData = flameHandle;

	//�����n���h���̕ۑ�
	DrawFuncData::PipelineGenerateData pipelineData = DrawFuncData::GetSpriteShader();
	pipelineData.desc.BlendState.AlphaToCoverageEnable = true;
	numberRender = DrawFuncData::SetTexPlaneData(pipelineData);
	numberTexBufferArray[0] = TextureResourceMgr::Instance()->LoadGraphBuffer(KazFilePathName::CursorPath + "CursorBase2.png");
	numberTexBufferArray[1] = TextureResourceMgr::Instance()->LoadGraphBuffer(KazFilePathName::CursorPath + "CursorNum1.png");
	numberTexBufferArray[2] = TextureResourceMgr::Instance()->LoadGraphBuffer(KazFilePathName::CursorPath + "CursorNum2.png");
	numberTexBufferArray[3] = TextureResourceMgr::Instance()->LoadGraphBuffer(KazFilePathName::CursorPath + "CursorNum3.png");
	numberTexBufferArray[4] = TextureResourceMgr::Instance()->LoadGraphBuffer(KazFilePathName::CursorPath + "CursorNum4.png");
	numberTexBufferArray[5] = TextureResourceMgr::Instance()->LoadGraphBuffer(KazFilePathName::CursorPath + "CursorNum5.png");
	numberTexBufferArray[6] = TextureResourceMgr::Instance()->LoadGraphBuffer(KazFilePathName::CursorPath + "CursorNum6.png");
	numberTexBufferArray[7] = TextureResourceMgr::Instance()->LoadGraphBuffer(KazFilePathName::CursorPath + "CursorNum7.png");
	numberTexBufferArray[8] = TextureResourceMgr::Instance()->LoadGraphBuffer(KazFilePathName::CursorPath + "CursorNumMax.png");


	flameRender = DrawFuncData::SetTexPlaneData(pipelineData);
	flameTextureBuffer = TextureResourceMgr::Instance()->LoadGraphBuffer(KazFilePathName::CursorPath + "CursorBase.png");


	cursorPos = { static_cast<float>(WIN_X) / 2.0f,static_cast<float>(WIN_Y) / 2.0f };
	limitValue = { 570.0f,static_cast<float>(WIN_Y) / 2.0f };

	KazMath::Vec2<float> scale = { 2.0f,2.0f };
	cursorFlameTex->data.transform.scale = scale;
	numberTex->data.transform.scale = scale;
	stopFlag = { 0,0 };
	baseScale = { 2.0f,2.0f };
	deadLine = 0.25f;

	//clickSoundHandle = SoundManager::Instance()->LoadSoundMem(KazFilePathName::SoundPath + "Push.wav", false);
	initClickSoundFlag = false;



	cursorFlameTex->data.pipelineName = PIPELINE_NAME_SPRITE_Z_ALWAYS;
	numberTex->data.pipelineName = PIPELINE_NAME_SPRITE_Z_ALWAYS;
}

void Cursor::Init()
{
	honraiCursorPos = { static_cast<float>(WIN_X) / 2.0f,static_cast<float>(WIN_Y) / 2.0f };
	cursorPos = { static_cast<float>(WIN_X) / 2.0f,static_cast<float>(WIN_Y) / 2.0f };

	cursorFlameTex->data.transform.scale = baseScale;
	numberTex->data.transform.scale = baseScale;
	stopFlag = { 0,0 };

	enableLockOnTimer = 0;
	notEnableLockOnFlag = false;
}

void Cursor::Input(bool UP_FLAG, bool DOWN_FLAG, bool LEFT_FLAG, bool RIGHT_FLAG, bool DONE_FLAG, bool RELEASE_FLAG, const KazMath::Vec2<float>& ANGLE, const KazMath::Vec2<float> &MOUSE_VEL)
{
	//UI���\������Ă�������͂����ׂĐ؂�B
	//if (OptionUI::Instance()->m_isDisplayUI) {
	//	upFlag = false;
	//	downFlag = false;
	//	leftFlag = false;
	//	rightFlag = false;
	//	doneFlag = false;
	//	releaseFlag = false;
	//	stickAngle = { 0.0f,0.0f };
	//	return;
	//}

	if (!dontMoveFlag)
	{
		upFlag = UP_FLAG;
		downFlag = DOWN_FLAG;
		leftFlag = LEFT_FLAG;
		rightFlag = RIGHT_FLAG;
		doneFlag = DONE_FLAG;
		releaseFlag = RELEASE_FLAG;
		stickAngle = ANGLE;
		mouseVel = MOUSE_VEL;
	}
	else
	{
		upFlag = false;
		downFlag = false;
		leftFlag = false;
		rightFlag = false;
		doneFlag = false;
		releaseFlag = false;
		stickAngle = { 0.0f,0.0f };
	}
}

void Cursor::Update()
{
	//�ő�l�Œ�
	if (LOCKON_MAX_NUM <= lockOnNum)
	{
		lockOnNum = 8;
	}

	if (doneFlag && !initClickSoundFlag)
	{
		//SoundManager::Instance()->PlaySoundMem(clickSoundHandle, 1);
		initClickSoundFlag = true;
	}

	//���b�N�I�����͂������[�X���ꂽ�珉��������
	if (releaseFlag)
	{
		enableLockOnTimer = 0;
		lockOnNum = 0;
		notEnableLockOnFlag = false;
		initClickSoundFlag = false;
	}




	//�ēx���b�N�I������܂ł̎���
	if (notEnableLockOnFlag)
	{
		++enableLockOnTimer;

		//�w��̃t���[���܂Ń^�C�}�[�����B�����珉����
		if (5 <= enableLockOnTimer)
		{
			enableLockOnTimer = 0;
			notEnableLockOnFlag = false;
		}
	}
	numberTex->data.handleData = numberHandle[lockOnNum];


	//�������b�N�I�����Ă��Ȃ����͒��S�̏\������
	if (lockOnNum == 0)
	{
		numberTex->data.transform.rotation += 2.0f;

		if (doneFlag) {
			numberTex->data.transform.rotation += 5.0f;
		}
	}
	else
	{
		numberTex->data.transform.rotation = 0.0f;
	}




	//�ړ�----------------------------------------------------------------
	vel = { 0.0f,0.0f };


	prevCursorPos = cursorPos;

	KazMath::Vec2<float> honraiStick = stickAngle;
	//�f�b�g���C��-----------------------
	if (fabs(stickAngle.x) < deadLine)
	{
		stickAngle.x = 0.0f;
	}
	else
	{
		stickAngle.x += -deadLine;
	}
	if (fabs(stickAngle.y) < deadLine)
	{
		stickAngle.y = 0.0f;
	}
	else
	{
		stickAngle.y += -deadLine;
	}
	//�f�b�g���C��-----------------------


	if (1.0f - deadLine <= fabs(stickAngle.x))
	{
		bool flag = signbit(stickAngle.x);
		if (flag)
		{
			stickAngle.x = -1.0f + deadLine;
		}
		else
		{
			stickAngle.x = 1.0f - deadLine;
		}
	}
	if (1.0f - deadLine <= fabs(stickAngle.y))
	{
		bool flag = signbit(stickAngle.y);
		if (flag)
		{
			stickAngle.y = -1.0f + deadLine;
		}
		else
		{
			stickAngle.y = 1.0f - deadLine;
		}
	}


	//�X�e�B�b�N�̑��삩��J�[�\���̃X�s�[�h�𒲐�
	baseSpeed = 10.0f;
	speed.x = baseSpeed * -stickAngle.x;
	speed.y = baseSpeed * stickAngle.y;
	if (mouseVel.Length() != 0.0f)
	{
		baseSpeed = 1.0f;
		speed.x = baseSpeed * -mouseVel.x;
		speed.y = baseSpeed * -mouseVel.y;
	}


	if (leftFlag)
	{
		vel.x = -speed.x;
		knockBackVal.x += KOCKBACK_VELOCITY.x;
		noTochFlag.x = 1;
	}
	if (rightFlag)
	{
		vel.x = -speed.x;
		knockBackVal.x += KOCKBACK_VELOCITY.x;
		noTochFlag.x = 1;
	}

	if (upFlag)
	{
		vel.y = -speed.y;
		knockBackVal.y += KOCKBACK_VELOCITY.y;
		noTochFlag.y = 1;
	}
	if (downFlag)
	{
		vel.y = -speed.y;
		knockBackVal.y += KOCKBACK_VELOCITY.y;
		noTochFlag.y = 1;
	}

	//�m�b�N�o�b�N�̐���
	if (KOCKBACK_MAX_VALUE.x <= knockBackVal.x)
	{
		knockBackVal.x = KOCKBACK_MAX_VALUE.x;
	}
	if (KOCKBACK_MAX_VALUE.y <= knockBackVal.y)
	{
		knockBackVal.y = KOCKBACK_MAX_VALUE.y;
	}


	if (!upFlag && !downFlag)
	{
		noTochFlag.y = 0;
	}
	if (!leftFlag && !rightFlag)
	{
		noTochFlag.x = 0;
	}

	if (!noTochFlag.x)
	{
		++noTockTimer.x;
	}
	else
	{
		noTockTimer.x = 0;
		oldVel.x = vel.x;
	}


	if (!noTochFlag.y)
	{
		++noTockTimer.y;
	}
	else
	{
		noTockTimer.y = 0;
		oldVel.y = vel.y;
	}


	//�m�b�N�o�b�N�̏���
	if (5 <= noTockTimer.x && false)
	{
		if (signbit(oldVel.x))
		{
			vel.x = knockBackVal.x;
		}
		else
		{
			vel.x = -knockBackVal.x;
		}
		knockBackVal.x = 0.0f;
	}
	if (5 <= noTockTimer.y && false)
	{
		if (signbit(oldVel.y))
		{
			vel.y = knockBackVal.y;
		}
		else
		{
			vel.y = -knockBackVal.y;
		}
		knockBackVal.y = 0.0f;
	}

	honraiCursorPos += vel;

	//��ʊO����o�Ȃ��悤�ɂ��鏈��
	if (honraiCursorPos.x < 0.0f)
	{
		honraiCursorPos.x = 0.0f;
	}
	if (WIN_X < honraiCursorPos.x)
	{
		honraiCursorPos.x = WIN_X;
	}

	if (honraiCursorPos.y < 0.0f)
	{
		honraiCursorPos.y = 0.0f;
	}
	if (WIN_Y < honraiCursorPos.y)
	{
		honraiCursorPos.y = WIN_Y;
	}


	//���
	KazMath::Vec2<float> distance = honraiCursorPos - cursorPos;
	cursorPos += distance * 0.5f;

	cursorFlameTex->data.transform.pos = { cursorPos.x,cursorPos.y };
	numberTex->data.transform.pos = { cursorPos.x,cursorPos.y };
	//�ړ�----------------------------------------------------------------


	//�J����������-----------------------
	//�ǔ�
	{
		//�J�[�\���̈ʒu���n�_�ƏI�_�ȓ��������ꍇ�A������͈͂̒ǔ�����߂ăJ�����ړ��ʂ�ǉ����Ȃ�
		if (dontMoveCameraStartPos.x < cursorPos.x &&
			cursorPos.x < dontMoveCameraEndPos.x)
		{

		}
		//�J�[�\���O�ɏo�悤�Ƃ����疳����͈͂̒ǔ����J�n����
		else
		{
			{
				float cursorStartDistanceX = dontMoveCameraStartPos.x - cursorPos.x;
				float cursorEndDistanceX = dontMoveCameraEndPos.x - cursorPos.x;
				//�I�_���n�_�̕����߂�������n�_����ɓ�����
				if (fabs(cursorStartDistanceX) < fabs(cursorEndDistanceX))
				{
					dontMoveCameraStartPos.x = cursorPos.x;
					dontMoveCameraEndPos.x = cursorPos.x + NO_MOVE_DISTANCE.x;
				}
				//�n�_���I�_�̕����߂�������I�_����ɓ�����
				else
				{
					dontMoveCameraStartPos.x = cursorPos.x - NO_MOVE_DISTANCE.x;
					dontMoveCameraEndPos.x = cursorPos.x;
				}
			}
			//�ړ��ʂ��J�����ɒǉ����Ă���
			honraiCameraMoveValue.x += prevCursorPos.x - cursorPos.x;
		}

		if (dontMoveCameraStartPos.y < cursorPos.y &&
			cursorPos.y < dontMoveCameraEndPos.y)
		{

		}
		//�J�[�\���O�ɏo�悤�Ƃ����疳����͈͂̒ǔ����J�n����
		else
		{
			{
				float cursorStartDistanceY = dontMoveCameraStartPos.y - cursorPos.y;
				float cursorEndDistanceY = dontMoveCameraEndPos.y - cursorPos.y;
				//�I�_���n�_�̕����߂�������n�_����ɓ�����
				if (fabs(cursorStartDistanceY) < fabs(cursorEndDistanceY))
				{
					dontMoveCameraStartPos.y = cursorPos.y;
					dontMoveCameraEndPos.y = cursorPos.y + NO_MOVE_DISTANCE.y;
				}
				//�n�_���I�_�̕����߂�������I�_����ɓ�����
				else
				{
					dontMoveCameraStartPos.y = cursorPos.y - NO_MOVE_DISTANCE.y;
					dontMoveCameraEndPos.y = cursorPos.y;
				}
			}
			//�ړ��ʂ��J�����ɒǉ����Ă���
			honraiCameraMoveValue.y += prevCursorPos.y - cursorPos.y;
		}
	}


	//�J�[�\��������----------------------------------------------
	if (disappearFlag)
	{
		int sub = 5;
		cursorAlpha -= sub;
		cursorFlameTex->data.pipelineName = PIPELINE_NAME_SPRITE_MULTITEX;
		//���S�ɏ�������J�[�\���̈ʒu������������----------------------------------------------
		//�J�[�\�����܂��������Ȃ�
		if (cursorAlpha <= 0)
		{
			dontMoveFlag = true;
			honraiCameraMoveValue = { 0.0f,0.0f };
			honraiCursorPos = { WIN_X / 2.0f,WIN_Y / 2.0f };
			cursorPos = honraiCursorPos;
			prevCursorPos = honraiCursorPos;
			dontMoveCameraStartPos = { 490.0f,310.0f };
			dontMoveCameraEndPos = { 640.0f,360.0f };
		}
		//���S�ɏ�������J�[�\���̈ʒu������������----------------------------------------------
	}
	else
	{
		cursorAlpha = 255;
		dontMoveFlag = false;
		//cursorFlameTex->data.pipelineName = PIPELINE_NAME_SPRITE_Z_ALWAYS;
	}

	numberTex->data.colorData.color.a = cursorAlpha;
	cursorFlameTex->data.colorData.color.a = cursorAlpha;
	//�J�[�\��������----------------------------------------------




	//�J�����̐���
	if (honraiCursorPos.x <= 0.0f)
	{
		float localLimit = 485.0f;
		honraiCameraMoveValue.x = localLimit;
	}
	if (WIN_X <= honraiCursorPos.x)
	{
		honraiCameraMoveValue.x = -limitValue.x;
	}

	if (honraiCursorPos.y <= 0.0f)
	{
		honraiCameraMoveValue.y = limitValue.y - NO_MOVE_DISTANCE.y;
	}
	if (WIN_Y <= honraiCursorPos.y)
	{
		honraiCameraMoveValue.y = -limitValue.y;
	}

	KazMath::Vec2<float> cameraDistance = honraiCameraMoveValue - cameraMoveValue;
	cameraMoveValue += cameraDistance * 0.5f;

	//�J����������-----------------------

	//�J�[�\�����o
	for (int i = 0; i < cursorEffectTex.size(); ++i)
	{
		if (releaseFlag && !cursorEffectTex[i].initFlag && !disappearFlag)
		{
			cursorEffectTex[i].cursorEffectTex.m_transform.pos = cursorPos;
			cursorEffectTex[i].cursorEffectTex.m_transform.scale = baseScale;
			cursorEffectTex[i].alpha = 255;
			cursorEffectTex[i].initFlag = true;
			break;
		}
	}

	for (int i = 0; i < cursorEffectTex.size(); ++i)
	{
		if (cursorEffectTex[i].initFlag)
		{
			cursorEffectTex[i].alpha -= 255 / 10;
			KazMath::Vec2<float> addSize = { 0.1f,0.1f };
			cursorEffectTex[i].cursorEffectTex.m_transform.scale += addSize;
		}
		if (cursorEffectTex[i].alpha <= 0)
		{
			cursorEffectTex[i].initFlag = false;
		}
	}


	//�����蔻��----------------------------------------------------------------
	KazMath::Vec3<float> nearPos = { cursorPos.x,cursorPos.y,0.0f };
	KazMath::Vec3<float> farPos = { cursorPos.x,cursorPos.y,1.0f };
	KazMath::Vec3<float> endPos = KazMath::ConvertScreenPosToWorldPos(farPos, CameraMgr::Instance()->GetViewMatrix(), CameraMgr::Instance()->GetPerspectiveMatProjection());

	hitBox.start = KazMath::ConvertScreenPosToWorldPos(nearPos, CameraMgr::Instance()->GetViewMatrix(), CameraMgr::Instance()->GetPerspectiveMatProjection());
	KazMath::Vec3<float> dir(endPos - hitBox.start);
	dir.Normalize();
	hitBox.dir = dir;
	//�����蔻��----------------------------------------------------------------


	for (int i = 0; i < boxEffectArray.size(); ++i)
	{
		if (boxEffectArray[i].IsAlive())
		{
			boxEffectArray[i].Update();
		}
	}

}

void Cursor::Draw(DrawingByRasterize& arg_rasterize)
{
	//UI���\������Ă�����`������ׂĐ؂�B
	//if (OptionUI::Instance()->m_isDisplayUI) {
	//	return;
	//}

	//����
	{
		KazMath::Transform2D transform;
		transform.pos = numberTex->data.transform.pos;
		transform.scale =
		{
			static_cast<float>(numberTexBufferArray[lockOnNum].bufferWrapper->GetBuffer()->GetDesc().Width) * 2.0f,
			static_cast<float>(numberTexBufferArray[lockOnNum].bufferWrapper->GetBuffer()->GetDesc().Height) * 2.0f
		};
		transform.rotation = numberTex->data.transform.rotation;
		DrawFunc::DrawTextureIn2D(numberRender, transform, numberTexBufferArray[lockOnNum]);
		arg_rasterize.ObjectRender(numberRender);
	}

	{
		KazMath::Transform2D transform;
		transform.pos = cursorFlameTex->data.transform.pos;
		transform.scale =
		{
			static_cast<float>(flameTextureBuffer.bufferWrapper->GetBuffer()->GetDesc().Width) * 2.0f,
			static_cast<float>(flameTextureBuffer.bufferWrapper->GetBuffer()->GetDesc().Height) * 2.0f
		};
		if (doneFlag) {
			transform.scale *= 0.9f;
		}
		DrawFunc::DrawTextureIn2D(flameRender, transform, flameTextureBuffer);
		arg_rasterize.ObjectRender(flameRender);
	}

	/*for (int i = 0; i < boxEffectArray.size(); ++i)
	{
		boxEffectArray[i].Draw();
	}*/

	for (int i = 0; i < cursorEffectTex.size(); ++i)
	{
		if (cursorEffectTex[i].initFlag)
		{
			KazMath::Transform2D transform;
			transform.pos = cursorFlameTex->data.transform.pos;
			transform.scale =
			{
				static_cast<float>(flameTextureBuffer.bufferWrapper->GetBuffer()->GetDesc().Width) *
				cursorEffectTex[i].cursorEffectTex.m_transform.scale.x,
				static_cast<float>(flameTextureBuffer.bufferWrapper->GetBuffer()->GetDesc().Height) *
				cursorEffectTex[i].cursorEffectTex.m_transform.scale.y
			};
			DrawFunc::DrawTextureIn2D(cursorEffectTex[i].cursorEffectTex.m_drawCommand, transform, cursorEffectTex[i].cursorEffectTex.m_textureBuffer, KazMath::Color(255, 255, 255, cursorEffectTex[i].alpha));
			arg_rasterize.ObjectRender(cursorEffectTex[i].cursorEffectTex.m_drawCommand);
		}
	}
	/*for (int i = 0; i < cursorEffectTex.size(); ++i)
	{
		if (cursorEffectTex[i].initFlag)
		{
			cursorEffectTex[i].cursorEffectTex->Draw();
		}
	}*/
}

bool Cursor::LockOn()
{
	//���b�N�I�������ő�l�����ł��邱��
	bool checkLockOnNumFlag = lockOnNum < LOCKON_MAX_NUM;
	//���b�N�I�����L���Ń��b�N�I���̓��͂�����Ă���
	bool enableToLockOnFlag = !notEnableLockOnFlag && doneFlag;

	//���̓�̏����������Ȃ�L��
	return checkLockOnNumFlag && enableToLockOnFlag;
}

bool Cursor::Release()
{
	return releaseFlag;
}

void Cursor::Hit(KazMath::Vec3<float>* POS)
{
	++lockOnNum;
	notEnableLockOnFlag = true;


	for (int i = 0; i < boxEffectArray.size(); ++i)
	{
		if (!boxEffectArray[i].IsAlive())
		{
			boxEffectArray[i].Start(POS);
			break;
		}
	}
}

const int& Cursor::GetCount()
{
	return lockOnNum;
}

KazMath::Vec2<float> Cursor::GetValue()
{
	//��ʒ�����0,0�̒��S���W�Ƃ���
	KazMath::Vec2<float> rateMaxValue = { WIN_X / 2.0f,WIN_Y / 2.0f };
	//�����v�Z
	KazMath::Vec2<float> value = cameraMoveValue / rateMaxValue;

	//�J�����̍��W�ɍ��킹��ׂ�X���̕����𔽓]
	value.x *= -1.0f;
	return value;
}

void Cursor::Disappear()
{
	disappearFlag = true;
}

void Cursor::Appear()
{
	disappearFlag = false;
}

void Cursor::SubCount(int SUB_VALUE)
{
	lockOnNum -= SUB_VALUE;
	if (lockOnNum <= -1)
	{
		lockOnNum = 0;
	}
}
