#pragma once
#include "LiveActor.h"
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
class PlayerZero : public LiveActor
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
	GameEngineTextureRenderer* Renderer_Slash;


// 플레이어 정보
private:
	void PlayerMove(float _DeltaTime);
	void PrintPlayerDebug();
	void CreateSlash();
	void WallGrabCheck();
	
private:
	// 입력
	float4 MousePos;
	float4 MouseDir;
	float4 InputDir;

	// 상태
	float JumpInput;
	bool IsFlip;
	bool IsAttack;

	// FSM
public:
	void AttackStart(const StateInfo& _Info);
	void AttackUpdate(float _DeltaTime, const StateInfo& _Info);

	void FallStart(const StateInfo& _Info);
	void FallUpdate(float _DeltaTime, const StateInfo& _Info);

	void IdleStart(const StateInfo& _Info);
	void IdleUpdate(float _DeltaTime, const StateInfo& _Info);

	void JumpStart(const StateInfo& _Info);
	void JumpUpdate(float _DeltaTime, const StateInfo& _Info);

	void RollStart(const StateInfo& _Info);
	void RollUpdate(float _DeltaTime, const StateInfo& _Info);

	void RunStart(const StateInfo& _Info);
	void RunUpdate(float _DeltaTime, const StateInfo& _Info);

	void WallGrabStart(const StateInfo& _Info);
	void WallGrabUpdate(float _DeltaTime, const StateInfo& _Info);

	void WallSlideStart(const StateInfo& _Info);
	void WallSlideUpdate(float _DeltaTime, const StateInfo& _Info);

	void FlipStart(const StateInfo& _Info);
	void FlipUpdate(float _DeltaTime, const StateInfo& _Info);

	void RunToIdleStart(const StateInfo& _Info);
	void RunToIdleUpdate(float _DeltaTime, const StateInfo& _Info);

	void IdleToRunStart(const StateInfo& _Info);
	void IdleToRunUpdate(float _DeltaTime, const StateInfo& _Info);

	void InputCheck();
	void CoolTimeCheck();

private:
	GameEngineStateManager PlayerStateManager;
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

// Animation 바인딩
private:

	inline void IdleRunStart(const FrameAnimation_DESC& _Info)
	{
		IdleRun_AniEnd = false;
	}

	inline void RunidleStart(const FrameAnimation_DESC& _Info)
	{
		RunIdle_AniEnd = false;
	}

	inline void RollStart(const FrameAnimation_DESC& _Info)
	{
		Roll_AniEnd = false;
	}

	inline void AttackStart(const FrameAnimation_DESC& _Info)
	{
		Attack_AniEnd = false;
	}

	inline void IdleRunEnd(const FrameAnimation_DESC& _Info)
	{
		IdleRun_AniEnd = true;
	}

	inline void RunidleEnd(const FrameAnimation_DESC& _Info)
	{
		RunIdle_AniEnd = true;
	}

	inline void RollEnd(const FrameAnimation_DESC& _Info)
	{
		Roll_AniEnd = true;
	}

	inline void AttackEnd(const FrameAnimation_DESC& _Info)
	{
		Attack_AniEnd = true;
	}

	void StopIdleToRun(const FrameAnimation_DESC& _Info)
	{
		if (InputDir.CompareInt2D({ 0, 0 }))
		{
			MoveVec = float4::ZERO;
			PlayerStateManager.ChangeState("Idle");
		}
	}

};

