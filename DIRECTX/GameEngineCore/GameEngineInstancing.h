#pragma once
#include "GameEngineShaderResourcesHelper.h"
#include "GameEngineInstancingBuffer.h"

// ���� :
class GameEngineInstancing
{
public:
	static unsigned int MinInstancingCount;
	static unsigned int StartInstancingCount;

public:
	// constrcuter destructer
	GameEngineInstancing();
	~GameEngineInstancing();

	// delete Function
	GameEngineInstancing(const GameEngineInstancing& _Other) = delete;
	GameEngineInstancing(GameEngineInstancing&& _Other) noexcept = delete;
	GameEngineInstancing& operator=(const GameEngineInstancing& _Other) = delete;
	GameEngineInstancing& operator=(GameEngineInstancing&& _Other) noexcept = delete;


public:
	//RenderingInstancing()
	//	: Buffer(nullptr)
	//	, Count(0)
	//	, DataInsert(0)
	//{
	//}


public:
	int DataInsert;
	std::vector<char> DataBuffer;
	GameEngineShaderResourcesHelper ShaderResources;
	GameEngineRenderingPipeLine* InstancingPipeLine;
	int Size;
	unsigned int Count;
	int MaxDataCount;
	GameEngineInstancingBuffer* Buffer;

protected:

private:

};

