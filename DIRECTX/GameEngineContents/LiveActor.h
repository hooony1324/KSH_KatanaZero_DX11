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
		NOWALL, // 공중
		UP_WALL,
		RIGHT_WALL,
		RIGHT_PASS, // 다음 스테이지 포탈
		LEFT_WALL,
		DOWN_WALL, // 지면(중력X)
		DOWN_GROUND, // 땅에 박힘
		DOWN_PASS, // 통과 가능

		RIGHTUP_SLOPE,
		RIGHTDOWN_SLOPE,
		LEFTUP_SLOPE,
		LEFTDOWN_SLOPE,

	};

	STATE WallState;

protected:
	void PixelCheck();
	void VelocityCheck(float _DeltaTime);

protected:
	GameEngineTextureRenderer* Renderer_Character;
	float InitSpeed;
	float MoveSpeed; // 속력
	float4 MoveDir;	// 방향
	float4 Velocity; // 속도
	float SumDeltaTime;

	float4 Down;
	float4 DoubleDown;
	float4 Up;
	float4 Left;
	float4 Right;
	float4 UpRight;
	float4 DownRight;
	float4 UpLeft;
	float4 DownLeft;
private:


};

