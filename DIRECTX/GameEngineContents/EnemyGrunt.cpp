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

	// 기본 정보
	Hp = 1;
	MoveSpeed = 150.0f;
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

	// 상태에 따라 MoveVec 결정
	StateManager.Update(_DeltaTime);

	// 움직임 상태 최종 MoveVec
	Move(_DeltaTime);


	EnemyActorDebug();
}

void EnemyGrunt::End()
{
}


