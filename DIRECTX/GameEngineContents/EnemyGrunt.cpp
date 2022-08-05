#include "PreCompile.h"
#include "EnemyGrunt.h"

EnemyGrunt::EnemyGrunt() 
{
	Hp = 1;
}

EnemyGrunt::~EnemyGrunt() 
{
}

void EnemyGrunt::Start()
{
	EnemyName = "grunt";

	Renderer_Character = CreateComponent<GameEngineTextureRenderer>();
	//Renderer_Character->SetTexture("spr_grunt_idle_0.png");
	//Renderer_Character->ScaleToTexture();
	Renderer_Character->SetScaleModeImage();
	CreateAllFolderAnimation();

	Collision_Character = CreateComponent<GameEngineCollision>();
	float4 RendererScale = Renderer_Character->GetTransform().GetLocalScale();
	Collision_Character->GetTransform().SetLocalScale({RendererScale.x, RendererScale.y, GetDepth(ACTOR_DEPTH::COLLISION)});
	Collision_Character->ChangeOrder(COLLISIONGROUP::ENEMY);

	Collision_ChaseSensor = CreateComponent<GameEngineCollision>();
	Collision_ChaseSensor->GetTransform().SetLocalScale({ 400, 100, GetDepth(ACTOR_DEPTH::COLLISION)});

	StateManager.CreateStateMember("Idle"
		, std::bind(&EnemyGrunt::IdleUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&EnemyGrunt::IdleStart, this, std::placeholders::_1));
	StateManager.CreateStateMember("Dead"
		, std::bind(&EnemyGrunt::DeathUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&EnemyGrunt::DeathStart, this, std::placeholders::_1));
	


	// 기본 정보
	MoveSpeed = 300.0f;
	StateManager.ChangeState("Idle");
	GetTransform().SetLocalScale({ 2, 2, 1 });
}

void EnemyGrunt::Update(float _DeltaTime)
{
	// 피격 체크
	Collision_Character->IsCollision(CollisionType::CT_OBB2D, COLLISIONGROUP::PLAYER_ATTACK, CollisionType::CT_OBB2D,
		std::bind(&EnemyGrunt::PlayerAttackCheck, this, std::placeholders::_1, std::placeholders::_2)
	);

	// 벽
	WallCheck();

	// 액션 체크
	ChooseAction();

	StateManager.Update(_DeltaTime);

	// 움직임 상태
	Move(_DeltaTime);

}

void EnemyGrunt::End()
{
}

bool EnemyGrunt::PlayerAttackCheck(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	if (nullptr == Collision_Character)
	{
		MsgBoxAssert("this의 콜리전이 nullptr입니다");
	}

	Hp--;
	if (Hp <= 0)
	{
		StateManager.ChangeState("Dead");
	}

	// 플레이어의 공격 위치를 받아서 반대로 날려짐
	float4 val = _Other->GetTransform().GetWorldPosition();

	return true;
}

void EnemyGrunt::Move(float _DeltaTime)
{
	switch (WallState)
	{
	case EnemyActor::STATE_WALL::NONE:
		GetTransform().SetWorldMove(float4::DOWN * MoveSpeed * _DeltaTime);
		break;
	case EnemyActor::STATE_WALL::RIGHT:
		break;
	case EnemyActor::STATE_WALL::LEFT:
		break;
	case EnemyActor::STATE_WALL::UP:
		break;
	case EnemyActor::STATE_WALL::DOWN:
		break;
	case EnemyActor::STATE_WALL::UNDERGROUND:
		GetTransform().SetWorldMove(float4::UP * MoveSpeed * _DeltaTime);
		break;
	case EnemyActor::STATE_WALL::RIGHTSLOPE:
		break;
	case EnemyActor::STATE_WALL::LEFTSLOPE:
		break;
	default:
		break;
	}

}

void EnemyGrunt::IdleStart(const StateInfo& _Info)
{
	Renderer_Character->ChangeFrameAnimation("idle");
}

void EnemyGrunt::IdleUpdate(float _DeltaTime, const StateInfo& _Info)
{
	switch (CurAction)
	{
	case EnemyActor::ENEMYACTION::PATROL:

		break;
	case EnemyActor::ENEMYACTION::CHASE:
		break;
	case EnemyActor::ENEMYACTION::ATTACK:
		break;
	default:
		break;
	}
}

void EnemyGrunt::DeathStart(const StateInfo& _Info)
{
	Renderer_Character->ChangeFrameAnimation("hurtground");
}

void EnemyGrunt::DeathUpdate(float _DeltaTime, const StateInfo& _Info)
{

}

