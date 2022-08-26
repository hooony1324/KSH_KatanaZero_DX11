#include "PreCompile.h"
#include "Fan.h"
#include <GameEngineCore/CoreMinimal.h>

Fan::Fan() 
{
}

Fan::~Fan() 
{
}

void Fan::Start()
{
	Renderer = CreateComponent<GameEngineTextureRenderer>();
	Renderer->SetOrder(static_cast<int>(ACTORGROUP::TIMEGROUP_FAN));

	Renderer->GetTransform().SetLocalScale(float4{ 36, 95, 1 } * 2.0f);
	Renderer->CreateFrameAnimationFolder("spin", FrameAnimation_DESC{ "fanblade_spin", 0.008f });
	Renderer->ChangeFrameAnimation("spin");


	Collision = CreateComponent<GameEngineCollision>();
	Collision->GetTransform().SetLocalScale({ 72, 190, GetDepth(ACTOR_DEPTH::COLLISION) });
	Collision->ChangeOrder(COLLISIONGROUP::ENEMY_ATTACK);
	Collision->SetDebugSetting(CollisionType::CT_AABB2D, { 1, 0, 0, 0.25f });

	Renderer->AnimationBindFrame("spin", [=]
	(const FrameAnimation_DESC& _Info)
		{
			_Info.CurFrame;
			if (_Info.CurFrame > 6 && _Info.CurFrame < 25)
			{
				Renderer->GetPixelData().MulColor = { 1.0f, 1.0f, 1.0f, 1.0f };
				Collision->Off();
			}
			else
			{
				if (false == GameEngineInput::GetInst()->IsFree("Shift"))
				{
					Renderer->GetPixelData().MulColor = { 1.5f, 0.5f, 0.5f, 1.0f };
				}
				Collision->On();
			}
		}
	);

}

void Fan::OnEvent()
{
	
}

void Fan::Update(float _DeltaTime)
{

}

void Fan::End()
{
}


