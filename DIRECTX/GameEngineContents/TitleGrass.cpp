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
	Renderer->GetTransform().SetLocalPosition({ 0, 0, 0});

	GetTransform().SetWorldScale({ 1.5f, 1.5f, 1 });

	// 화면 밑 그림자
	GameEngineTextureRenderer* Renderer_Shadow = CreateComponent<GameEngineTextureRenderer>();
	Renderer_Shadow->SetTexture("spr_title_background_black.png");
	Renderer_Shadow->GetTransform().SetLocalScale({ 1280, 1000 });
	Renderer_Shadow->GetTransform().SetLocalPosition({ 0, -540, -10 });

	// Lerp
	float Depth = static_cast<float>(TITLE_DEPTH::GRASS);
	LerpStart({ 0, -720, Depth }, { 0, -270, Depth }, 0.7f);
}

void TitleGrass::Update(float _DeltaTime)
{
	LerpUpdate(_DeltaTime);
}

void TitleGrass::End()
{
}
