#include "DrawCallSet.h"

DrawCallSet::DrawCallSet() :m_outlineFlag(false)
{
}

void DrawCallSet::Load(std::string arg_filepath, std::string arg_filename, bool arg_isOpaque)
{

	m_modelData = ModelLoader::Instance()->Load(arg_filepath, arg_filename);
	m_model = DrawFuncData::SetDefferdRenderingModelAnimation(m_modelData, arg_isOpaque);
	m_boneIdentity = KazBufferHelper::SetConstBufferData(sizeof(DirectX::XMMATRIX) * 256);
	std::array<DirectX::XMMATRIX, 256>bone;
	for (auto& obj : bone)
	{
		obj = DirectX::XMMatrixIdentity();
	}
	m_boneIdentity.bufferWrapper->TransData(&bone, sizeof(DirectX::XMMATRIX) * 256);
	m_boneIdentity.rangeType = GRAPHICS_RANGE_TYPE_CBV_VIEW;
	m_boneIdentity.rootParamType = GRAPHICS_PRAMTYPE_DATA4;

}

void DrawCallSet::LoadNoLighting(std::string arg_filepath, std::string arg_filename, bool arg_isOpaque)
{

	m_modelData = ModelLoader::Instance()->Load(arg_filepath, arg_filename);
	m_model = DrawFuncData::SetDefferdRenderingModelAnimationNoLighting(m_modelData, arg_isOpaque);
	m_boneIdentity = KazBufferHelper::SetConstBufferData(sizeof(DirectX::XMMATRIX) * 256);
	std::array<DirectX::XMMATRIX, 256>bone;
	for (auto& obj : bone)
	{
		obj = DirectX::XMMatrixIdentity();
	}
	m_boneIdentity.bufferWrapper->TransData(&bone, sizeof(DirectX::XMMATRIX) * 256);
	m_boneIdentity.rangeType = GRAPHICS_RANGE_TYPE_CBV_VIEW;
	m_boneIdentity.rootParamType = GRAPHICS_PRAMTYPE_DATA4;

}

void DrawCallSet::LoadOutline(std::string arg_filepath, std::string arg_filename, bool arg_isOpaque)
{
	m_modelData = ModelLoader::Instance()->Load(arg_filepath, arg_filename);
	m_model = DrawFuncData::SetDefferdRenderingModelAnimationDissolve(m_modelData);
	m_boneIdentity = KazBufferHelper::SetConstBufferData(sizeof(DirectX::XMMATRIX) * 256);
	std::array<DirectX::XMMATRIX, 256>bone;
	for (auto& obj : bone)
	{
		obj = DirectX::XMMatrixIdentity();
	}
	m_boneIdentity.bufferWrapper->TransData(&bone, sizeof(DirectX::XMMATRIX) * 256);
	m_boneIdentity.rangeType = GRAPHICS_RANGE_TYPE_CBV_VIEW;
	m_boneIdentity.rootParamType = GRAPHICS_PRAMTYPE_DATA4;

	m_outlineFlag = true;
}

void DrawCallSet::LoadZAllways(std::string arg_filepath, std::string arg_filename, bool arg_isOpaque)
{
	m_modelData = ModelLoader::Instance()->Load(arg_filepath, arg_filename);
	m_model = DrawFuncData::SetDefferdRenderingModelAnimationZAllways(m_modelData, arg_isOpaque);
	m_boneIdentity = KazBufferHelper::SetConstBufferData(sizeof(DirectX::XMMATRIX) * 256);
	std::array<DirectX::XMMATRIX, 256>bone;
	for (auto& obj : bone)
	{
		obj = DirectX::XMMatrixIdentity();
	}
	m_boneIdentity.bufferWrapper->TransData(&bone, sizeof(DirectX::XMMATRIX) * 256);
	m_boneIdentity.rangeType = GRAPHICS_RANGE_TYPE_CBV_VIEW;
	m_boneIdentity.rootParamType = GRAPHICS_PRAMTYPE_DATA4;
}

void DrawCallSet::Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec, KazMath::Transform3D arg_transform, int arg_instanceID, bool m_isDrawRaytracing, const KazMath::Color& arg_color)
{
	if (m_outlineFlag)
	{
		DrawFunc::DrawModelDisolve(m_model, arg_transform, arg_color);
	}
	else
	{
		DrawFunc::DrawModel(m_model, arg_transform, arg_color);
	}
	m_model.extraBufferArray[3] = m_boneIdentity;
	arg_rasterize.ObjectRender(m_model);
	//if (m_isDrawRaytracing) {
	//	for (auto& index : m_model.m_raytracingData.m_blas)
	//	{
	//		arg_blasVec.Add(index, arg_transform.GetMat(), arg_instanceID);
	//	}
	//}

}
