#include "ChildCube.h"
#include <GameEngineCore/CoreMinimal.h>

ChildCube::ChildCube() 
{
}

ChildCube::~ChildCube() 
{
}

void ChildCube::Start()
{
	GameEngineRenderer* Renderer = CreateComponent<GameEngineRenderer>();
	Renderer->GetTransform().SetLocalScale({ 100, 100, 100 });
}

void ChildCube::Update(float _DeltaTime)
{
}

void ChildCube::End()
{
}
