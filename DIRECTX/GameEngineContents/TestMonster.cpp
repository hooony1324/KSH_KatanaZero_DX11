#include "PreCompile.h"
#include "TestMonster.h"
#include <GameEngineCore/CoreMinimal.h>

TestMonster::TestMonster() 
{
}

TestMonster::~TestMonster() 
{
}

void TestMonster::Start()
{
	//Collision = CreateComponent<GameEngineCollision>();
	//Collision->GetTransform().SetLocalScale({ 100, 100, 10000 });
	//Collision->SetDebugSetting(CollisionType::CT_AABB2D, { 1, 1, 1, 0.5f });
	//Collision->ChangeOrder(COLLISIONGROUP::ENEMY);
	//Collision->SetCollisionMode(CollisionMode::Ex);

	GetTransform().SetWorldPosition({ 200, 100, 0 });
}

void TestMonster::Update(float _DeltaTime)
{
	if (GameEngineInput::GetInst()->IsPress("w"))
	{
		GetTransform().SetWorldUpMove(100, _DeltaTime);
	}
	if (GameEngineInput::GetInst()->IsPress("a"))
	{
		GetTransform().SetWorldLeftMove(100, _DeltaTime);
	}
	if (GameEngineInput::GetInst()->IsPress("s"))
	{
		GetTransform().SetWorldDownMove(100, _DeltaTime);
	}
	if (GameEngineInput::GetInst()->IsPress("d"))
	{
		GetTransform().SetWorldRightMove(100, _DeltaTime);
	}

	Collision->IsCollisionEnterBase(CollisionType::CT_AABB2D, static_cast<int>(COLLISIONGROUP::PLAYER), CollisionType::CT_AABB2D
		, std::bind(&TestMonster::BlockedPlayer, this, std::placeholders::_1, std::placeholders::_2));
}

void TestMonster::End()
{
}

CollisionReturn TestMonster::BlockedPlayer(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	int a = 0;

	return CollisionReturn::ContinueCheck;
}
