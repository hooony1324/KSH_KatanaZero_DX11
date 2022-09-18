#pragma once
#include <GameEngineCore/GameEngineActor.h>

// ���� : �÷��̾�, ��, ����, �Ѿ�, ...
// '����'�� ��ġ, ����

struct Pix
{
	int x;
	int y;
};

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
	static void SwitchWallStateDebug()
	{
		WallStateDebugOn = !WallStateDebugOn;
	}

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
	// �ȼ�üũ �����
	std::vector<Pix> Pixels; 
	GameEngineFontRenderer* FRenderer_WallState;
	static bool WallStateDebugOn;

	std::list<FrameCapturedData*> CapturedDataList;
	GameEngineTextureRenderer* FrameDataRenderer;
	bool IsReverse;

	// �����
public:
	// �÷��̾�, �Ѿ�, ���� �������� �ִ� �ڽĿ��� �� �����ؾ���
	virtual void PushFrameCpaturedData() = 0;

	// �������� ����� �� ��, ���� �������� ���� ������� �������� Ŵ
	virtual void ReverseStartSetting() = 0;
	virtual void ReverseEndSetting() = 0;


	void PlayReverseCapturedData();
	void RemoveCapturedData();
	bool IsReverseEnd()
	{
		return IsReverse && CapturedDataList.size() == 0;
	}
};

