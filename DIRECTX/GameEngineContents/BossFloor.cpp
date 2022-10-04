#include "PreCompile.h"
#include "BossFloor.h"
#include <GameEngineCore/CoreMinimal.h>

float4 BossFloor::Pos = float4::ZERO;

BossFloor::BossFloor() 
{
}

BossFloor::~BossFloor() 
{
}

void BossFloor::Start()
{
	//GameEngineTextureRenderer* Background = CreateComponent<GameEngineTextureRenderer>();

	Renderer = CreateComponent<GameContentsEffectRenderer>();
	Renderer->SetConcaveEffect();
	Renderer->SetTexture("spr_psychboss_floor_0.png");
	Renderer->SetScaleRatio(2.0f);
	Renderer->ScaleToTexture();

	FloorScale = Renderer->GetTransform().GetLocalScale();
	Collision = CreateComponent<GameEngineCollision>();
	Collision->GetTransform().SetLocalScale({ FloorScale.x , FloorScale.y, GetDepth(ACTOR_DEPTH::COLLISION)});
	Collision->ChangeOrder(COLLISIONGROUP::BOSSFLOOR);
	Collision->SetDebugSetting(CollisionType::CT_AABB2D, float4{ 0.7f, 1.0f, 0.0f, 0.2f });

	
}

void BossFloor::Update(float _DeltaTime)
{
	Pos = Collision->GetTransform().GetWorldPosition();

	float4& TexConcavePos = Renderer->GetConcaveData().ConcavePos;
	TexConcavePos = float4(-1.0f, -1.0f, -1.0f, -1.0f);

	bool PlayerConcave = false;
	bool BossConcave = false;

	// 충돌체가 있으면 누름(Concave)
	Collision->IsCollision(CollisionType::CT_AABB2D, COLLISIONGROUP::PLAYER, CollisionType::CT_AABB2D,
		[&](GameEngineCollision* _This, GameEngineCollision* _Other)
		{
			PlayerConcave = true;
			float4 FloorPos = _This->GetTransform().GetWorldPosition() + float4(-FloorScale.x * 0.5f, FloorScale.y * 2.0f);
			float4 ColPos = _Other->GetTransform().GetWorldPosition();
			

			// 누르기
			if (ColPos.y < FloorPos.y)
			{
				//				0 ~ 1	 0 ~ 1   0 ~ 1  0 ~ 1
				// ConcavePos{ PlayerX, PlayerY, BossX, BossY }
				// 00 ~ 11로

				float4 ConcavePos = FloorPos - ColPos;
				ConcavePos.x = std::clamp(abs(ConcavePos.x / FloorScale.x), 0.0f, 1.0f);
				ConcavePos.y = std::clamp(abs(ConcavePos.y / FloorScale.y), 0.0f, 1.0f);
				
				TexConcavePos.x = ConcavePos.x;
				TexConcavePos.y = ConcavePos.y;
			}
			// 떼기
			else
			{
				TexConcavePos.x = -1.0f;
				TexConcavePos.y = -1.0f;
			}

			return CollisionReturn::ContinueCheck;
		});

	Collision->IsCollision(CollisionType::CT_AABB2D, COLLISIONGROUP::BOSSTENTACLE, CollisionType::CT_AABB2D,
		[&](GameEngineCollision* _This, GameEngineCollision* _Other)
		{
			BossConcave = true;
			Renderer->GetConcaveData().ConcaveStrength = 3.5f;
			float4 FloorPos = _This->GetTransform().GetWorldPosition() + float4(-FloorScale.x * 0.5f, FloorScale.y * 2.0f);
			float4 ColPos = _Other->GetTransform().GetWorldPosition();

			// 누르기
			if (ColPos.y < FloorPos.y)
			{
				//				0 ~ 1	 0 ~ 1   0 ~ 1  0 ~ 1
				// ConcavePos{ PlayerX, PlayerY, BossX, BossY }
				// 00 ~ 11로

				float4 ConcavePos = FloorPos - ColPos;
				ConcavePos.x = std::clamp(abs(ConcavePos.x / FloorScale.x), 0.0f, 1.0f);
				ConcavePos.y = std::clamp(abs(ConcavePos.y / FloorScale.y), 0.0f, 1.0f);

				TexConcavePos.z = ConcavePos.x;
				TexConcavePos.w = ConcavePos.y;
			}
			// 떼기
			else
			{
				TexConcavePos.z = -1.0f;
				TexConcavePos.w = -1.0f;
			}

			return CollisionReturn::ContinueCheck;
		});

	

}

void BossFloor::End()
{
}
