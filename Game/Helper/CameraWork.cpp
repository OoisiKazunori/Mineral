#include"CameraWork.h"
#include"../KazLibrary/Input/KeyBoradInputManager.h"
#include"../Game/Effect/ShakeMgr.h"
#include<DirectXMath.h>
#include<algorithm>

CameraWork::CameraWork()
{
	besidePoly = std::make_unique<BoxPolygonRender>();
	verticlaPoly = std::make_unique<BoxPolygonRender>();
	cameraPoly = std::make_unique<BoxPolygonRender>();
}

void CameraWork::Init(const KazMath::Vec3<float> &BASE_POS)
{
	centralPos = BASE_POS;
	baseTargetPos = KazMath::Vec3<float>(0.0f, 3.0f, 0.0f);


	eyePos = { 0.0f,5.0f,-10.0f };
	leftRightAngleVel = { -90.0f,-90.0f };
	upDownAngleVel = { 0.0f,0.0f };
	mulValue = { 10.0f,30.0f };
	mulValue2 = { 60.0f,60.0f };


	trackLeftRightAngleVel = leftRightAngleVel;
	trackUpDownAngleVel = upDownAngleVel;

	r = 8.0f;
	r2 = 8.0f;

	forceCameraDirVel.x = FORCE_CAMERA_FRONT;
}

void CameraWork::Update(const KazMath::Vec2<float> &CURSOR_VALUE, KazMath::Vec3<float> *PLAYER_POS, KazMath::Vec3<float>& PLAYER_ROTATE, bool DEBUG_FLAG)
{
	if (!DEBUG_FLAG)
	{
#pragma region GameCamera

		upDownAngleVel =
		{
			mulValue.x * -CURSOR_VALUE.y,
			mulValue.y * -CURSOR_VALUE.y
		};
		leftRightAngleVel =
		{
			forceCameraDirVel.x + mulValue2.x * -CURSOR_VALUE.x,
			forceCameraDirVel.x + mulValue2.y * -CURSOR_VALUE.x
		};


		//���E�̊p�x�ύX�̃C�[�W���O
		{
			KazMath::Vec2<float> distance = leftRightAngleVel - trackLeftRightAngleVel;
			trackLeftRightAngleVel += distance * 0.1f;
		}
		//�㉺�̊p�x�ύX�̃C�[�W���O
		{
			KazMath::Vec2<float> distance = upDownAngleVel - trackUpDownAngleVel;
			trackUpDownAngleVel += distance * 0.1f;
		}
		//���E�̉�]
		besidePoly->data.transform.pos =
		{
			cosf(KazMath::AngleToRadian(trackLeftRightAngleVel.x)) * r,
			0.0f,
			sinf(KazMath::AngleToRadian(trackLeftRightAngleVel.y)) * r
		};
		//�㉺�̉�]
		verticlaPoly->data.transform.pos =
		{
			0.0f,
			trackUpDownAngleVel.x,
			0.0f,
		};

		//{
		//	float mul = 1.0f;
		//	float limitValue = 3.0f;
		//	//�㉺�Ńv���C���[�̂���ʒu��ς���
		//	if (0.1f <= CURSOR_VALUE.y)
		//	{
		//		mul *= -1.0f;
		//		limitValue = 2.5f;
		//	}

		//	//�|�[�^�����o�J�n������v���C���[�̓������Œ肷��
		//	DirectX::XMFLOAT3 rate = { CURSOR_VALUE.x ,CURSOR_VALUE.y,0.0f };
		//	honraiPlayerCameraPos.x = PLAYER_POS->x + (2.0f * -rate.x);
		//	honraiPlayerCameraPos.z = PLAYER_POS->z + (limitValue * rate.y) * mul;

		//	if (0.0f <= honraiPlayerCameraPos.z)
		//	{
		//		honraiPlayerCameraPos.z = 0.0f;
		//	}
		//	KazMath::Vec3<float> distance = honraiPlayerCameraPos - *PLAYER_POS;
		//	*PLAYER_POS += distance * 0.1f;
		//}

		//�㉺���E�̉�]
		cameraPoly->data.transform.pos = centralPos + (besidePoly->data.transform.pos + verticlaPoly->data.transform.pos);




		/*--------------------   ��������ǉ�*/


		//�J�����̊�ƂȂ���������߂邽�߂Ƀv���C���[�̉�]����N�H�[�^�j�I���𐶐�����B
		DirectX::XMVECTOR playerQ = DirectX::XMQuaternionRotationRollPitchYaw(DirectX::XMConvertToRadians(PLAYER_ROTATE.x), DirectX::XMConvertToRadians(PLAYER_ROTATE.y), DirectX::XMConvertToRadians(PLAYER_ROTATE.z));
		DirectX::XMVECTOR defaultPlayerQ = playerQ;

		//�J�������v���C���[�̌��ɒǏ]�����邽�߁A�v���C���[�̌��x�N�g�������߂�B
		DirectX::XMVECTOR playerBehindVec = DirectX::XMVector3Transform(DirectX::XMVectorSet(0,0,-1,0), DirectX::XMMatrixRotationQuaternion(playerQ));

		//eyePos = cameraPoly->data.transform.pos;
		const float PLAYER_CAMERA_DISTANCE = 10.0f;
		eyePos = *PLAYER_POS + KazMath::Vec3<float>(playerBehindVec.m128_f32[0], playerBehindVec.m128_f32[1], playerBehindVec.m128_f32[2]) * PLAYER_CAMERA_DISTANCE;

		//�J�[�\���ɂ���ăJ��������]�����邽�߂ɁA�E�x�N�g���Ə�x�N�g�������߂�B
		DirectX::XMVECTOR rightVec = DirectX::XMVector3Transform(DirectX::XMVectorSet(1, 0, 0, 0), DirectX::XMMatrixRotationQuaternion(playerQ));
		DirectX::XMVECTOR upVec = DirectX::XMVector3Transform(DirectX::XMVectorSet(0, 1, 0, 0), DirectX::XMMatrixRotationQuaternion(playerQ));

		//�J�[�\���̈ړ���(���W�A��)
		const float MAX_CURSOR_MOVE_RIGHT = 0.5f;
		const float MAX_CURSOR_MOVE_UP = 0.5f;

		//�㉺���E�����̉�]�ʂ����߂�B
		const float CURCOR_CLAMP = 0.5f;
		DirectX::XMVECTOR cameraRightQ = DirectX::XMQuaternionRotationAxis(upVec, std::clamp(CURSOR_VALUE.x, -CURCOR_CLAMP, CURCOR_CLAMP) * MAX_CURSOR_MOVE_RIGHT);
		DirectX::XMVECTOR cameraUpQ = DirectX::XMQuaternionRotationAxis(rightVec, -std::clamp(CURSOR_VALUE.y, -CURCOR_CLAMP, CURCOR_CLAMP) * MAX_CURSOR_MOVE_UP);

		//��]��K��������B
		//playerQ = DirectX::XMQuaternionMultiply(DirectX::XMQuaternionMultiply(playerQ, cameraUpQ), cameraRightQ);

		//���_�����ɍ��W�𓮂����B
		DirectX::XMVECTOR targetVector = DirectX::XMVector3Transform(DirectX::XMVectorSet(0, 0, 1, 0), DirectX::XMMatrixRotationQuaternion(playerQ));
		KazMath::Vec3<float> targetVec = KazMath::Vec3<float>(targetVector.m128_f32[0], targetVector.m128_f32[1], targetVector.m128_f32[2]);

		//targetPos = baseTargetPos;
		//targetPos = eyePos + targetVec * 10.0f;
		targetPos = eyePos + KazMath::Vec3<float>(0,0,1) * 10.0f;

		//�v���C���[����ʂ̉����Ɏ����Ă���B
		KazMath::Vec3<float> upVec3 = KazMath::Vec3<float>(upVec.m128_f32[0], upVec.m128_f32[1], upVec.m128_f32[2]);
		const float PLAYER_OFFSET = 3.0f;
		//eyePos += upVec3 * PLAYER_OFFSET;
		eyePos += ShakeMgr::Instance()->GetShakeVec3();
		//targetPos += upVec3 * PLAYER_OFFSET;

		//�J�[�\���̒l�ɂ���ăv���C���[��������x�X����B
		const float MAX_PLAYER_MOVE_RIGHT = 2.0f;
		const float MAX_PLAYER_MOVE_UP = 2.0f;
		DirectX::XMVECTOR playerRightQ = DirectX::XMQuaternionRotationAxis(upVec, std::clamp(CURSOR_VALUE.x, -CURCOR_CLAMP, CURCOR_CLAMP) * MAX_PLAYER_MOVE_RIGHT);
		DirectX::XMVECTOR playerUpQ = DirectX::XMQuaternionRotationAxis(rightVec, -std::clamp(CURSOR_VALUE.y, -CURCOR_CLAMP, CURCOR_CLAMP) * MAX_PLAYER_MOVE_UP);
		defaultPlayerQ = DirectX::XMQuaternionMultiply(DirectX::XMQuaternionMultiply(defaultPlayerQ, playerRightQ), playerUpQ);
		DirectX::XMVECTOR rotate, scale, position;
		DirectX::XMMatrixDecompose(&scale, &rotate, &position, DirectX::XMMatrixRotationQuaternion(defaultPlayerQ));
		//PLAYER_ROTATE = KazMath::Vec3<float>(DirectX::XMConvertToDegrees(rotate.m128_f32[0]), DirectX::XMConvertToDegrees(rotate.m128_f32[1]), DirectX::XMConvertToDegrees(rotate.m128_f32[2]));


#pragma endregion
	}
	else
	{
#pragma region DebugCamera
		KeyBoradInputManager *input = KeyBoradInputManager::Instance();

		debugCameraMove = { 0,0,0 };
		float debugSpeed = 5.0f;
		float debugSideSpeed = 5.0f;
		if (input->InputState(DIK_D))
		{
			debugCameraMove.x = -debugSpeed;
		}
		if (input->InputState(DIK_A))
		{
			debugCameraMove.x = debugSpeed;
		}
		if (input->InputState(DIK_W))
		{
			debugCameraMove.y = debugSpeed;
		}
		if (input->InputState(DIK_S))
		{
			debugCameraMove.y = -debugSpeed;
		}

		if (input->InputState(DIK_RIGHTARROW))
		{
			angle.x += debugSideSpeed;
		}
		if (input->InputState(DIK_LEFTARROW))
		{
			angle.x += -debugSideSpeed;
		}

		if (input->InputState(DIK_UPARROW))
		{
			angle.y += debugSideSpeed;
		}
		if (input->InputState(DIK_DOWNARROW))
		{
			angle.y += -debugSideSpeed;
		}
#pragma endregion
		//�f�o�b�N�p
		eyePos = KazMath::CaluEyePosForDebug(eyePos, debugCameraMove, angle, 0.1f);
		targetPos = KazMath::CaluTargetPosForDebug(eyePos, angle.x);
	}


}

KazMath::Vec3<float>& CameraWork::GetEyePos()
{
	return eyePos;
}

const KazMath::Vec3<float> CameraWork::GetTargetPos()
{
	return targetPos;
}

void CameraWork::ForceCamera(KazEnemyHelper::CameraDir DIR)
{
	switch (DIR)
	{
	case KazEnemyHelper::CAMERA_FRONT:
		forceCameraDirVel.x = FORCE_CAMERA_FRONT;
		break;
	case KazEnemyHelper::CAMERA_BACK:
		forceCameraDirVel.x = FORCE_CAMERA_BACK;
		break;
	case KazEnemyHelper::CAMERA_LEFT:
		forceCameraDirVel.x = FORCE_CAMERA_LEFT;
		break;
	case KazEnemyHelper::CAMERA_RIGHT:
		forceCameraDirVel.x = FORCE_CAMERA_RIGHT;
		break;
	default:
		break;
	}
}