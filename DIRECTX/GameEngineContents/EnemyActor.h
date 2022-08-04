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


// 액션 상태
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

protected:
	GameEngineTextureRenderer* Renderer_Character;
	GameEngineCollision* Collision_Character;
	GameEngineCollision* Collision_ChaseSensor;			// 플레이어 근처 식별

	float AttackRange;									// Chase -> Attack 할 수 있는 거리
	float4 Velocity;
	float4 MoveVec;
	float MoveSpeed;

	int Hp;

// Animation
protected:
	std::string EnemyName;
	GameEngineStateManager AnimationStateManager;

	void CreateAllFolderAnimation();
};

