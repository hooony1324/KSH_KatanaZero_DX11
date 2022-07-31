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
		NONE,
		UP_WALL,
		RIGHT_WALL,
		RIGHT_PASS, // 다음 스테이지 포탈
		LEFT_WALL,
		DOWN_WALL, // 지면(중력X)
		UNDER_GROUND, // 땅에 박힘
		UNDER_RIGHTUP_SLOPE,
		UNDER_LEFTUP_SLOPE,

		RIGHTUP_SLOPE,
		RIGHTDOWN_SLOPE,
		LEFTUP_SLOPE,
		LEFTDOWN_SLOPE,

	};

	STATE WallState;
	bool IsFly; // 공중

protected:
	void PixelCheck();
	void WallCheck();
	void FloatCheck(float _DeltaTime);
	void LookCheck(float _InputOrVelocityDir);

protected:
	GameEngineTextureRenderer* Renderer_Character;
	float InitSpeed;
	float MoveSpeed; // 속력
	float4 MoveVec;	// 방향

	float4 Velocity; // 속도
	float4 GrabityForce; // 중력 방향

	int CurLookDir;	// 시선
	int PrevLookDir;

	bool Up;
	bool MiddleDown;
	bool Down;
	bool DoubleDown;
	bool DoubleDownBlue;
	bool Left_Up;
	bool Left_Down;
	bool Right_Up;
	bool Right_Down;
	bool Left;
	bool Right;

	bool IsJump;
	bool IsFall; // 중력 적용해서 떨어짐
	float FloatDeltaTime; // 총 공중에 있는 시간만큼 중력 세게 적용

};

