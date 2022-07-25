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


	// ȭ�� �� �׸���
	GameEngineDefaultRenderer* Renderer_Shadow = CreateComponent<GameEngineDefaultRenderer>();
	Renderer_Shadow->GetTransform().SetLocalScale({ 1280, 1000 });
	Renderer_Shadow->GetTransform().SetLocalPosition({ 0, -560 });
	Renderer_Shadow->SetPipeLine("Color");
	Color_Shadow = float4{ 0, 0, 0, 1 };
	Renderer_Shadow->ShaderResources.SetConstantBufferLink("ResultColor", Color_Shadow);
	Renderer_Shadow->GetTransform().SetWorldMove({ 0, 0, -100 });
}

void TitleGrass::Update(float _DeltaTime)
{
	LerpUpdate(_DeltaTime);
}

void TitleGrass::End()
{
}
