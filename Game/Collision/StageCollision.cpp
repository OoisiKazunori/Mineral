#include "StageCollision.h"

void StageCollision::Setting(std::string arg_filePath, std::string arg_fileName, KazMath::Transform3D arg_transform)
{

	DrawCallSet stage;
	stage.Load(arg_filePath, arg_fileName);

	m_stageCollision.Setting(stage.m_modelData->modelData[0].vertexData, arg_transform);


	DrawCallSet tutorialStage;
	tutorialStage.Load(arg_filePath, "TutorialStageCollision.gltf");

	m_tutorialStageCollision.Setting(tutorialStage.m_modelData->modelData[0].vertexData, arg_transform);

}
