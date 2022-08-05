#pragma once
#include <GameEngineCore/GameEngineActor.h>

// 설명 : 모든 적 개체의 상태 -> Patrol > Chase > Attack 
// Grunt ->
class GameEngineTextureRenderer;
class GameEngineCollision;
class EnemyActor : public GameEngineActor
{
public:
	// constrcuter destructer
	EnemyActor();
	virtual ~EnemyActor();

	// delete Function
	EnemyActor(const EnemyActor& _Other) = delete;
	EnemyActor(EnemyActor&& _Other) noexcept = delete;
	EnemyActor& operator=(const EnemyActor& _Other) = delete;
	EnemyActor& operator=(EnemyActor&& _Other) noexcept = delete;

// 벽 상태
protected:
	void OnEvent() override;

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
	
	bool Down;
	bool DoubleDown;
	bool Left_Up;
	bool Left_Down;
	bool Right_Up;
	bool Right_Down;
	bool Left;
	bool Right;
	void WallCheck();
	STATE_WALL WallState;

	bool IsFall;


// 상태
protected:
	enum class ENEMYACTION
	{
		PATROL,
		CHASE,
		ATTACK,
	};

	// 피격 체크, true : 콜리전 체크 1회  false : 지속
	virtual bool PlayerAttackCheck(GameEngineCollision* _This, GameEngineCollision* _Other) = 0;

	void ChooseAction();
	ENEMYACTION PrevAction;
	ENEMYACTION CurAction;

	void Move(float _DeltaTime);
protected:

	GameEngineTextureRenderer* Renderer_Character;
	GameEngineCollision* Collision_Character;
	GameEngineCollision* Collision_ChaseSensor;	

	float AttackRange;
	float4 Velocity;
	float4 MoveVec;
	float MoveSpeed;

	float4 FlyVec;
	float FlyRadian;

	int Hp;

// 애니메이션
protected:
	std::string EnemyName;
	GameEngineStateManager StateManager;
	GameEngineStateManager AnimationStateManager;

	void CreateAllFolderAnimation();
};

