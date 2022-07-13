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
	Renderer = CreateComponent<GameEngineDefaultRenderer>();
	Renderer->GetTransform().SetLocalScale({ 640, 720, 0 });
	Renderer->SetPipeLine("Color");
	Renderer->PipeLineHelper.SetConstantBufferLink("ResultColor", BackColor);

	BackColor.r = 0;
	BackColor.g = 0;
	BackColor.b = 0;
}

void TitlePlate::Update(float _DeltaTime)
{

}

void TitlePlate::End()
{
}
