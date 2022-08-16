#pragma once
#include <GameEngineCore/GameEngineActor.h>

// 설명 : 플레이어(제로, 드래곤, ...), 또는 보스 몬스터
class GameEngineTextureRenderer;
class GameEngineCollision;
class CharacterActor : public GameEngineActor
{
public:
	static void CheatModeSwitch()
	{
		CheatMode = !CheatMode;
	}

	static void CheatModeOn()
	{
		CheatMode = true;
	}

	static void CheatModeOff()
	{
		CheatMode = false;
	}


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
		return (Right_Red || Left_Red ) && !IsDead;
	}

	inline bool IsPlayerDead()
	{
		return IsDead;
	}

	void Respawn()
	{
		IsDead = false;
		Hp = 1;
		PlayerStateManager.ChangeState("Idle");
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

// 상태 체크
protected:
	void WallCheck();
	void FloatTimeCheck(float _DeltaTime);
	void LookCheck(float _InputOrVelocityDir);
	
	// 피격
	void EnemyAttackCheck();
	bool Damaged(GameEngineCollision* _This, GameEngineCollision* _Other);
	bool HitBullet(GameEngineCollision* _This, GameEngineCollision* _Other);

	GameEngineStateManager PlayerStateManager;

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
	bool DownBlue;
	bool DoubleDown;
	bool DoubleDownBlue;
	bool Left_Up;
	bool Left_Down;
	bool Right_Red;
	bool Right_Up;
	bool Right_Down;
	bool Left_Red;
	bool Left;
	bool Right;

	bool IsJump;
	bool IsFall; // 중력 적용해서 떨어짐
	bool WallGrab;
	float FloatDeltaTime; // 총 공중에 있는 시간만큼 중력 세게 적용

	// 죽음
protected:
	int Hp;
	float FlyAngle;
	float4 FlyVector;
	bool IsDead;
	bool Invincible; // 구르는 동안 무적
	static bool CheatMode;
};

