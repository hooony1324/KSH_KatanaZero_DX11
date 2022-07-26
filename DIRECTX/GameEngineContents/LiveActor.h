#pragma once
#include <GameEngineCore/GameEngineActor.h>

// ���� :
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

protected:
	enum class STATE
	{
		UP_WALL,
		RIGHT_WALL,
		RIGHT_PASS, // ���� �������� ��Ż
		LEFT_WALL,
		DOWN_WALL,
		DOWN_PASS, // ��� ����
	};

	STATE WallState;

protected:
	void PixelCheck();

protected:
	GameEngineTextureRenderer* Renderer_Character;
	float4 MoveDir;
	float MoveSpeed;

private:
	float4 Pixel_Down;
};

