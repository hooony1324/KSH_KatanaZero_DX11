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
	Background = CreateComponent<GameEngineDefaultRenderer>();
	Background->GetTransform().SetLocalScale({ 1280, 1440, 0 });
	Background->SetPipeLine("color");
	Background->ShaderResources.SetConstantBufferLink("RESULTCOLOR", Color_Background);
	Color_Background.r = 0;
	Color_Background.g = 0;
	Color_Background.b = 0;
	Color_Background.w = 1.0f;

	Renderer = CreateComponent<GameEngineTextureRenderer>();
	Renderer->SetTexture("spr_title_background_0.png");
	Renderer->GetTransform().SetLocalScale({ 1280, 1440, 0 });

	LerpStart({ 0, 100 }, { 0, 300 }, 0.3f);
}

void TitlePlate::Update(float _DeltaTime)
{
	LerpUpdate(_DeltaTime);
}

void TitlePlate::End()
{
}
