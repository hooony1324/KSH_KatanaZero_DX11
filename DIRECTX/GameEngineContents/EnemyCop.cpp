#include "PreCompile.h"
#include "EnemyCop.h"
#include "EnemyBullet.h"

EnemyCop::EnemyCop() 
{
	EnemyName = "cop";
}

EnemyCop::~EnemyCop() 
{
}

void EnemyCop::Start()
{
	CreateRendererAndCollision();
	CreateAllFolderAnimation();
	CreateAllState();

	// Attack관련 오버라이드
	Collision_ChaseSensor->GetTransform().SetLocalScale({ 300, 50, GetDepth(ACTOR_DEPTH::COLLISION) });

	StateManager.CreateStateMember("Attack"
		, std::bind(&EnemyCop::AttackUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&EnemyCop::AttackStart, this, std::placeholders::_1));


	// 기본 정보
	Hp = 1;
	MoveSpeed = 150.0f;
	AttackRange = 500.0f;
	ChaseSensorPaddingX = 100.0f;
	IsChasingEnemy = false;
	StateManager.ChangeState("Spawn");
	GetTransform().SetLocalScale({ 2, 2, 1 });

	// Aim -> Shoot
	Renderer_GunArm->SetTexture("spr_cop_aim_rightarm.png");
	Renderer_GunArm->ScaleToTexture();
	Renderer_GunArm->GetTransform().SetLocalPosition({ 0, 20, 0 });
	Renderer_GunArm->Off();

	AttackStateManager.CreateStateMember("Aim"
		, std::bind(&EnemyCop::AimUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&EnemyCop::AimStart, this, std::placeholders::_1));

	AttackStateManager.CreateStateMember("Shoot"
		, std::bind(&EnemyCop::ShootUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&EnemyCop::ShootStart, this, std::placeholders::_1));

	Collision_Aim->On();
}

void EnemyCop::Update(float _DeltaTime)
{
	PlayerAttackCheck();

	// 벽
	WallCheck();

	// 액션 체크
	PlayerAlertCheck();

	PlayerLeftRightCheck();
	// 상태에 따라 MoveVec 결정
	StateManager.Update(_DeltaTime);

	// 움직임 상태 최종 MoveVec
	Move(_DeltaTime);


	//EnemyActorDebug();

	
}

void EnemyCop::End()
{
}

void EnemyCop::AttackStart(const StateInfo& _Info)
{
	Renderer_Character->ChangeFrameAnimation("attack");
	MoveVec.x = 0;

	AttackStateManager.ChangeState("Aim");
}

void EnemyCop::AttackUpdate(float _DeltaTime, const StateInfo& _Info)
{
	// 플레이어 사정거리 밖이라면 Run
	if (abs((EnemyPos - PlayerPos).Length()) > AttackRange)
	{
		Renderer_GunArm->Off();
		StateManager.ChangeState("Run");
		return;
	}

	// Aim -> Shoot
	AttackStateManager.Update(_DeltaTime);
}

void EnemyCop::AimStart(const StateInfo& _Info)
{
	Renderer_GunArm->On();
	if (PrevLookDir > 0)
	{
		Renderer_GunArm->GetTransform().PixLocalPositiveX();
	}
	else
	{
		Renderer_GunArm->GetTransform().PixLocalNegativeX();
	}

}

void EnemyCop::AimUpdate(float _DeltaTime, const StateInfo& _Info)
{
	// 조준하고 
	AimDir = (PlayerPos - EnemyPos).NormalizeReturn();

	// 중간에 플레이어가 반대로 넘어가면 렌더러방향 바꿈

	// 중간에 장애물 있으면 Run으로

	// 일정 시간 뒤 발사
	if (_Info.StateTime > 0.8f)
	{
		AttackStateManager.ChangeState("Shoot");
		return;
	}

}


Bullet* Blt;
void EnemyCop::ShootStart(const StateInfo& _Info)
{
	Blt = GetLevel()->CreateActor<EnemyBullet>();
	
	Blt->Instance(EnemyPos + float4{ 0, 30, GetDepth(ACTOR_DEPTH::FX)}, AimDir);
}

void EnemyCop::ShootUpdate(float _DeltaTime, const StateInfo& _Info)
{
	// 한번 쏘고 쿨타임
	if (_Info.StateTime > 1.0f)
	{
		AttackStateManager.ChangeState("Aim");
	}
}

// RAY??
bool EnemyCop::PointToPlayer()
{


	return false;
}

