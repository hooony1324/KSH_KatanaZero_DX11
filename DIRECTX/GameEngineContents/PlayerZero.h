#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineBase/GameEngineSound.h>

const float SPEED_PLAYER = 400.0f;

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
	UP,
	LEFT,
	RIGHT,
	DOWN,
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
	void CreateAllAnimation();

private:
	GameEngineTextureRenderer* Renderer_Player;
	GameEngineTextureRenderer* Renderer_Slash;

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
	void CoolTimeCheck();

private:
	STATE_PLAYER PlayerState;
	Timer* AttackTimer;
	Timer* RollTimer;

	// Animation
	bool IdleRun_AniEnd;
	bool RunIdle_AniEnd;
	bool Roll_AniEnd;
	bool Attack_AniEnd;

	// CoolTime
	bool AttackAble;
	bool RollAble;

// 플레이어 정보
private:
	void CreateSlash();
	
private:
	// 입력
	float4 MousePos;
	float4 MouseDir;
	float4 InputDir;
	float4 MoveDir;
	float PlayerSpeed;

	float MoveForce;
};

