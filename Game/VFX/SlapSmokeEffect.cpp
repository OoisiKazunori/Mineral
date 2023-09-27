#include "SlapSmokeEffect.h"
#include"../KazLibrary/Easing/easing.h"
#include"../KazLibrary/Imgui/MyImgui.h"

KazMath::Vec2<float> SlapSmokeEffect::s_smokeScale = { 3.0f,1.0f };
KazMath::Vec2<float> SlapSmokeEffect::s_smokeSubRange = { 10.0f,0.0f };
KazMath::Vec2<int> SlapSmokeEffect::s_smokeInterval = { 30,-10 };

SlapSmokeEffect::SlapSmokeEffect()
{
	std::vector<ShaderOptionData>shaderArray;
	shaderArray.emplace_back(ShaderOptionData(KazFilePathName::RelativeShaderPath + "ShaderFile/" + "InstanceModel.hlsl", "VSDefferdMain", "vs_6_4", SHADER_TYPE_VERTEX));
	shaderArray.emplace_back(ShaderOptionData(KazFilePathName::RelativeShaderPath + "ShaderFile/" + "InstanceModel.hlsl", "PSDefferdMain", "ps_6_4", SHADER_TYPE_PIXEL));
	m_drawSmokeRender = DrawFuncData::SetDefferdRenderingInstanceModel(ModelLoader::Instance()->Load("Resource/VFX/smoke/", "Slap_VFX.gltf"), shaderArray);
	for (auto& obj : m_drawSmokeRender.drawMultiMeshesIndexInstanceCommandData.drawIndexInstancedData)
	{
		obj.instanceCount = PARTICLE_MAX_NUM;
	}

	m_smokeWorldMatBuffer = KazBufferHelper::SetUploadBufferData(sizeof(CoordinateSpaceMatData) * PARTICLE_MAX_NUM, "SmokeParticleRAM");
	m_smokeWorldMatVRAMBuffer = KazBufferHelper::SetGPUBufferData(sizeof(CoordinateSpaceMatData) * PARTICLE_MAX_NUM, "SmokeParticleVRAM");
	m_smokeWorldMatVRAMBuffer.bufferWrapper->ChangeBarrier(D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	m_smokeWorldMatVRAMBuffer.rangeType = GRAPHICS_RANGE_TYPE_UAV_VIEW;
	m_smokeWorldMatVRAMBuffer.rootParamType = GRAPHICS_PRAMTYPE_DATA;
	m_drawSmokeRender.extraBufferArray[0] = m_smokeWorldMatVRAMBuffer;
}

void SlapSmokeEffect::Init(const KazMath::Vec3<float>& arg_emittPos, float arg_range, bool arg_isStrong)
{
	//�p�[�e�B�N���̔������ϓ��ɒu���A�c��̓����_���ɒu�����Ƃŉ~��ۂ����R�ȉ��Ɍ�����
	int circleParticleNum = (PARTICLE_MAX_NUM / 2);
	int angleVel = 360 / circleParticleNum;

	float addScale = 0.0f;
	float addRangeSub = 0.0f;
	if (arg_isStrong)
	{
		addScale = 3.0f;
		addRangeSub = s_smokeSubRange.x / 2;
	}

	//360�x�Ƀp�[�e�B�N�����΂�
	for (int i = 0; i < circleParticleNum; ++i)
	{
		//������x�p�[�e�B�N���̊Ԋu�����炷
		int angle = angleVel * i + KazMath::Rand<int>(s_smokeInterval.x, -s_smokeInterval.y);
		KazMath::Vec3<float>vel(cosf(KazMath::AngleToRadian(angle)), 0.0f, sinf(KazMath::AngleToRadian(angle)));
		//������
		m_particleArray[i].Init(arg_emittPos, vel, arg_range, KazMath::Rand<float>(4.0f + addScale, 2.0f + addScale));
	}

	for (int i = circleParticleNum; i < m_particleArray.size(); ++i)
	{
		//������x�p�[�e�B�N���̊Ԋu�����炷
		float angle = KazMath::Rand<float>(360.0f, 0.0f);
		KazMath::Vec3<float>vel(cosf(KazMath::AngleToRadian(angle)), 0.0f, sinf(KazMath::AngleToRadian(angle)));
		//������
		m_particleArray[i].Init(arg_emittPos, vel, arg_range - KazMath::Rand<float>(s_smokeSubRange.x + addRangeSub, s_smokeSubRange.y), KazMath::Rand<float>(2.0f + addScale / 2.0f, 1.0f + addScale / 2));
	}
}

void SlapSmokeEffect::Update()
{
	int index = 0;
	for (auto& obj : m_particleArray)
	{
		obj.Update();
		CoordinateSpaceMatData data(
			obj.m_transform.GetMat(),
			CameraMgr::Instance()->GetViewMatrix(),
			CameraMgr::Instance()->GetPerspectiveMatProjection()
		);
		data.m_rotaion = DirectX::XMMatrixIdentity();
		m_matArray[index] = data;
		++index;
	}
	m_smokeWorldMatBuffer.bufferWrapper->TransData(m_matArray.data(), sizeof(CoordinateSpaceMatData) * PARTICLE_MAX_NUM);
	m_smokeWorldMatVRAMBuffer.bufferWrapper->CopyBuffer(m_smokeWorldMatBuffer.bufferWrapper->GetBuffer());
}

void SlapSmokeEffect::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec)
{
	arg_rasterize.ObjectRender(m_drawSmokeRender);
	for (auto& obj : m_matArray)
	{
		arg_blasVec.Add(m_drawSmokeRender.m_raytracingData.m_blas[0], obj.m_world, 0);
	}

	//for (auto& obj : m_particleArray)
	//{
	//	obj.Draw(arg_rasterize, arg_blasVec);
	//}
}

void SlapSmokeEffect::DebugImGui()
{
	ImGui::Begin("Smoke");
	ImGui::DragFloat("SmokeScaleMax", &s_smokeScale.x);								//���̑傫���̍ő�l
	ImGui::DragFloat("SmokeScaleMin", &s_smokeScale.y);								//���̑傫���̍ŏ��l
	//���Ԃ̒P�ʂ�flame�œ���
	ImGui::DragInt("SmokeExpandingTime", &SmokeParticle::s_velRateTime);					//�����U���͈͂܂ōL���鎞��
	ImGui::DragInt("SmokeDisappearTime", &SmokeParticle::s_scaleRateTime);				//�����������鎞��
	ImGui::DragInt("SmokeReadyToDisappearTime", &SmokeParticle::s_disappearMaxTime);		//�����c�鎞��
	ImGui::DragFloat("RangeSubMax", &s_smokeSubRange.x);							//���͈̔͂�{�����ǂꂭ�炢���������邩�̍ő�l
	ImGui::DragFloat("RangeSubMin", &s_smokeSubRange.y);							//���͈̔͂�{�����ǂꂭ�炢���������邩�̍ŏ��l
	ImGui::DragInt("SmokeIntervalMax", &s_smokeInterval.x);
	ImGui::DragInt("SmokeIntervalMin", &s_smokeInterval.y);
	ImGui::End();
}
