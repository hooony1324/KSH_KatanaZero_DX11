#include "PreCompile.h"
#include "Stair.h"
#include <GameEngineCore/CoreMinimal.h>

Stair* Stair::PlayerNearestStair = nullptr;

// 1층 > 2층 > 3층 순으로 Enemy탐색후 3층 2층 1층 내려온다
void Stair::SearchEnemyPassingDownStairs(float _EnemyPosY, std::vector<Stair*>& _PassbyStairs)
{
	_PassbyStairs.clear();
	Stair* CurStair = PlayerNearestStair;

	while (nullptr != CurStair)
	{
		float DistanceY = CurStair->GetTransform().GetWorldPosition().y - _EnemyPosY;
		bool SameFloor;
		if (DistanceY > 35)
		{
			SameFloor = false;
		}
		else if (DistanceY < -240)
		{
			SameFloor = false;
		}
		else
		{
			SameFloor = true;
		}

		if (false == SameFloor)
		{
			_PassbyStairs.push_back(CurStair);
			CurStair = CurStair->UpStair;
		}
		else
		{
			_PassbyStairs.push_back(CurStair);
			break;
		}
	}

}

void Stair::SearchEnemyPassingUpStairs(float _EnemyPosY, std::vector<Stair*>& _PassbyStairs)
{
	_PassbyStairs.clear();
	Stair* CurStair = PlayerNearestStair;

	while (nullptr != CurStair)
	{
		bool SameFloor = abs(CurStair->GetTransform().GetWorldPosition().y - _EnemyPosY < 40);
		if (true == SameFloor)
		{
			break;
		}
		else
		{
			_PassbyStairs.push_back(CurStair);
			CurStair = CurStair->DownStair;
		}
	}
}

Stair::Stair()
{
}

Stair::~Stair() 
{
}

void Stair::Start()
{
	Collision = CreateComponent<GameEngineCollision>();
	Collision->GetTransform().SetLocalScale({ 10, 10, GetDepth(ACTOR_DEPTH::COLLISION) });
	Collision->SetDebugSetting(CollisionType::CT_AABB2D, { 1.0f, 0.8f, 0.0f, 0.25f });
	Collision->ChangeOrder(COLLISIONGROUP::STAIR);


	FRenderer_Debug = CreateComponent<GameEngineFontRenderer>();
	FRenderer_Debug->SetText("Player Here!");
	FRenderer_Debug->SetSize(16);
	FRenderer_Debug->SetLeftAndRightSort(LeftAndRightSort::CENTER);
	FRenderer_Debug->SetPositionMode(FontPositionMode::WORLD);
	FRenderer_Debug->GetTransform().SetLocalPosition({ 0, 30, 0 });
	FRenderer_Debug->Off();
	

	Off();
}

void Stair::Spawn(float4 _WorldPos, Stair* _Up, Stair* _Down)
{
	UpStair = _Up;
	DownStair = _Down;

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
