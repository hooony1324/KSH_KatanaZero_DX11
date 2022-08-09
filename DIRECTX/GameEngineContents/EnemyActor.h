#pragma once
#include <GameEngineCore/GameEngineActor.h>

// 설명 : 모든 적 개체의 상태 -> Patrol > Chase > Attack 
// Grunt ->
class GameEngineTextureRenderer;
class GameEngineCollision;
class EnemyActor : public GameEngineActor
{
public:
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
	bool Damaged(GameEngineCollision* _This, GameEngineCollision* _Other);
	void PlayerAlertCheck();
	bool SeePlayer(GameEngineCollision* _This, GameEngineCollision* _Other);
	void PlayerLeftRightCheck();
	bool FindPlayer;
	float4 PlayerPos;
	float4 EnemyPos;
	int PlayerDir;


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

	virtual void DeadStart(const StateInfo& _Info);
	virtual void DeadUpdate(float _DeltaTime, const StateInfo& _Info);

	GameEngineStateManager StateManager;

	void Move(float _DeltaTime);
	void LookDirCheck();

protected:
	GameEngineTextureRenderer* Renderer_Character;
	GameEngineTextureRenderer* Renderer_Alert;
	GameEngineCollision* Collision_Character;
	GameEngineCollision* Collision_ChaseSensor;	
	
	float4 Velocity;
	float4 MoveVec;
	float MoveSpeed;
	float4 FlyVec;
	float FlyRadian;
	int PrevLookDir;


	
};

