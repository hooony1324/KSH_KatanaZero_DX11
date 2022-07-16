#pragma once
#include <GameEngineCore/GameEngineActor.h>

enum class STATE_PLAYER
{
	NONE,
	ATTACK,
	FALL,
	IDLE,
	JUMP,
	ROLL,
	RUN,
	WALLSLIDE,
	CROUCH,
	RUNTOIDLE,
	IDLETORUN,
};

enum class PLAYERINPUT
{
	NONE,
	UP,
	UPRIGHT,
	RIGHT,
	RIGHTDOWN,
	DOWN,
	LEFTDOWN,
	LEFT,
	UPLEFT,
	CLICK,
};

// 설명 :
class GameEngineTextureRenderer;
class Timer;
class PlayerZero : public GameEngineActor
{
public:
	// constrcuter destructer
	PlayerZero();
	~PlayerZero();

	// delete Function
	PlayerZero(const PlayerZero& _Other) = delete;
	PlayerZero(PlayerZero&& _Other) noexcept = delete;
	PlayerZero& operator=(const PlayerZero& _Other) = delete;
	PlayerZero& operator=(PlayerZero&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

// 초기화 관련
private:
	void CreateKey();
	void CreateAllAnimation();

private:
	GameEngineTextureRenderer* Renderer;

// FSM
public:
	void UpdateState();
	void ChangeState(STATE_PLAYER _PlayerState);

	void AttackStart();
	void AttackUpdate();
	
	void FallStart();
	void FallUpdate();
	
	void IdleStart();
	void IdleUpdate();
	
	void JumpStart();
	void JumpUpdate();
	
	void RollStart();
	void RollUpdate();
	
	void RunStart();
	void RunUpdate();

	void WallSlideStart();
	void WallSlideUpdate();

	void CrouchStart();
	void CrouchUpdate();
	
	void RunToIdleStart();
	void RunToIdleUpdate();

	void IdleToRunStart();
	void IdleToRunUpdate();

	void InputCheck();
private:
	STATE_PLAYER PlayerState;
	Timer* AttackTimer;

	// Animation
	bool IdleRunEnd;
	bool RunIdleEnd;
	bool RollEnd;

// 플레이어 정보
private:
	PLAYERINPUT Input;
	float4 MoveDir;
};

