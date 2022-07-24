#include "PreCompile.h"
#include "TitleGrass.h"
#include <GameEngineCore/CoreMinimal.h>

const float LERPIN_DIST = 450.0f;

TitleGrass::TitleGrass() 
{
}

TitleGrass::~TitleGrass() 
{
}

void TitleGrass::Start()
{
	Renderer = CreateComponent<GameEngineTextureRenderer>();
	Renderer->SetTexture("spr_title_grass_0.png");
	Renderer->ScaleToTexture();

	GetTransform().SetWorldScale({ 1.5f, 1.5f, 1 });

	// Lerp
	LerpStart({ 0, -720}, { 0, -270 }, 0.7f);
}

void TitleGrass::Update(float _DeltaTime)
{
	LerpUpdate(_DeltaTime);
}

void TitleGrass::End()
{
}
