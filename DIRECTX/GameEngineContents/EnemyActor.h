#pragma once
#include "LiveActor.h"

// 설명 : 모든 적 개체의 상태 -> Patrol > Chase > Attack 
// Grunt ->
class GameEngineTextureRenderer;
class GameEngineCollision;
class EnemyActor : public LiveActor
{
public:
	bool IsClicked; // 디버깅용

	bool IsDead()
	{
		return Hp <= 0;
	}

	void Respawn()
	{
		GetTransform().SetWorldPosition(SpawnPos);
		StateManager.ChangeState("Spawn");
	}

	void SetSpawnPos(float4 _Pos)
	{
		SpawnPos = _Pos;
	}

protected:
	float4 SpawnPos;

public:
	// constrcuter destructer
	EnemyActor();
	virtual ~EnemyActor();

	// delete Function
	EnemyActor(const EnemyActor& _Other) = delete;
	EnemyActor(EnemyActor&& _Other) noexcept = delete;
	EnemyActor& operator=(const EnemyActor& _Other) = delete;
	EnemyActor& operator=(EnemyActor&& _Other) noexcept = delete;

	void EnemyActorDebug();

// 초기화
protected:
	std::string EnemyName;
	void CreateRendererAndCollision();
	void CreateAllFolderAnimation();
	void CreateAllState();

	int Hp;
	float AttackRange;
	bool AttackAniEnd;
	bool IsChasingEnemy;

// 벽 상태
protected:
	void OnEvent() override;
	void WallCheck();
	
	GameEngineTextureRenderer* CurCollisionMap;

	enum class STATE_WALL
	{
		NONE,
		RIGHT,
		LEFT,
		UP,
		DOWN,
		DOWNBLUE,
		UNDERGROUND,
		RIGHTSLOPE,
		LEFTSLOPE,
	};
	STATE_WALL WallState;
	
	bool Down;
	bool DownBlue;
	bool DoubleDown;
	bool DoubleDownBlue;
	bool Left_Up;
	bool Left_Down;
	bool Right_Up;
	bool Right_Down;
	bool Left;
	bool Right;

	bool IsFall;

// FSM
protected:
	// 순찰, 추격
	void PlayerAttackCheck();
	void PlayerAlertCheck();
	void PlayerSameFloorCheck();

	CollisionReturn Damaged(GameEngineCollision* _This, GameEngineCollision* _Other);
	CollisionReturn DoorCheck(GameEngineCollision* _This, GameEngineCollision* _Other);
	CollisionReturn SeePlayer(GameEngineCollision* _This, GameEngineCollision* _Other);

	GameEngineCollision* PlayerCollision;
	class CharacterActor* ChasingPlayer;
	std::vector<Stair*> StairsToPlayer;
	Stair* CurDestEndStair;
	Stair* CurDestStair;
	bool IsDoorFront;	// 순찰 중 문이 플레이어 앞에 있음
	bool IsDoor;		// 순찰 중 문이 앞에 있음
	bool FindPlayer;
	float4 PlayerPos;
	float4 EnemyPos;
	float4 PlayerDir;
	float ChaseSensorPaddingX;
	bool PlayerSameFloor;

	// PATROL : WALK / IDLE / TURN
	// CHASE : RUN / TURN / ATTACK
	virtual void SpawnUpdate(float _DeltaTime, const StateInfo& _Info);

	virtual void IdleStart(const StateInfo& _Info);
	virtual void IdleUpdate(float _DeltaTime, const StateInfo& _Info);

	virtual void WalkStart(const StateInfo& _Info);
	virtual void WalkUpdate(float _DeltaTime, const StateInfo& _Info);

	virtual void PatrolTurnStart(const StateInfo& _Info);
	virtual void PatrolTurnUpdate(float _DeltaTime, const StateInfo& _Info);

	// PATROL -> CHASE
	virtual void AlertStart(const StateInfo& _Info);
	virtual void AlertUpdate(float _DeltaTime, const StateInfo& _Info);

	virtual void RunStart(const StateInfo& _Info);
	virtual void RunUpdate(float _DeltaTime, const StateInfo& _Info);

	virtual void ChaseTurnStart(const StateInfo& _Info);
	virtual void ChaseTurnUpdate(float _DeltaTime, const StateInfo& _Info);

	// 오버라이드 해야 될 듯(원거리 / 근거리, 이펙트 등등)
	virtual void AttackStart(const StateInfo& _Info) = 0;
	virtual void AttackUpdate(float _DeltaTime, const StateInfo& _Info) = 0;

	// 죽음
	virtual void HurtflyStart(const StateInfo& _Info);
	virtual void HurtflyUpdate(float _DeltaTime, const StateInfo& _Info);

	virtual void HurtgroundStart(const StateInfo& _Info);
	virtual void HurtgroundUpdate(float _DeltaTime, const StateInfo& _Info);

	// 위층 아래측 추격
	virtual void GoUpstairStart(const StateInfo& _Info);
	virtual void GoUpstairUpdate(float _DeltaTime, const StateInfo& _Info);

	virtual void GoDownstairStart(const StateInfo& _Info);
	virtual void GoDownstairUpdate(float _DeltaTime, const StateInfo& _Info);

	virtual void SlopeRunStart(const StateInfo& _Info);
	virtual void SlopeRunUpdate(float _DeltaTime, const StateInfo& _Info);


	GameEngineStateManager StateManager;

	void Move(float _DeltaTime);
	void LookDirCheck();

protected:
	GameEngineTextureRenderer* Renderer_Character;
	GameEngineTextureRenderer* Renderer_Alert;
	GameEngineTextureRenderer* Renderer_GunArm;
	GameEngineCollision* Collision_Character;
	GameEngineCollision* Collision_ChaseSensor;	
	GameEngineCollision* Collision_Aim;


	float4 Velocity;
	float4 MoveVec;
	float MoveSpeed;
	float4 FlyVec;
	float FlyPower;
	float FlyRadian;
	int PrevLookDir;
	float AimPaddingX;

	// 역재생
public:
	virtual void PushFrameCpaturedData() override;
	// 렌더러가 역재생 될 때, 기존 렌더러는 끄고 역재생용 렌더러는 킴
	virtual void ReverseStartSetting() override;
	virtual void ReverseEndSetting() override;

private:
	// Pixel 위치
	Pix DownPix = { 0, -1 };
	Pix DoubleDownPix = { 0, -2 };
	Pix DownBluePix = { 0, -1 };
	Pix DoubleDownBluePix = { 0, -2 };
	Pix LeftPix = { -20, 3 };
	Pix RightPix = { 20, 3 };
	Pix Right_UpPix = { 20, 70 };
	Pix Left_UpPix = { -20, 70 };
	Pix Right_DownPix = { 15, -2 };
	Pix Left_DownPix = { -15, -2 };
};

