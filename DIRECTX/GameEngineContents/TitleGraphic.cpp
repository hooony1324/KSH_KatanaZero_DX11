#include "PreCompile.h"
#include "TitleGraphic.h"
#include <GameEngineCore/CoreMinimal.h>

TitleGraphic::TitleGraphic() 
{
}

TitleGraphic::~TitleGraphic() 
{
}

void TitleGraphic::Start()
{
	Renderer = CreateComponent<GameEngineTextureRenderer>();
	Renderer->GetTransform().SetLocalScale({ 639, 360, 0 });
	Renderer->SetTexture("spr_titlegraphic_2.png");
}

void TitleGraphic::Update(float _DeltaTime)
{
}

void TitleGraphic::End()
{
}
