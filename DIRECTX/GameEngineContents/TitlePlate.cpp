#include "PreCompile.h"
#include "TitlePlate.h"
#include <GameEngineCore/CoreMinimal.h>

const float LERPIN_DIST = 200.0f;

TitlePlate::TitlePlate() 
{
}

TitlePlate::~TitlePlate() 
{
}

void TitlePlate::Start()
{
	Background = CreateComponent<GameEngineTextureRenderer>();
	Background->SetTexture("spr_title_background_black.png");
	Background->GetTransform().SetLocalScale({ 1280, 1440, 1 });
	Background->GetTransform().SetLocalPosition({0, 0, 10});

	Renderer = CreateComponent<GameEngineTextureRenderer>();
	Renderer->SetTexture("spr_title_background_0.png");
	Renderer->GetTransform().SetLocalScale({ 1280, 1440, 1 });
	Renderer->GetTransform().SetLocalPosition({ 0, 0, -1});

	float Depth = static_cast<float>(TITLE_DEPTH::PLATE);
	LerpStart({ 0, 100, Depth }, { 0, 300, Depth }, 0.3f);
}

void TitlePlate::Update(float _DeltaTime)
{
	LerpUpdate(_DeltaTime);
}

void TitlePlate::End()
{
}
