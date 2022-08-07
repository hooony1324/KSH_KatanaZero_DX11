#include "PreCompile.h"
#include "EnemyGrunt.h"



EnemyGrunt::EnemyGrunt() 
{
	EnemyName = "grunt";
}

EnemyGrunt::~EnemyGrunt() 
{
}

void EnemyGrunt::Start()
{
	// 초기화
	CreateRendererAndCollision();
	CreateAllFolderAnimation();
	CreateAllState();
	StateManager.CreateStateMember("Attack"
		, std::bind(&EnemyGrunt::AttackUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&EnemyGrunt::AttackStart, this, std::placeholders::_1));

	// 기본 정보
	Hp = 1;
	MoveSpeed = 150.0f;
	AttackRange = 80.0f;
	StateManager.ChangeState("Spawn");
	GetTransform().SetLocalScale({ 2, 2, 1 });
}

void EnemyGrunt::Update(float _DeltaTime)
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


	EnemyActorDebug();
}

void EnemyGrunt::End()
{
}

void EnemyGrunt::AttackStart(const StateInfo& _Info)
{
	Renderer_Character->ChangeFrameAnimation("attack");
	MoveVec.x = 0;
}

void EnemyGrunt::AttackUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (false == AttackAniEnd)
	{
		return;
	}

	if (_Info.StateTime < 1.0f)
	{
		return;
	}

	// 턴 할지 뛸 지 결정
	if (PrevLookDir == PlayerDir)
	{
		AttackAniEnd = true;
		StateManager.ChangeState("Run");
	}
	else
	{
		AttackAniEnd = true;
		StateManager.ChangeState("ChaseTurn");
	}


}


