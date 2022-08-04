#pragma once
#include <GameEngineCore/GameEngineActor.h>

// ���� : ��� �� ��ü�� ���� -> Patrol > Chase > Attack 
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

// �� ����
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


// �׼� ����
protected:
	enum class ENEMYACTION
	{
		PATROL,
		CHASE,
		ATTACK,
	};

	// �ǰ� üũ, true : �ݸ��� üũ 1ȸ  false : ����
	virtual bool PlayerAttackCheck(GameEngineCollision* _This, GameEngineCollision* _Other) = 0;

	void ChooseAction();
	ENEMYACTION PrevAction;
	ENEMYACTION CurAction;

protected:
	GameEngineTextureRenderer* Renderer_Character;
	GameEngineCollision* Collision_Character;
	GameEngineCollision* Collision_ChaseSensor;			// �÷��̾� ��ó �ĺ�

	float AttackRange;									// Chase -> Attack �� �� �ִ� �Ÿ�
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

