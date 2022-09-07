#pragma once
#include "GameEngineRes.h"

// Ό³Έν :
class GameEngineStructuredBuffer : public GameEngineRes<GameEngineStructuredBuffer>
{
public:
	// constrcuter destructer
	GameEngineStructuredBuffer();
	~GameEngineStructuredBuffer();

	// delete Function
	GameEngineStructuredBuffer(const GameEngineStructuredBuffer& _Other) = delete;
	GameEngineStructuredBuffer(GameEngineStructuredBuffer&& _Other) noexcept = delete;
	GameEngineStructuredBuffer& operator=(const GameEngineStructuredBuffer& _Other) = delete;
	GameEngineStructuredBuffer& operator=(GameEngineStructuredBuffer&& _Other) noexcept = delete;

	void Create(size_t _DataSize, size_t _DataCount, void* _StartData);
	
	void ChangeData(const void* _Data, size_t _Size) const;

protected:

private:
	ID3D11Buffer* Buffer;
	D3D11_BUFFER_DESC BufferDesc;
	D3D11_SHADER_BUFFER_DESC ShaderDesc;
	D3D11_MAPPED_SUBRESOURCE SettingResources;
	ID3D11ShaderResourceView* SRV;
	UINT DataSize;
	UINT DataCount;

	void Release();

};

