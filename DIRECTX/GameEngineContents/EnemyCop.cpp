#include "PreCompile.h"
#include "EnemyCop.h"
#include <GameEngineCore/CoreMinimal.h>

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

	// 기본 정보
	Hp = 1;
	MoveSpeed = 150.0f;
	StateManager.ChangeState("Spawn");
	GetTransform().SetLocalScale({ 2, 2, 1 });
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

