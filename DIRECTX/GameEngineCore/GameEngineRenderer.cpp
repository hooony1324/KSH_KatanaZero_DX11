#include "PreCompile.h"
#include "GameEngineRenderer.h"
#include "GameEngineActor.h"
#include "GameEngineLevel.h"
#include <Windows.h>
#include <GameEngineBase/GameEngineWindow.h>
#include "GameEngineCamera.h"
#include "GameEngineInstancing.h"


#include "GameEngineRenderingPipeLine.h"
#include "GameEngineVertexBuffer.h"
#include "GameEngineIndexBuffer.h"


GameEngineRenderer::GameEngineRenderer()
	: CameraOrder(CAMERAORDER::MAINCAMERA)
	, RenderingOrder(0)
{
}

GameEngineRenderer::~GameEngineRenderer() 
{
	int a = 0;
}

void GameEngineRenderer::Start() 
{
}

void GameEngineRenderer::EngineShaderResourcesSetting(GameEngineShaderResourcesHelper* _ShaderResources)
{
	//// 랜더러 쪽으로 빠져야 한다.
	if (true == _ShaderResources->IsConstantBuffer("TRANSFORMDATA"))
	{
		_ShaderResources->SetConstantBufferLink("TRANSFORMDATA", &GetTransformData(), sizeof(GetTransformData()));
	}

	if (true == _ShaderResources->IsConstantBuffer("RENDEROPTION"))
	{
		_ShaderResources->SetConstantBufferLink("RENDEROPTION", &renderOption, sizeof(renderOption));
	}
}

void GameEngineRenderer::PushRendererToMainCamera()
{
	GetActor()->GetLevel()->PushRendererToMainCamera(this);	
}

void GameEngineRenderer::SetRenderingOrder(int _Order)
{
	Camera->ChangeRenderingOrder(this, _Order);
}

void GameEngineRenderer::PushRendererToUICamera()
{
	GetActor()->GetLevel()->PushRendererToUICamera(this);
}

GameEngineRenderingPipeLine* GameEngineRenderer::ClonePipeLine(GameEngineRenderingPipeLine* _Rendering)
{
	// 이름없는 녀석으로 만든다.
	GameEngineRenderingPipeLine* Clone = GameEngineRenderingPipeLine::Create();
	Clone->Copy(_Rendering);
	return Clone;
}

bool GameEngineRenderer::IsInstancing(GameEngineRenderingPipeLine* _Rendering)
{
	std::unordered_map<GameEngineRenderingPipeLine*, GameEngineInstancing>::iterator InstancingIter = Camera->InstancingMap.find(_Rendering);

	if (InstancingIter == Camera->InstancingMap.end())
	{
		return false;
	}

	return true == IsInstancing_ && GameEngineInstancing::MinInstancingCount <= InstancingIter->second.Count;
}

// 우리 엔진에서 인스턴싱을 한다면 무조건 숫자하나만 인스턴싱을 했으니까. 이건 ok
void GameEngineRenderer::InstancingDataSetting(GameEngineRenderingPipeLine* _Line)
{
	// 몇번째 순서인지 알려주고 있어요
	// 이녀석을 통해서 
	int InstancingIndex = Camera->PushInstancingIndex(_Line);

	GameEngineInstancing* Instancing = Camera->GetInstancing(_Line);

	if (nullptr == Instancing)
	{
		MsgBoxAssert("인스턴싱이 켜져있지만 인스턴싱 정보는 없습니다.");
	}

	if (true == Instancing->ShaderResources.IsStructuredBuffer("AllInstancingTransformData"))
	{
		GameEngineStructuredBufferSetter* Setter = Instancing->ShaderResources.GetStructuredBuffer("AllInstancingTransformData");

		Setter->Push(GetTransform().GetTransformData(), InstancingIndex);

		

		//Setter->Res

		// GetTransform().GetTransformData()
	}
}


//void GameEngineRenderer::Render(float _DeltaTime)
//{
//	//// 랜더링
//	////GameEngineVertexBuffer* Vertex = GameEngineVertexBuffer::Find("Rect");
//	////GameEngineIndexBuffer* Index = GameEngineIndexBuffer::Find("Rect");
//
//	//GameEngineVertexBuffer* Vertex = GameEngineVertexBuffer::Find("Box");
//	//GameEngineIndexBuffer* Index = GameEngineIndexBuffer::Find("Box");
//	//
//	//std::vector<POINT> DrawVertex;
//	//DrawVertex.resize(Index->Indexs.size());
//
//	//std::vector<float4> CopyBuffer;
//	//CopyBuffer.resize(Index->Indexs.size());
//
//
//	//for (size_t i = 0; i < Index->Indexs.size(); i++)
//	//{
//	//	int TriIndex = Index->Indexs[i];
//
//	//	// 0 번째 순서의 점이 됩니다.
//	//	// 최초에 원본 매쉬의 점을 복사합니다.
//	//	CopyBuffer[i] = Vertex->Vertexs[TriIndex];
//
//	//	auto& tran = GetTransform();
//
//	//	// 버텍스쉐이더
//	//	CopyBuffer[i] = CopyBuffer[i] * GetTransform().GetWorldViewProjection();
//
//
//	//	// 레스터라이저
//	//	//// 기록해놨던 z값으로 나뉘는것
//	//	CopyBuffer[i] = CopyBuffer[i] / CopyBuffer[i].w;
//
//
//	//	DrawVertex[i] = CopyBuffer[i].GetConvertWindowPOINT();
//	//}
//	//
//	//for (size_t i = 0; i < DrawVertex.size(); i += 3)
//	//{
//	//	Polygon(GameEngineWindow::GetHDC(), &DrawVertex[i], 3);
//	//}
//}

void GameEngineRenderer::ChangeCamera(CAMERAORDER _Order)
{
	GetActor()->GetLevel()->PushRenderer(this, _Order);
}


