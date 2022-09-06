#pragma once
#include <GameEngineCore/GameEngineActor.h>

// ���� : �÷��̾�, ��, ����, �Ѿ�, ...
// '����'�� ��ġ, ����
class FrameCapturedData
{
public:
	FrameCapturedData() : Texture(nullptr) {};
	~FrameCapturedData() {};

	float4 Position;
	GameEngineTexture* Texture;
	float4 TextureScale;
	
};

class LiveActor : public GameEngineActor
{
public:
	// constrcuter destructer
	LiveActor();
	virtual ~LiveActor();

	// delete Function
	LiveActor(const LiveActor& _Other) = delete;
	LiveActor(LiveActor&& _Other) noexcept = delete;
	LiveActor& operator=(const LiveActor& _Other) = delete;
	LiveActor& operator=(LiveActor&& _Other) noexcept = delete;

	std::list<FrameCapturedData*> CapturedDataList;
	GameEngineTextureRenderer* FrameDataRenderer;
	bool IsReverse;
};

