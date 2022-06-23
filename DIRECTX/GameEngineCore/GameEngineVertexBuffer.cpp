#include "PreCompile.h"
#include "GameEngineVertexBuffer.h"
#include <GameEngineBase/GameEngineString.h>

GameEngineVertexBuffer::GameEngineVertexBuffer()
{
}

GameEngineVertexBuffer::~GameEngineVertexBuffer()
{
}

GameEngineVertexBuffer* GameEngineVertexBuffer::Create(const std::string& _Name, const std::vector<float4>& _Vertex)
{
	GameEngineVertexBuffer* NewRes = CreateResName(_Name);
	NewRes->Vertexs = _Vertex;
	return NewRes;
}

GameEngineVertexBuffer* GameEngineVertexBuffer::Create(const std::vector<float4>& _Vertex)
{
	GameEngineVertexBuffer* NewRes = CreateResUnName();
	NewRes->Vertexs = _Vertex;
	return NewRes;
}