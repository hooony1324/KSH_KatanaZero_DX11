#include "PreCompile.h"
#include "TitleGrass.h"
#include <GameEngineCore/CoreMinimal.h>

TitleGrass::TitleGrass() 
{
}

TitleGrass::~TitleGrass() 
{
}

void TitleGrass::Start()
{
	Renderer = CreateComponent<GameEngineDefaultRenderer>();
	Renderer->GetTransform().SetLocalScale({ 640, 120, 0 });
	Renderer->GetTransform().SetLocalPosition({ 0, 320, 0 });
	Renderer->SetPipeLine("Texture");
}

void TitleGrass::Update(float _DeltaTime)
{
}

void TitleGrass::End()
{
}
