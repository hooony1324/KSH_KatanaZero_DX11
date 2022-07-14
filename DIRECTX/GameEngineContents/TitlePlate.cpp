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
	Renderer = CreateComponent<GameEngineTextureRenderer>();
	Renderer->GetTransform().SetLocalScale({ 640, 720, 0 });
	Renderer->SetTexture("spr_title_background_0.png");
}

void TitlePlate::Update(float _DeltaTime)
{

}

void TitlePlate::End()
{
}
