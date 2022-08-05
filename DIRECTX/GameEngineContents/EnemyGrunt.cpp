#include "PreCompile.h"
#include "EnemyGrunt.h"



EnemyGrunt::EnemyGrunt() 
{
	
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
	Hp = 1;
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

	// 상태에 따라 MoveVec 결정
	StateManager.Update(_DeltaTime);

	// 움직임 상태 최종 MoveVec
	Move(_DeltaTime);

}

void EnemyGrunt::End()
{
}

bool EnemyGrunt::PlayerAttackCheck(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	if (Hp == 0)
	{
		return false;
	}

	// 플레이어의 공격 위치를 받아서 반대로 날려짐

	Hp--;
	if (Hp <= 0)
	{
		FlyVec = _This->GetTransform().GetWorldPosition() - _Other->GetTransform().GetWorldPosition();
		FlyVec.z = 0;
		FlyVec.Normalize();
		StateManager.ChangeState("Dead");
	}


	return true;
}

void EnemyGrunt::Move(float _DeltaTime)
{



	switch (WallState)
	{
	case EnemyActor::STATE_WALL::NONE:
		break;
	case EnemyActor::STATE_WALL::RIGHT:
		break;
	case EnemyActor::STATE_WALL::LEFT:
		break;
	case EnemyActor::STATE_WALL::UP:
		MoveVec.y = -0.02f;
		break;
	case EnemyActor::STATE_WALL::DOWN:
		break;
	case EnemyActor::STATE_WALL::UNDERGROUND:
		MoveVec.y = 1;
		break;
	case EnemyActor::STATE_WALL::RIGHTSLOPE:
		break;
	case EnemyActor::STATE_WALL::LEFTSLOPE:
		break;
	default:
		break;
	}

	Velocity = MoveVec * MoveSpeed* _DeltaTime;
	GetTransform().SetWorldMove(Velocity);
}

void EnemyGrunt::IdleStart(const StateInfo& _Info)
{
	Renderer_Character->ChangeFrameAnimation("idle");
}

void EnemyGrunt::IdleUpdate(float _DeltaTime, const StateInfo& _Info)
{
	

}

void EnemyGrunt::DeathStart(const StateInfo& _Info)
{
	Renderer_Character->ChangeFrameAnimation("hurtfly");

	MoveVec = FlyVec * 1.2f;
	FlyRadian = float4::VectorXYtoRadian({ 0, 0 }, FlyVec);
	MoveSpeed *= 1.5f;
}

void EnemyGrunt::DeathUpdate(float _DeltaTime, const StateInfo& _Info)
{
	float DT = _Info.StateTime;

	static bool Played = false;
	if (WallState == STATE_WALL::DOWN)
	{
		if (false == Played)
		{
			Death(3.0f);
			Renderer_Character->ChangeFrameAnimation("hurtground");
			Played = true;
		}
		MoveVec.y = 0;
		MoveVec.x = GameEngineMath::LerpLimit(FlyVec.x, 0, DT);
	}
	else
	{
		MoveVec.y = FlyVec.y * sinf(FlyRadian) - (9.8f * DT) / 6.0f;
	}
}

