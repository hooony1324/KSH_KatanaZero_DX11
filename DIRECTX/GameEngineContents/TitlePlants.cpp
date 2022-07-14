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
	Renderer->GetTransform().SetLocalPosition({ 0, -320, 0 });
	Renderer->SetTexture("spr_title_plants_0.png");
}

void TitlePlants::Update(float _DeltaTime)
{
}

void TitlePlants::End()
{
}
