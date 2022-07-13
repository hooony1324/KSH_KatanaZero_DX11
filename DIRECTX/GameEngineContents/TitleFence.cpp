#include "PreCompile.h"
#include "TitleFence.h"
#include <GameEngineCore/CoreMinimal.h>

TitleFence::TitleFence() 
{
}

TitleFence::~TitleFence() 
{
}

void TitleFence::Start()
{
	Renderer = CreateComponent<GameEngineDefaultRenderer>();
	Renderer->GetTransform().SetLocalScale({ 640, 720, 0 });
	Renderer->SetPipeLine("Texture");
}

void TitleFence::Update(float _DeltaTime)
{
}

void TitleFence::End()
{
}
