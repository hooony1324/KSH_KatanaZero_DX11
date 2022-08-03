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
	Renderer->GetTransform().SetLocalScale(float4{ 36, 95, 1 } * 2.0f);
	Renderer->CreateFrameAnimationFolder("spin", FrameAnimation_DESC{ "fanblade_spin", 0.004f });
	Renderer->ChangeFrameAnimation("spin");


}

void Fan::Update(float _DeltaTime)
{
}

void Fan::End()
{
}
