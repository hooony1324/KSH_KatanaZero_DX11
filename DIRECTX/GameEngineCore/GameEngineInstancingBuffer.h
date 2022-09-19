#pragma once
#include "GameEngineRes.h"
#include <vector>
#include <string>
#include <map>
#include <list>
#include "GameEngineVertexs.h"

// Ό³Έν :
class GameEngineInstancingBuffer
{
public:
	// constrcuter destructer
	GameEngineInstancingBuffer();
	~GameEngineInstancingBuffer();

	// delete Function
	GameEngineInstancingBuffer(const GameEngineInstancingBuffer& _Other) = delete;
	GameEngineInstancingBuffer(GameEngineInstancingBuffer&& _Other) noexcept = delete;
	GameEngineInstancingBuffer& operator=(const GameEngineInstancingBuffer& _Other) = delete;
	GameEngineInstancingBuffer& operator=(GameEngineInstancingBuffer&& _Other) noexcept = delete;

protected:

private:
	D3D11_BUFFER_DESC BufferDesc;

	ID3D11Buffer* Buffer;


};

