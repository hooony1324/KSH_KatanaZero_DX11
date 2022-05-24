#include "GameEngineRenderer.h"
#include "GameEngineActor.h"
#include "GameEngineLevel.h"
#include <Windows.h>
#include <GameEngineBase/GameEngineWindow.h>

#include "GameEngineVertexBuffer.h"
#include "GameEngineIndexBuffer.h"

GameEngineRenderer::GameEngineRenderer()
{
}

GameEngineRenderer::~GameEngineRenderer()
{
}


void GameEngineRenderer::Start()
{
	GetActor()->GetLevel()->PushRenderer(this);
}

float Degree = 0.0f;
void GameEngineRenderer::Render(float _DeltaTime)
{
	GameEngineVertexBuffer* Vertex = GameEngineVertexBuffer::Find("Box");
	GameEngineIndexBuffer* Index = GameEngineIndexBuffer::Find("Box");

	std::vector<POINT> DrawVertex;
	DrawVertex.resize(Index->Indexs.size());

	std::vector<float4> CopyBuffer;
	CopyBuffer.resize(Index->Indexs.size());

	Degree += _DeltaTime * 90.0f;

	for (size_t i = 0; i < Index->Indexs.size(); i++)
	{
		int TriIndex = Index->Indexs[i];

		// 0 ��° ������ ���� �˴ϴ�.
		CopyBuffer[i] = Vertex->Vertexs[TriIndex];

		// [0.5f] [0.5f] []                  [100] [100] [] 
		// ũ
		CopyBuffer[i] *= GetActor()->GetTransform().GetScale();

		// ����
		CopyBuffer[i] = float4::VectorRotationToDegreeXAxis(CopyBuffer[i], Degree);
		CopyBuffer[i] = float4::VectorRotationToDegreeYAxis(CopyBuffer[i], Degree);
		CopyBuffer[i] = float4::VectorRotationToDegreeZAxis(CopyBuffer[i], Degree);

		// �̵�
		CopyBuffer[i] += GetActor()->GetTransform().GetPosition();



		DrawVertex[i] = CopyBuffer[i].GetConvertWindowPOINT();
	}


	for (size_t i = 0; i < DrawVertex.size(); i += 3)
	{
		Polygon(GameEngineWindow::GetHDC(), &DrawVertex[i], 3);
	}


	// Rectangle(GameEngineWindow::GetHDC(), LeftTop.ix(), LeftTop.iy(), RightBot.ix(), RightBot.iy());
}