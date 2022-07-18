#include "PreCompile.h"
#include "TitlePlate.h"
#include <GameEngineCore/CoreMinimal.h>

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
	Color_Background.w = 0.8f;

	Renderer = CreateComponent<GameEngineTextureRenderer>();
	Renderer->GetTransform().SetLocalScale({ 1280, 1440, 0 });
	Renderer->GetTransform().SetWorldMove({ 0, 100, 0 });
	Renderer->SetTexture("spr_title_background_0.png");
}

void TitlePlate::Update(float _DeltaTime)
{

}

void TitlePlate::End()
{
}
