#pragma once
#include <Raytracing/BlasVector.h>
#include <DrawFunc/DrawFuncHelper.h>
#include"../Game/Render/BasicDraw.h"

class DrawCallSet {

public:

	std::shared_ptr<ModelInfomation>m_modelData;
	DrawFuncData::DrawCallData m_model;
	KazBufferHelper::BufferData m_boneIdentity;

public:

	void Load(std::string arg_filepath, std::string arg_filename, bool arg_isOpaque = true);
	void LoadNoLighting(std::string arg_filepath, std::string arg_filename, bool arg_isOpaque = true);
	void LoadOutline(std::string arg_filepath, std::string arg_filename, bool arg_isOpaque = true);

	void Draw(DrawingByRasterize& arg_rasterize, Raytracing::BlasVector& arg_blasVec, KazMath::Transform3D arg_transform, int arg_instanceID = 0, bool m_isDrawRaytracing = true);

};