#include "Cube.h"
#include <GameEngineBase/GameEngineWindow.h>
#include <GameEngineCore/GameEngineRenderer.h>

Cube::Cube() 
{
}

Cube::~Cube() 
{
}

void Cube::Start()
{
	GameEngineRenderer* Renderer = CreateComponent<GameEngineRenderer>();
	GetTransform().SetScale({ 100, 100, 100 });
	GetTransform().SetPosition(GameEngineWindow::GetScale().Half());
}

void Cube::Update(float _DeltaTime)
{
}

void Cube::End()
{
}
