#pragma once
#include "CharacterActor.h"
#include <GameEngineBase/GameEngineSound.h>

const float SPEED_PLAYER = 500.0f;

// ���� :
class Timer;
class GameEngineTextureRenderer;
class GameEngineCollision;
class PlayerZero : public CharacterActor
{
public:
	//bool IsPlayerAttack(); // ī�޶� ��鸲 + ��� ������

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

// �ʱ�ȭ ����
private:
	void CreateAllAnimation();

private:


// �÷��̾� ����
private:
	void PlayerMove(float _DeltaTime);
	void CreateSlash();
	void WallGrabCheck();
	void PrintPlayerDebug();
	
private:
	// �Է�
	float4 MousePos;
	float4 MouseDir;


	// ����
	float JumpInput;
	bool IsFlip;
	bool IsAttack;

	// FSM
private:
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

	void DeadStart(const StateInfo& _Info);
	void DeadUpdate(float _DeltaTime, const StateInfo& _Info);

	void DoorBreakStart(const StateInfo& _Info);
	void DoorBreakUpdate(float _DeltaTime, const StateInfo& _Info);

	void InputCheck();
	void CoolTimeCheck();

private:
	
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


// ���ε�
private:
	void StateManagerInit();

};

