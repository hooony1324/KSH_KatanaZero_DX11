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
	void RemoveCapturedData();
	void PlayReverseCapturedData();
	bool IsReverseEnd()
	{
		return IsReverse && CapturedDataList.size() == 0;
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

	// �����
protected:
	// �÷��̾�, �Ѿ�, ���� �������� �ִ� �ڽĿ��� �� �����ؾ���
	virtual void PushFrameCpaturedData() = 0;

	// �������� ����� �� ��, ���� �������� ���� ������� �������� Ŵ
	virtual void ReverseStartSetting() = 0;
	virtual void ReverseEndSetting() = 0;

};

