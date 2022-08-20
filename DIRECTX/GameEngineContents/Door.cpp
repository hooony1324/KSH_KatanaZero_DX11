#include "PreCompile.h"
#include "Door.h"
#include <GameEngineCore/CoreMinimal.h>

Door::Door() 
{
}

Door::~Door() 
{
}

void Door::Close()
{
	Collision->On();
	Renderer->ChangeFrameAnimation("idle");
}

bool Door::Open(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	Collision->Off();
	Renderer->ChangeFrameAnimation("open");

	return true;
}

void Door::Start()
{
	Renderer = CreateComponent<GameEngineTextureRenderer>();
	Renderer->SetOrder(static_cast<int>(ACTORGROUP::TIMEGROUP));

	Renderer->SetTexture("spr_door_animation_0.png");
	Renderer->SetSamplingModePoint();
	Renderer->ScaleToTexture();
	Renderer->GetTransform().PixLocalNegativeX();
	Renderer->CreateFrameAnimationFolder("idle", FrameAnimation_DESC{ "door_idle", 0.1f, true });
	Renderer->CreateFrameAnimationFolder("open", FrameAnimation_DESC{ "door_open", 0.1f, false });
	Renderer->ChangeFrameAnimation("idle");

	Collision = CreateComponent<GameEngineCollision>();
	float4 Scale = Renderer->GetTransform().GetLocalScale();
	Collision->GetTransform().SetLocalScale(float4{ 15, Scale.y, GetDepth(ACTOR_DEPTH::DOOR) });
	Collision->GetTransform().SetLocalPosition({ 20, 0 , 0 });
	Collision->ChangeOrder(COLLISIONGROUP::DOOR);
	Collision->SetDebugSetting(CollisionType::CT_AABB2D, { 1, 0, 0, 0.25f });

	// -330, 72
	GetTransform().SetLocalScale({ 2.0f, 2.0f, 1 });
	GetTransform().SetWorldPosition({ 0, 0, GetDepth(ACTOR_DEPTH::DOOR) });

}

void Door::Update(float _DeltaTime)
{
	
	Collision->IsCollision(CollisionType::CT_AABB2D, COLLISIONGROUP::PLAYER_ATTACK, CollisionType::CT_AABB2D
		, std::bind(&Door::Open, this, std::placeholders::_1, std::placeholders::_2));
}

void Door::End()
{
}
