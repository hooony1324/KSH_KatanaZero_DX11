#include "PreCompile.h"
#include "Stair.h"
#include <GameEngineCore/CoreMinimal.h>

Stair* Stair::PlayerNearestStair = nullptr;

Stair::Stair()
{
}

Stair::~Stair() 
{
}

void Stair::Start()
{
	Collision = CreateComponent<GameEngineCollision>();
	Collision->GetTransform().SetLocalScale({ 20, 20, GetDepth(ACTOR_DEPTH::COLLISION) });
	Collision->SetDebugSetting(CollisionType::CT_AABB2D, { 1.0f, 0.8f, 0.0f, 0.25f });
	Collision->ChangeOrder(COLLISIONGROUP::STAIR);


	FRenderer_Debug = CreateComponent<GameEngineFontRenderer>();
	FRenderer_Debug->SetText("Player Here!");
	FRenderer_Debug->SetSize(20);
	FRenderer_Debug->SetLeftAndRightSort(LeftAndRightSort::CENTER);
	FRenderer_Debug->SetPositionMode(FontPositionMode::WORLD);
	FRenderer_Debug->GetTransform().SetLocalPosition({ 0, 40, 0 });
	FRenderer_Debug->Off();
	

	Off();
}

void Stair::Spawn(float4 _WorldPos, Stair* _Up, Stair* _Down)
{
	UpStairs.push_back(_Up);
	DownStairs.push_back(_Down);

	GetTransform().SetWorldPosition({ _WorldPos.x, _WorldPos.y, GetDepth(ACTOR_DEPTH::BACKGROUND_COL) });
}

void Stair::Update(float _DeltaTime)
{

	// 플레이어와 같은층 판단
	float4 PlayerPos = GlobalValueManager::PlayerPos;
	float4 StairPos = GetTransform().GetWorldPosition();

	float DistanceY = PlayerPos.y - StairPos.y;

	if (DistanceY <= 230 && DistanceY > -40)
	{
		PlayerOnFloor = true;
		float DistanceX = PlayerPos.x - StairPos.x;
		PlayerDistance = float4(DistanceX, DistanceY).Length(); // 플레이어와 가장 가까운 계단고르기 위함
	}
	else
	{
		PlayerDistance = 500;
		PlayerOnFloor = false;
	}

	// 같은층만 계산할거
	if (false == PlayerOnFloor)
	{
		FRenderer_Debug->Off();
		return;
	}

	// 가장 가까운 계단만 목표
	if (PlayerNearestStair == nullptr)
	{
		PlayerNearestStair = this;
	}
	else
	{
		PlayerNearestStair = PlayerNearestStair->PlayerDistance < this->PlayerDistance ? PlayerNearestStair : this;
	}

	// "Player Here!"
	if (true == GEngine::IsCollisionDebug() && this == PlayerNearestStair)
	{
		FRenderer_Debug->On();
	}
	else
	{
		FRenderer_Debug->Off();
	}
	
}

void Stair::End()
{
}
