#pragma once
#include "CharacterActor.h"
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineBase/GameEngineSound.h>

const float SPEED_PLAYER = 400.0f;

// 설명 :
class Timer;
class GameEngineTextureRenderer;
class GameEngineCollision;
class PlayerZero : public CharacterActor
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
	GameEngineCollision* Collision_Slash;

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

	// 죽음

	void InputCheck();
	void CoolTimeCheck();

private:
	GameEngineStateManager PlayerStateManager;
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

	inline void IdleRunAniStart(const FrameAnimation_DESC& _Info)
	{
		IdleRun_AniEnd = false;
	}

	inline void RunidleAniStart(const FrameAnimation_DESC& _Info)
	{
		RunIdle_AniEnd = false;
	}

	inline void RollAniStart(const FrameAnimation_DESC& _Info)
	{
		Roll_AniEnd = false;
	}

	inline void AttackAniStart(const FrameAnimation_DESC& _Info)
	{
		Collision_Slash->On();
		Attack_AniEnd = false;
	}

	inline void IdleRunAniEnd(const FrameAnimation_DESC& _Info)
	{
		IdleRun_AniEnd = true;
	}

	inline void RunidleAniEnd(const FrameAnimation_DESC& _Info)
	{
		RunIdle_AniEnd = true;
	}

	inline void RollAniEnd(const FrameAnimation_DESC& _Info)
	{
		Roll_AniEnd = true;
	}

	inline void AttackAniEnd(const FrameAnimation_DESC& _Info)
	{
		Collision_Slash->Off();
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

	void SlashAniUpdate(const FrameAnimation_DESC& _Info)
	{
		return;
		//Collision_Slash->On();
	}

};

