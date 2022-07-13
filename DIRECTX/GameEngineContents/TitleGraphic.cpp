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
	Renderer = CreateComponent<GameEngineDefaultRenderer>();
	Renderer->GetTransform().SetLocalScale({ 639, 360, 0 });
	Renderer->SetPipeLine("Texture");
}

void TitleGraphic::Update(float _DeltaTime)
{
}

void TitleGraphic::End()
{
}
