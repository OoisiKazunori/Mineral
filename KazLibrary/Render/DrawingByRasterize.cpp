#include "DrawingByRasterize.h"
#include"../KazLibrary/DirectXCommon/DirectX12CmdList.h"
#include"../KazLibrary/Buffer/DescriptorHeapMgr.h"

//�e�X�g�p�A�p�C�v���C���̃n���h�����Ƀ\�[�g��������
int int_cmpr(const DrawFuncData::DrawData* a, const DrawFuncData::DrawData* b)
{
	RESOURCE_HANDLE lAHandle = a->pipelineHandle, lBHandle = b->pipelineHandle;

	if (lAHandle < lBHandle)
	{
		return 1;
	}
	else if (lBHandle < lAHandle)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}
//�����_�[�^�[�Q�b�g,�[�x,��,�p�C�v���C���̗D��x�Ń\�[�g����\��

DrawingByRasterize::DrawingByRasterize()
{
}

void DrawingByRasterize::ObjectRender(const DrawFuncData::DrawCallData& DRAW_DATA)
{
	kazCommandList.emplace_back(DRAW_DATA);
}

void DrawingByRasterize::UIRender(const DrawFuncData::DrawCallData& DRAW_DATA)
{
	m_uiKazCommandList.emplace_back(DRAW_DATA);
}

void DrawingByRasterize::UISort()
{
	m_uiRenderInfomationArray.clear();

	//�����_�[�^�[�Q�b�g���Ƀ\�[�g��������B
	m_uiKazCommandList.sort([](DrawFuncData::DrawCallData a, DrawFuncData::DrawCallData b)
		{
			RESOURCE_HANDLE lAHandle = a.renderTargetHandle, lBHandle = b.renderTargetHandle;
			if (lAHandle < lBHandle)
			{
				return true;
			}
			else if (lBHandle < lAHandle)
			{
				return false;
			}
			else
			{
				return false;
			}
		});

	//�\�[�g���I�������DirectX12�̃R�}���h���X�g�ɖ��ߏo����悤�ɕ`����𐶐�����B
	for (auto& callData : m_uiKazCommandList)
	{
		DrawFuncData::DrawData result;

		result.drawMultiMeshesIndexInstanceCommandData = callData.drawMultiMeshesIndexInstanceCommandData;
		result.drawInstanceCommandData = callData.drawInstanceCommandData;
		result.drawIndexInstanceCommandData = callData.drawIndexInstanceCommandData;
		result.m_executeIndirectGenerateData = callData.m_executeIndirectGenerateData;
		result.drawCommandType = callData.drawCommandType;

		result.materialBuffer = callData.materialBuffer;
		result.buffer = callData.extraBufferArray;
		result.renderTargetHandle = callData.renderTargetHandle;
		result.depthHandle = callData.depthHandle;

		result.pipelineData = callData.pipelineData.desc;

		result.m_executeIndirectGenerateData.m_uavArgumentBuffer = callData.m_executeIndirectGenerateData.m_uavArgumentBuffer;


		if (result.drawCommandType == DrawFuncData::VERT_TYPE::MULTI_MESHED)
		{
			bool debug = false;
		}

		//ExecuteIndirect�̔��s
		if (callData.drawCommandType == DrawFuncData::VERT_TYPE::EXECUTEINDIRECT_INDEX ||
			callData.drawCommandType == DrawFuncData::VERT_TYPE::EXECUTEINDIRECT_INSTANCE)
		{
			RootSignatureDataTest rootSignatureGenerateData;
			for (int i = 0; i < callData.m_executeIndirectGenerateData.m_desc.size(); ++i)
			{
				switch (callData.m_executeIndirectGenerateData.m_desc[i].Type)
				{
				case D3D12_INDIRECT_ARGUMENT_TYPE_CONSTANT_BUFFER_VIEW:
					rootSignatureGenerateData.rangeArray.emplace_back(
						GRAPHICS_RANGE_TYPE_CBV_VIEW, static_cast<GraphicsRootParamType>(GRAPHICS_PRAMTYPE_DATA + i)
					);
					break;
				case D3D12_INDIRECT_ARGUMENT_TYPE_SHADER_RESOURCE_VIEW:
					rootSignatureGenerateData.rangeArray.emplace_back(
						GRAPHICS_RANGE_TYPE_SRV_VIEW, static_cast<GraphicsRootParamType>(GRAPHICS_PRAMTYPE_DATA + i)
					);
					break;
				case D3D12_INDIRECT_ARGUMENT_TYPE_UNORDERED_ACCESS_VIEW:
					rootSignatureGenerateData.rangeArray.emplace_back(
						GRAPHICS_RANGE_TYPE_UAV_VIEW, static_cast<GraphicsRootParamType>(GRAPHICS_PRAMTYPE_DATA + i)
					);
					break;
				default:
					break;
				}
			}
			//���[�g�V�O�l�`���[
			result.m_commandRootsignatureHandle = rootSignatureBufferMgr.GenerateRootSignature(rootSignatureGenerateData);

			//�R�}���h�V�O�l�`������
			ExecuteIndirectData::GenerateCommandSignature(
				result.m_commandSignature,
				rootSignatureBufferMgr.GetBuffer(result.m_commandRootsignatureHandle),
				callData.m_executeIndirectGenerateData.m_desc
			);
		}

		for (UINT i = 0; i < result.pipelineData.NumRenderTargets; ++i)
		{
			switch (callData.pipelineData.blendMode)
			{
			case DrawFuncPipelineData::PipelineBlendModeEnum::ALPHA:
				result.pipelineData.BlendState.RenderTarget[i] = DrawFuncPipelineData::SetAlphaBlend();
				break;
			case DrawFuncPipelineData::PipelineBlendModeEnum::ADD:
				result.pipelineData.BlendState.RenderTarget[i] = DrawFuncPipelineData::SetAddBlend();
				break;
			case DrawFuncPipelineData::PipelineBlendModeEnum::SUB:
				result.pipelineData.BlendState.RenderTarget[i] = DrawFuncPipelineData::SetSubBlend();
				break;
			case DrawFuncPipelineData::PipelineBlendModeEnum::NONE:
				result.pipelineData.BlendState.RenderTarget[i].BlendEnable = false;
				break;
			default:
				break;
			}
		}


		//�V�F�[�_�[�̃R���p�C��
		for (int i = 0; i < callData.pipelineData.shaderDataArray.size(); ++i)
		{
			RESOURCE_HANDLE lShaderHandle = shaderBufferMgr.GenerateShader(callData.pipelineData.shaderDataArray[i]);
			ErrorCheck(lShaderHandle, callData.callLocation);

			result.shaderHandleArray.emplace_back(lShaderHandle);
			D3D12_SHADER_BYTECODE shaderByteCode = CD3DX12_SHADER_BYTECODE(shaderBufferMgr.GetBuffer(lShaderHandle)->GetBufferPointer(), shaderBufferMgr.GetBuffer(lShaderHandle)->GetBufferSize());
			switch (callData.pipelineData.shaderDataArray[i].shaderType)
			{
			case SHADER_TYPE_VERTEX:
				result.pipelineData.VS = shaderByteCode;
				break;
			case SHADER_TYPE_PIXEL:
				result.pipelineData.PS = shaderByteCode;
				break;
			case SHADER_TYPE_GEOMETORY:
				result.pipelineData.GS = shaderByteCode;
				break;
			default:
				break;
			}
		}

		RootSignatureDataTest rootSignatureGenerateData;
		if (callData.materialBuffer.size() != 0)
		{
			const int FIRST_MESH_INDEX = 0;
			//�}�e���A���o�b�t�@�����ă��[�g�V�O�l�`���[�̏��l�ߍ���
			//�S���b�V�����ʂœ���}�e���A�����̃X�^�b�N�����ă��[�g�V�O�l�`���[�̍ŏ��ɐς߂�
			for (int i = 0; i < MATERIAL_TEXTURE_MAX; ++i)
			{
				rootSignatureGenerateData.rangeArray.emplace_back
				(
					GRAPHICS_RANGE_TYPE_SRV_DESC,
					callData.materialBuffer[FIRST_MESH_INDEX][i].rootParamType
				);
			}
		}
		//���̑��o�b�t�@�����ă��[�g�V�O�l�`���[�̏��l�ߍ���
		for (int i = 0; i < callData.extraBufferArray.size(); ++i)
		{
			rootSignatureGenerateData.rangeArray.emplace_back
			(
				callData.extraBufferArray[i].rangeType,
				callData.extraBufferArray[i].rootParamType
			);
		}
		result.m_rootsignatureHandle = rootSignatureBufferMgr.GenerateRootSignature(rootSignatureGenerateData);

		//�p�C�v���C���̐���
		result.pipelineData.pRootSignature = rootSignatureBufferMgr.GetBuffer(result.m_rootsignatureHandle).Get();
		result.pipelineHandle = piplineBufferMgr.GeneratePipeline(
			result.pipelineData,
			PipelineDuplicateBlocking::PipelineDuplicateData(
				rootSignatureGenerateData, callData.pipelineData.shaderDataArray, callData.pipelineData.blendMode
			)
		);
		ErrorCheck(result.pipelineHandle, callData.callLocation);

		m_uiRenderInfomationArray.emplace_back(result);
	}
	m_uiKazCommandList.clear();
}

void DrawingByRasterize::Sort()
{
	renderInfomationForDirectX12Array.clear();

	//�����_�[�^�[�Q�b�g���Ƀ\�[�g��������B
	kazCommandList.sort([](DrawFuncData::DrawCallData a, DrawFuncData::DrawCallData b)
		{
			RESOURCE_HANDLE lAHandle = a.renderTargetHandle, lBHandle = b.renderTargetHandle;
			if (lAHandle < lBHandle)
			{
				return true;
			}
			else if (lBHandle < lAHandle)
			{
				return false;
			}
			else
			{
				return false;
			}
		});

	//�\�[�g���I�������DirectX12�̃R�}���h���X�g�ɖ��ߏo����悤�ɕ`����𐶐�����B
	for (auto& callData : kazCommandList)
	{
		DrawFuncData::DrawData result;

		result.drawMultiMeshesIndexInstanceCommandData = callData.drawMultiMeshesIndexInstanceCommandData;
		result.drawInstanceCommandData = callData.drawInstanceCommandData;
		result.drawIndexInstanceCommandData = callData.drawIndexInstanceCommandData;
		result.m_executeIndirectGenerateData = callData.m_executeIndirectGenerateData;
		result.drawCommandType = callData.drawCommandType;

		result.materialBuffer = callData.materialBuffer;
		result.buffer = callData.extraBufferArray;
		result.renderTargetHandle = callData.renderTargetHandle;
		result.depthHandle = callData.depthHandle;

		result.pipelineData = callData.pipelineData.desc;

		result.m_executeIndirectGenerateData.m_uavArgumentBuffer = callData.m_executeIndirectGenerateData.m_uavArgumentBuffer;


		if (result.drawCommandType == DrawFuncData::VERT_TYPE::MULTI_MESHED)
		{
			bool debug = false;
		}

		//ExecuteIndirect�̔��s
		if (callData.drawCommandType == DrawFuncData::VERT_TYPE::EXECUTEINDIRECT_INDEX ||
			callData.drawCommandType == DrawFuncData::VERT_TYPE::EXECUTEINDIRECT_INSTANCE)
		{
			RootSignatureDataTest rootSignatureGenerateData;
			for (int i = 0; i < callData.m_executeIndirectGenerateData.m_desc.size(); ++i)
			{
				switch (callData.m_executeIndirectGenerateData.m_desc[i].Type)
				{
				case D3D12_INDIRECT_ARGUMENT_TYPE_CONSTANT_BUFFER_VIEW:
					rootSignatureGenerateData.rangeArray.emplace_back(
						GRAPHICS_RANGE_TYPE_CBV_VIEW, static_cast<GraphicsRootParamType>(GRAPHICS_PRAMTYPE_DATA + i)
					);
					break;
				case D3D12_INDIRECT_ARGUMENT_TYPE_SHADER_RESOURCE_VIEW:
					rootSignatureGenerateData.rangeArray.emplace_back(
						GRAPHICS_RANGE_TYPE_SRV_VIEW, static_cast<GraphicsRootParamType>(GRAPHICS_PRAMTYPE_DATA + i)
					);
					break;
				case D3D12_INDIRECT_ARGUMENT_TYPE_UNORDERED_ACCESS_VIEW:
					rootSignatureGenerateData.rangeArray.emplace_back(
						GRAPHICS_RANGE_TYPE_UAV_VIEW, static_cast<GraphicsRootParamType>(GRAPHICS_PRAMTYPE_DATA + i)
					);
					break;
				default:
					break;
				}
			}
			//���[�g�V�O�l�`���[
			result.m_commandRootsignatureHandle = rootSignatureBufferMgr.GenerateRootSignature(rootSignatureGenerateData);

			//�R�}���h�V�O�l�`������
			ExecuteIndirectData::GenerateCommandSignature(
				result.m_commandSignature,
				rootSignatureBufferMgr.GetBuffer(result.m_commandRootsignatureHandle),
				callData.m_executeIndirectGenerateData.m_desc
			);
		}

		for (UINT i = 0; i < result.pipelineData.NumRenderTargets; ++i)
		{
			switch (callData.pipelineData.blendMode)
			{
			case DrawFuncPipelineData::PipelineBlendModeEnum::ALPHA:
				result.pipelineData.BlendState.RenderTarget[i] = DrawFuncPipelineData::SetAlphaBlend();
				break;
			case DrawFuncPipelineData::PipelineBlendModeEnum::ADD:
				result.pipelineData.BlendState.RenderTarget[i] = DrawFuncPipelineData::SetAddBlend();
				break;
			case DrawFuncPipelineData::PipelineBlendModeEnum::SUB:
				result.pipelineData.BlendState.RenderTarget[i] = DrawFuncPipelineData::SetSubBlend();
				break;
			case DrawFuncPipelineData::PipelineBlendModeEnum::NONE:
				result.pipelineData.BlendState.RenderTarget[i].BlendEnable = false;
				break;
			default:
				break;
			}
		}


		//�V�F�[�_�[�̃R���p�C��
		for (int i = 0; i < callData.pipelineData.shaderDataArray.size(); ++i)
		{
			RESOURCE_HANDLE lShaderHandle = shaderBufferMgr.GenerateShader(callData.pipelineData.shaderDataArray[i]);
			ErrorCheck(lShaderHandle, callData.callLocation);

			result.shaderHandleArray.emplace_back(lShaderHandle);
			D3D12_SHADER_BYTECODE shaderByteCode = CD3DX12_SHADER_BYTECODE(shaderBufferMgr.GetBuffer(lShaderHandle)->GetBufferPointer(), shaderBufferMgr.GetBuffer(lShaderHandle)->GetBufferSize());
			switch (callData.pipelineData.shaderDataArray[i].shaderType)
			{
			case SHADER_TYPE_VERTEX:
				result.pipelineData.VS = shaderByteCode;
				break;
			case SHADER_TYPE_PIXEL:
				result.pipelineData.PS = shaderByteCode;
				break;
			case SHADER_TYPE_GEOMETORY:
				result.pipelineData.GS = shaderByteCode;
				break;
			default:
				break;
			}
		}

		RootSignatureDataTest rootSignatureGenerateData;
		if (callData.materialBuffer.size() != 0)
		{
			const int FIRST_MESH_INDEX = 0;
			//�}�e���A���o�b�t�@�����ă��[�g�V�O�l�`���[�̏��l�ߍ���
			//�S���b�V�����ʂœ���}�e���A�����̃X�^�b�N�����ă��[�g�V�O�l�`���[�̍ŏ��ɐς߂�
			for (int i = 0; i < MATERIAL_TEXTURE_MAX; ++i)
			{
				rootSignatureGenerateData.rangeArray.emplace_back
				(
					GRAPHICS_RANGE_TYPE_SRV_DESC,
					callData.materialBuffer[FIRST_MESH_INDEX][i].rootParamType
				);
			}
		}
		//���̑��o�b�t�@�����ă��[�g�V�O�l�`���[�̏��l�ߍ���
		for (int i = 0; i < callData.extraBufferArray.size(); ++i)
		{
			rootSignatureGenerateData.rangeArray.emplace_back
			(
				callData.extraBufferArray[i].rangeType,
				callData.extraBufferArray[i].rootParamType
			);
		}
		result.m_rootsignatureHandle = rootSignatureBufferMgr.GenerateRootSignature(rootSignatureGenerateData);

		//�p�C�v���C���̐���
		result.pipelineData.pRootSignature = rootSignatureBufferMgr.GetBuffer(result.m_rootsignatureHandle).Get();
		result.pipelineHandle = piplineBufferMgr.GeneratePipeline(
			result.pipelineData,
			PipelineDuplicateBlocking::PipelineDuplicateData(
				rootSignatureGenerateData, callData.pipelineData.shaderDataArray, callData.pipelineData.blendMode
			)
		);
		ErrorCheck(result.pipelineHandle, callData.callLocation);

		renderInfomationForDirectX12Array.emplace_back(result);
	}
	kazCommandList.clear();
}

void DrawingByRasterize::Render()
{
	RenderTargetStatus::Instance()->SetDoubleBufferFlame();
	//RenderTargetStatus::Instance()->ClearDoubuleBuffer(BG_COLOR);
	RenderTargetStatus::Instance()->ClearDoubuleBuffer(DirectX::XMFLOAT3(100, 100, 100));


	RESOURCE_HANDLE preRenderTargetHandle = -1;
	RESOURCE_HANDLE preDepthHandle = -1;
	for (auto& renderData : renderInfomationForDirectX12Array)
	{
		//�O��ƈႤ�n���h�����o���烌���_�[�^�[�Q�b�g��؂�ւ���
		if (renderData.renderTargetHandle != preRenderTargetHandle && renderData.depthHandle == -1)
		{
			RenderTargetStatus::Instance()->PrepareToChangeBarrier(renderData.renderTargetHandle, preRenderTargetHandle);
			RenderTargetStatus::Instance()->ClearRenderTarget(renderData.renderTargetHandle);
		}
		//�����_�[�^�[�Q�b�g�؂�ւ����Ƀf�v�X�ɏ�������
		if (renderData.depthHandle != preDepthHandle)
		{
			RenderTargetStatus::Instance()->SetDepth(renderData.renderTargetHandle);
		}
		preDepthHandle = renderData.depthHandle;
		preRenderTargetHandle = renderData.renderTargetHandle;


		RESOURCE_HANDLE pipelineHandle = renderData.pipelineHandle;
		RESOURCE_HANDLE rootSignatureHandle = -1;
		//�ʏ�̕`��
		if (renderData.drawCommandType != DrawFuncData::VERT_TYPE::EXECUTEINDIRECT_INDEX)
		{
			rootSignatureHandle = renderData.m_rootsignatureHandle;

			DirectX12CmdList::Instance()->cmdList->SetGraphicsRootSignature(
				rootSignatureBufferMgr.GetBuffer(rootSignatureHandle).Get()
			);
			DirectX12CmdList::Instance()->cmdList->SetPipelineState(
				piplineBufferMgr.GetBuffer(pipelineHandle).Get()
			);
			SetBufferOnCmdList(renderData.buffer, rootSignatureBufferMgr.GetRootParam(rootSignatureHandle));
		}
		//ExcuteIndirect�̎g�p
		else
		{
			rootSignatureHandle = renderData.m_rootsignatureHandle;

			DirectX12CmdList::Instance()->cmdList->SetGraphicsRootSignature(
				rootSignatureBufferMgr.GetBuffer(rootSignatureHandle).Get()
			);
			DirectX12CmdList::Instance()->cmdList->SetPipelineState(
				piplineBufferMgr.GetBuffer(pipelineHandle).Get()
			);
			//SetBufferOnCmdList(renderData.buffer, rootSignatureBufferMgr.GetRootParam(rootSignatureHandle));
		}

		//�`��R�}���h���s
		switch (renderData.drawCommandType)
		{
		case DrawFuncData::VERT_TYPE::INDEX:
			DrawIndexInstanceCommand(renderData.drawIndexInstanceCommandData);
			break;
		case DrawFuncData::VERT_TYPE::INSTANCE:
			DrawInstanceCommand(renderData.drawInstanceCommandData);
			break;
		case DrawFuncData::VERT_TYPE::MULTI_MESHED:
			MultiMeshedDrawIndexInstanceCommand(renderData.drawMultiMeshesIndexInstanceCommandData, renderData.materialBuffer, rootSignatureBufferMgr.GetRootParam(rootSignatureHandle));
			break;
		case DrawFuncData::VERT_TYPE::EXECUTEINDIRECT_INDEX:
			DrawExecuteIndirect(renderData.drawMultiMeshesIndexInstanceCommandData, renderData.m_commandSignature, renderData.m_executeIndirectGenerateData);
			break;
		case DrawFuncData::VERT_TYPE::EXECUTEINDIRECT_INSTANCE:
			DrawExecuteIndirect(renderData.drawMultiMeshesIndexInstanceCommandData, renderData.m_commandSignature, renderData.m_executeIndirectGenerateData);
			break;
		default:
			break;
		}
	}

	if (preRenderTargetHandle != -1)
	{
		RenderTargetStatus::Instance()->PrepareToCloseBarrier(preRenderTargetHandle);
		RenderTargetStatus::Instance()->SetDoubleBufferFlame();
		RenderTargetStatus::Instance()->ClearDoubuleBuffer(DirectX::XMFLOAT3(100, 100, 100));
	}
}

void DrawingByRasterize::RenderAfterBackBuffer()
{
	RESOURCE_HANDLE preRenderTargetHandle = -1;
	RESOURCE_HANDLE preDepthHandle = -1;
	for (auto& renderData : m_uiRenderInfomationArray)
	{
		//�O��ƈႤ�n���h�����o���烌���_�[�^�[�Q�b�g��؂�ւ���
		if (renderData.renderTargetHandle != preRenderTargetHandle && renderData.depthHandle == -1)
		{
			RenderTargetStatus::Instance()->PrepareToChangeBarrier(renderData.renderTargetHandle, preRenderTargetHandle);
			RenderTargetStatus::Instance()->ClearRenderTarget(renderData.renderTargetHandle);
		}
		//�����_�[�^�[�Q�b�g�؂�ւ����Ƀf�v�X�ɏ�������
		if (renderData.depthHandle != preDepthHandle)
		{
			RenderTargetStatus::Instance()->SetDepth(renderData.renderTargetHandle);
		}
		preDepthHandle = renderData.depthHandle;
		preRenderTargetHandle = renderData.renderTargetHandle;


		RESOURCE_HANDLE pipelineHandle = renderData.pipelineHandle;
		RESOURCE_HANDLE rootSignatureHandle = -1;
		//�ʏ�̕`��
		if (renderData.drawCommandType != DrawFuncData::VERT_TYPE::EXECUTEINDIRECT_INDEX)
		{
			rootSignatureHandle = renderData.m_rootsignatureHandle;

			DirectX12CmdList::Instance()->cmdList->SetGraphicsRootSignature(
				rootSignatureBufferMgr.GetBuffer(rootSignatureHandle).Get()
			);
			DirectX12CmdList::Instance()->cmdList->SetPipelineState(
				piplineBufferMgr.GetBuffer(pipelineHandle).Get()
			);
			SetBufferOnCmdList(renderData.buffer, rootSignatureBufferMgr.GetRootParam(rootSignatureHandle));
		}
		//ExcuteIndirect�̎g�p
		else
		{
			rootSignatureHandle = renderData.m_rootsignatureHandle;

			DirectX12CmdList::Instance()->cmdList->SetGraphicsRootSignature(
				rootSignatureBufferMgr.GetBuffer(rootSignatureHandle).Get()
			);
			DirectX12CmdList::Instance()->cmdList->SetPipelineState(
				piplineBufferMgr.GetBuffer(pipelineHandle).Get()
			);
			//SetBufferOnCmdList(renderData.buffer, rootSignatureBufferMgr.GetRootParam(rootSignatureHandle));
		}

		//�`��R�}���h���s
		switch (renderData.drawCommandType)
		{
		case DrawFuncData::VERT_TYPE::INDEX:
			DrawIndexInstanceCommand(renderData.drawIndexInstanceCommandData);
			break;
		case DrawFuncData::VERT_TYPE::INSTANCE:
			DrawInstanceCommand(renderData.drawInstanceCommandData);
			break;
		case DrawFuncData::VERT_TYPE::MULTI_MESHED:
			MultiMeshedDrawIndexInstanceCommand(renderData.drawMultiMeshesIndexInstanceCommandData, renderData.materialBuffer, rootSignatureBufferMgr.GetRootParam(rootSignatureHandle));
			break;
		case DrawFuncData::VERT_TYPE::EXECUTEINDIRECT_INDEX:
			DrawExecuteIndirect(renderData.drawMultiMeshesIndexInstanceCommandData, renderData.m_commandSignature, renderData.m_executeIndirectGenerateData);
			break;
		case DrawFuncData::VERT_TYPE::EXECUTEINDIRECT_INSTANCE:
			DrawExecuteIndirect(renderData.drawMultiMeshesIndexInstanceCommandData, renderData.m_commandSignature, renderData.m_executeIndirectGenerateData);
			break;
		default:
			break;
		}
	}

	if (preRenderTargetHandle != -1)
	{
		RenderTargetStatus::Instance()->PrepareToCloseBarrier(preRenderTargetHandle);
		RenderTargetStatus::Instance()->SetDoubleBufferFlame();
	}

	shaderBufferMgr.Update();
	rootSignatureBufferMgr.Update();
	piplineBufferMgr.Update();
}

void DrawingByRasterize::SetBufferOnCmdList(const std::vector<KazBufferHelper::BufferData>& BUFFER_ARRAY, std::vector<RootSignatureParameter> ROOT_PARAM)
{
	for (int i = 0; i < BUFFER_ARRAY.size(); ++i)
	{
		const int L_PARAM = KazRenderHelper::SetBufferOnCmdList(ROOT_PARAM, BUFFER_ARRAY[i].rangeType, BUFFER_ARRAY[i].rootParamType);

		if (L_PARAM == -1)
		{
			continue;
		}
		//�f�X�N���v�^�q�[�v�ɃR�}���h���X�g�ɐςށB�]�肪�����Ȃ�f�X�N���v�^�q�[�v���Ɣ��f����
		if (BUFFER_ARRAY[i].rangeType % 2 == 0)
		{
			DirectX12CmdList::Instance()->cmdList->SetGraphicsRootDescriptorTable(L_PARAM, DescriptorHeapMgr::Instance()->GetGpuDescriptorView(BUFFER_ARRAY[i].bufferWrapper->GetViewHandle()));
			continue;
		}

		//�r���[�Őςލۂ͂��ꂼ��̎�ނɍ��킹�ăR�}���h���X�g�ɐς�
		switch (BUFFER_ARRAY[i].rangeType)
		{
		case GRAPHICS_RANGE_TYPE_SRV_VIEW:
			DirectX12CmdList::Instance()->cmdList->SetGraphicsRootShaderResourceView(L_PARAM, BUFFER_ARRAY[i].bufferWrapper->GetBuffer()->GetGPUVirtualAddress());
			break;
		case GRAPHICS_RANGE_TYPE_UAV_VIEW:
			DirectX12CmdList::Instance()->cmdList->SetGraphicsRootUnorderedAccessView(L_PARAM, BUFFER_ARRAY[i].bufferWrapper->GetBuffer()->GetGPUVirtualAddress());
			break;
		case GRAPHICS_RANGE_TYPE_CBV_VIEW:
			DirectX12CmdList::Instance()->cmdList->SetGraphicsRootConstantBufferView(L_PARAM, BUFFER_ARRAY[i].bufferWrapper->GetBuffer()->GetGPUVirtualAddress());
			break;
		default:
			break;
		}
	}
}

void DrawingByRasterize::MultiMeshedDrawIndexInstanceCommand(const KazRenderHelper::MultipleMeshesDrawIndexInstanceCommandData& DATA, const std::vector<std::vector<KazBufferHelper::BufferData>>& MATERIAL_BUFFER, std::vector<RootSignatureParameter> ROOT_PARAM)
{
	//�`�施��-----------------------------------------------------------------------------------------------------
	const int COMMAND_MAX_DATA = static_cast<int>(DATA.vertexBufferDrawData.size());

	for (int i = 0; i < COMMAND_MAX_DATA; ++i)
	{
		//�}�e���A�����̃X�^�b�N
		if (MATERIAL_BUFFER.size() != 0)
		{
			SetBufferOnCmdList(MATERIAL_BUFFER[i], ROOT_PARAM);
		}

		if (DATA.drawIndexInstancedData[i].indexCountPerInstance == 0)
		{
			DirectX12CmdList::Instance()->cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
			DirectX12CmdList::Instance()->cmdList->IASetVertexBuffers(DATA.vertexBufferDrawData[i].slot, DATA.vertexBufferDrawData[i].numViews, &DATA.vertexBufferDrawData[i].vertexBufferView);
			DirectX12CmdList::Instance()->cmdList->DrawInstanced(
				2,
				DATA.drawIndexInstancedData[i].instanceCount,
				0,
				0
			);
			continue;
		}

		DirectX12CmdList::Instance()->cmdList->IASetPrimitiveTopology(DATA.topology);
		DirectX12CmdList::Instance()->cmdList->IASetVertexBuffers(DATA.vertexBufferDrawData[i].slot, DATA.vertexBufferDrawData[i].numViews, &DATA.vertexBufferDrawData[i].vertexBufferView);
		DirectX12CmdList::Instance()->cmdList->IASetIndexBuffer(&DATA.indexBufferView[i]);
		DirectX12CmdList::Instance()->cmdList->DrawIndexedInstanced(
			DATA.drawIndexInstancedData[i].indexCountPerInstance,
			DATA.drawIndexInstancedData[i].instanceCount,
			DATA.drawIndexInstancedData[i].startIndexLocation,
			DATA.drawIndexInstancedData[i].baseVertexLocation,
			DATA.drawIndexInstancedData[i].startInstanceLocation
		);
	}
	//�`�施��-----------------------------------------------------------------------------------------------------
}

void DrawingByRasterize::DrawIndexInstanceCommand(const KazRenderHelper::DrawIndexInstanceCommandData& DATA)
{
	//�`�施��-----------------------------------------------------------------------------------------------------
	DirectX12CmdList::Instance()->cmdList->IASetPrimitiveTopology(DATA.topology);
	DirectX12CmdList::Instance()->cmdList->IASetVertexBuffers(DATA.vertexBufferDrawData.slot, DATA.vertexBufferDrawData.numViews, &DATA.vertexBufferDrawData.vertexBufferView);
	DirectX12CmdList::Instance()->cmdList->IASetIndexBuffer(&DATA.indexBufferView);
	DirectX12CmdList::Instance()->cmdList->DrawIndexedInstanced(
		DATA.drawIndexInstancedData.indexCountPerInstance,
		DATA.drawIndexInstancedData.instanceCount,
		DATA.drawIndexInstancedData.startIndexLocation,
		DATA.drawIndexInstancedData.baseVertexLocation,
		DATA.drawIndexInstancedData.startInstanceLocation
	);
	//�`�施��-----------------------------------------------------------------------------------------------------
}

void DrawingByRasterize::DrawInstanceCommand(const KazRenderHelper::DrawInstanceCommandData& DATA)
{
	DirectX12CmdList::Instance()->cmdList->IASetPrimitiveTopology(DATA.topology);
	DirectX12CmdList::Instance()->cmdList->IASetVertexBuffers(DATA.vertexBufferDrawData.slot, DATA.vertexBufferDrawData.numViews, &DATA.vertexBufferDrawData.vertexBufferView);
	DirectX12CmdList::Instance()->cmdList->DrawInstanced(
		DATA.drawInstanceData.vertexCountPerInstance,
		DATA.drawInstanceData.instanceCount,
		DATA.drawInstanceData.startVertexLocation,
		DATA.drawInstanceData.startInstanceLocation
	);
}

void DrawingByRasterize::DrawExecuteIndirect(const KazRenderHelper::MultipleMeshesDrawIndexInstanceCommandData& DATA, const Microsoft::WRL::ComPtr<ID3D12CommandSignature>& arg_commandSignature, const DrawFuncData::ExcuteIndirectArgumentData& arg_argmentData)
{
	DirectX12CmdList::Instance()->cmdList->IASetPrimitiveTopology(DATA.topology);
	DirectX12CmdList::Instance()->cmdList->IASetVertexBuffers(DATA.vertexBufferDrawData[0].slot, DATA.vertexBufferDrawData[0].numViews, &DATA.vertexBufferDrawData[0].vertexBufferView);
	DirectX12CmdList::Instance()->cmdList->IASetIndexBuffer(&DATA.indexBufferView[0]);

	RenderTargetStatus::Instance()->ChangeBarrier(
		arg_argmentData.m_uavArgumentBuffer.bufferWrapper->GetBuffer().Get(),
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT
	);

	DirectX12CmdList::Instance()->cmdList->ExecuteIndirect
	(
		arg_commandSignature.Get(),
		1,
		arg_argmentData.m_uavArgumentBuffer.bufferWrapper->GetBuffer().Get(),
		0,
		nullptr,
		0
	);

	RenderTargetStatus::Instance()->ChangeBarrier(
		arg_argmentData.m_uavArgumentBuffer.bufferWrapper->GetBuffer().Get(),
		D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS
	);
}

std::string DrawingByRasterize::ErrorMail(const std::source_location& DRAW_SOURCE_LOCATION)
{
	std::string lFunctionString = DRAW_SOURCE_LOCATION.function_name();
	std::string lFileNameString = DRAW_SOURCE_LOCATION.file_name();
	std::string lColumn = std::to_string(DRAW_SOURCE_LOCATION.column());
	std::string lLine = std::to_string(DRAW_SOURCE_LOCATION.line());

	return lFileNameString + "�t�@�C����" + lFunctionString + "�֐���" + lLine + "�s�ڂ�" + lColumn + "�����ڂɏ�����Ă���`��N���X�Ő������ꂽ���ɖ�肪����܂�";
}