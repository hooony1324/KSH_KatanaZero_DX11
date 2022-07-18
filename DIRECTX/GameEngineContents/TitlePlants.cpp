#include "PreCompile.h"
#include "TitlePlants.h"
#include <GameEngineCore/CoreMinimal.h>

TitlePlants::TitlePlants() 
{
}

TitlePlants::~TitlePlants() 
{
}

void TitlePlants::Start()
{
	Renderer = CreateComponent<GameEngineTextureRenderer>();
	Renderer->GetTransform().SetLocalScale({ 640, 360, 0 });
	Renderer->GetTransform().SetLocalPosition({ 0, -160, 0 });
	Renderer->CreateFrameAnimationFolder("title_plants", FrameAnimation_DESC{ "title_plants", 0.1f });
	Renderer->ChangeFrameAnimation("title_plants");
}

void TitlePlants::Update(float _DeltaTime)
{
}

void TitlePlants::End()
{
}
