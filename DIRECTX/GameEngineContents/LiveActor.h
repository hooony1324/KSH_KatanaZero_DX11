#pragma once
#include <GameEngineCore/GameEngineActor.h>

// 설명 : 플레이어, 적, 몬스터, 총알, ...
// '액터'의 위치, 상태
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

