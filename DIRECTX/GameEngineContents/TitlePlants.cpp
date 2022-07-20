#include "PreCompile.h"
#include "TitlePlants.h"
#include <GameEngineCore/CoreMinimal.h>

const float LERPIN_DIST = 1000.0f;

TitlePlants::TitlePlants() 
{
}

TitlePlants::~TitlePlants() 
{
}

void TitlePlants::Start()
{
	Renderer = CreateComponent<GameEngineTextureRenderer>();
	Renderer->SetTexture("spr_title_grass_0.png");
	Renderer->GetTransform().SetLocalScale({ 1280, 720 });
	Renderer->CreateFrameAnimationFolder("title_plants", FrameAnimation_DESC{ "title_plants", 0.1f });
	Renderer->ChangeFrameAnimation("title_plants");

	GetTransform().SetWorldPosition({ 0, -1000, 0 });

	// Lerp
	LerpStart({ 0, -1000, 0 }, { 0, 0, 0 }, 0.8f);
}

void TitlePlants::Update(float _DeltaTime)
{
	LerpUpdate(_DeltaTime);
}

void TitlePlants::End()
{
}