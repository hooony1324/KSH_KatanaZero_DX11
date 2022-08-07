#pragma once
#include <GameEngineCore/GameEngineActor.h>

// 설명 : 플레이어(제로, 드래곤, ...), 또는 보스 몬스터
class GameEngineTextureRenderer;
class GameEngineCollision;
class CharacterActor : public GameEngineActor
{
	// 적은 플레이어를 한번 인식하면 끝까지 쫓아온다
public:
	static float4 CharacterPosition;

public:
	// constrcuter destructer
	CharacterActor();
	virtual ~CharacterActor();

	// delete Function
	CharacterActor(const CharacterActor& _Other) = delete;
	CharacterActor(CharacterActor&& _Other) noexcept = delete;
	CharacterActor& operator=(const CharacterActor& _Other) = delete;
	CharacterActor& operator=(CharacterActor&& _Other) noexcept = delete;

	inline bool IsRoomChangeAvailable()
	{
		return Right_UpBlue && Right_DownBlue;
	}

protected:
	enum class STATE_WALL
	{
		NONE,
		RIGHT,
		LEFT,
		UP,
		DOWN,
		UNDERGROUND,
		RIGHTSLOPE,
		LEFTSLOPE,
	};

	STATE_WALL WallState;

protected:
	void WallCheck();
	void FloatTimeCheck(float _DeltaTime);
	void LookCheck(float _InputOrVelocityDir);

// 이동 관련
protected:
	GameEngineTextureRenderer* Renderer_Character;
	GameEngineCollision* Collision_Character;
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
	bool Right_UpBlue;
	bool Right_Up;
	bool Right_Down;
	bool Right_DownBlue;
	bool Left;
	bool Right;

	bool IsJump;
	bool IsFall; // 중력 적용해서 떨어짐
	bool WallGrab;
	float FloatDeltaTime; // 총 공중에 있는 시간만큼 중력 세게 적용

// 공격 관련
protected:

};

