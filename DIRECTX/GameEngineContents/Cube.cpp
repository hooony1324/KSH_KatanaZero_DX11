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
	auto val = GetTransform().GetLocalScale();

	GetTransform().SetLocalPosition(GameEngineWindow::GetScale().Half());


	GameEngineRenderer* Renderer = CreateComponent<GameEngineRenderer>();
	Renderer->GetTransform().SetLocalPosition({ 100, 0, 0 });
	Renderer->GetTransform().SetLocalScale({ 100, 100, 100 });
	
}

float TimeAngle = 0.0f;

void Cube::Update(float _DeltaTime)
{
	TimeAngle += _DeltaTime * 20.0f;

	GetTransform().SetLocalRotation({ TimeAngle, TimeAngle, TimeAngle });
}

void Cube::End()
{
}
