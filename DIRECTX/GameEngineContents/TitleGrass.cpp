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
	Renderer->GetTransform().SetWorldPosition({ 0, 0, static_cast<float>(TITLE_DEPTH::GRASS) });

	GetTransform().SetWorldScale({ 1.5f, 1.5f, 1 });

	// Lerp
	LerpStart({ 0, -720}, { 0, -270 }, 0.7f);


	// 화면 밑 그림자
	GameEngineDefaultRenderer* Renderer_Shadow = CreateComponent<GameEngineDefaultRenderer>();
	Renderer_Shadow->GetTransform().SetLocalScale({ 1280, 1000 });
	Renderer_Shadow->SetPipeLine("Color");
	Color_Shadow = float4{ 0, 0, 0, 1 };
	Renderer_Shadow->ShaderResources.SetConstantBufferLink("ResultColor", Color_Shadow);
	Renderer_Shadow->GetTransform().SetWorldPosition({ 0, -840, static_cast<float>(TITLE_DEPTH::GRASS) });
}

void TitleGrass::Update(float _DeltaTime)
{
	LerpUpdate(_DeltaTime);
}

void TitleGrass::End()
{
}
