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
		DOWN_GROUND, // 땅에 박힘

		RIGHTUP_SLOPE,
		RIGHTDOWN_SLOPE,
		LEFTUP_SLOPE,
		LEFTDOWN_SLOPE,

	};

	STATE WallState;
	bool IsFly; // 공중

protected:
	void PixelCheck();
	void VelocityCheck(float _DeltaTime);
	void LookCheck();

protected:
	GameEngineTextureRenderer* Renderer_Character;
	float InitSpeed;
	float MoveSpeed; // 속력
	float4 MoveDir;	// 방향
	float4 Velocity; // 속도
	int CurLookDir;	// 시선
	int PrevLookDir;

	bool Up;
	bool Down;
	bool DownBlue;
	bool Left_Up;
	bool Left_Down;
	bool Right_Up;
	bool Right_Down;

	bool Left;
	bool Right;
	

private:


};

