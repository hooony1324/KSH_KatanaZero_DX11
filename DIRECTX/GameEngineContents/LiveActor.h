#pragma once
#include <GameEngineCore/GameEngineActor.h>

// 설명 :
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
		RIGHT_PASS, // 다음 스테이지 포탈
		LEFT_WALL,
		DOWN_WALL,
		DOWN_PASS, // 통과 가능
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

