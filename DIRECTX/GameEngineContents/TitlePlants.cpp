#include "PreCompile.h"
#include "TitlePlants.h"
#include <GameEngineCore/CoreMinimal.h>

TitlePlants::TitlePlants() 
{
}

TitlePlants::~TitlePlants() 
{
}

void TitlePlants::Start()
{
	Renderer = CreateComponent<GameEngineDefaultRenderer>();
	Renderer->GetTransform().SetLocalScale({ 640, 360, 0 });
	Renderer->GetTransform().SetLocalPosition({ 0, 320, 0 });
	Renderer->SetPipeLine("Texture");
}

void TitlePlants::Update(float _DeltaTime)
{
}

void TitlePlants::End()
{
}
