#include "PreCompile.h"
#include "EnemyCop.h"

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
	AttackRange = 250.0f;
	ChaseSensorPaddingX = 100.0f;
	StateManager.ChangeState("Spawn");
	GetTransform().SetLocalScale({ 2, 2, 1 });

	// Aim -> Shoot
	Renderer_GunArm = CreateComponent<GameEngineTextureRenderer>();
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

	Collision_Aim = CreateComponent<GameEngineCollision>();
	Collision_Aim->GetTransform().SetLocalScale({ 200, 4, GetDepth(ACTOR_DEPTH::COLLISION) });
	Collision_Aim->GetTransform().SetLocalPosition({ 0, 18, 0 });
	Collision_Aim->ChangeOrder(COLLISIONGROUP::ENEMY_AIM);
	Collision_Aim->SetDebugSetting(CollisionType::CT_OBB2D, { 0, 1, 0, 0.5f });
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

	Renderer_GunArm->On();
	AttackStateManager.ChangeState("Aim");
}

void EnemyCop::AttackUpdate(float _DeltaTime, const StateInfo& _Info)
{
	AttackStateManager.Update(_DeltaTime);
}

void EnemyCop::AimStart(const StateInfo& _Info)
{

}

void EnemyCop::AimUpdate(float _DeltaTime, const StateInfo& _Info)
{
}

void EnemyCop::ShootStart(const StateInfo& _Info)
{
}

void EnemyCop::ShootUpdate(float _DeltaTime, const StateInfo& _Info)
{
}

