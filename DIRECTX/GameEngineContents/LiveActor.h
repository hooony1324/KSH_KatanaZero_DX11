#pragma once
#include <GameEngineCore/GameEngineActor.h>

// 설명 : 플레이어, 적, 몬스터, 총알, ...
// '액터'의 위치, 상태
class FrameCapturedData
{
public:
	FrameCapturedData() : Texture(nullptr), TextureScale(float4::ZERO) {};
	~FrameCapturedData() {};

	float4 Position;
	GameEngineTexture* Texture;
	float4 TextureScale;
	
};

class LiveActor : public GameEngineActor
{
public:
	int GetCaptureSize()
	{
		return static_cast<int>(CapturedDataList.size());
	}

public:
	// constrcuter destructer
	LiveActor();
	virtual ~LiveActor();

	// delete Function
	LiveActor(const LiveActor& _Other) = delete;
	LiveActor(LiveActor&& _Other) noexcept = delete;
	LiveActor& operator=(const LiveActor& _Other) = delete;
	LiveActor& operator=(LiveActor&& _Other) noexcept = delete;

protected:
	std::list<FrameCapturedData*> CapturedDataList;
	GameEngineTextureRenderer* FrameDataRenderer;
	bool IsReverse;

	// 역재생
public:
	// 플레이어, 총알, 등의 렌더러가 있는 자식에서 꼭 구현해야함
	virtual void PushFrameCpaturedData() = 0;

	// 렌더러가 역재생 될 때, 기존 렌더러는 끄고 역재생용 렌더러는 킴
	virtual void ReverseStartSetting() = 0;
	virtual void ReverseEndSetting() = 0;


	void PlayReverseCapturedData();
	void RemoveCapturedData();
	bool IsReverseEnd()
	{
		return IsReverse && CapturedDataList.size() == 0;
	}
};

