#include "PreCompile.h"
#include "Door.h"
#include <GameEngineCore/CoreMinimal.h>

Door::Door() 
{
}

Door::~Door() 
{
}

void Door::Init()
{
	Renderer->ChangeFrameAnimation("open");
}

void Door::Start()
{
	Renderer = CreateComponent<GameEngineTextureRenderer>();
	Renderer->SetTexture("spr_door_animation_0.png");
	Renderer->ScaleToTexture();
	Renderer->GetTransform().PixLocalNegativeX();
	Renderer->CreateFrameAnimationFolder("idle", FrameAnimation_DESC{ "door_idle", 0.1f, true });
	Renderer->CreateFrameAnimationFolder("open", FrameAnimation_DESC{ "door_open", 0.1f, true });
	Renderer->ChangeFrameAnimation("idle");

	// -330, 72
	Renderer->GetTransform().SetWorldPosition({ 0, 0, GetDepth(ACTOR_DEPTH::DOOR) });
	GetTransform().SetLocalScale({ 2.0f, 2.0f, 1 });

}

void Door::Update(float _DeltaTime)
{

	auto val = GetTransform().GetWorldPosition();
}

void Door::End()
{
}
