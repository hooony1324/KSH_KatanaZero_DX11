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

float TimeAngle = 0.0f;

void Cube::Update(float _DeltaTime)
{
	TimeAngle += _DeltaTime * 20.0f;

	GetTransform().SetRotationDegree({ TimeAngle, TimeAngle, TimeAngle });
}

void Cube::End()
{
}
