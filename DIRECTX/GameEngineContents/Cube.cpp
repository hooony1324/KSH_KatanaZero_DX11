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
	Renderer->GetTransform().SetLocalScale({ 100, 100, 100 });
	
}

void Cube::Update(float _DeltaTime)
{
	//TimeAngle += _DeltaTime * 20.0f;

	//GetTransform().SetLocalRotation({ TimeAngle, TimeAngle, TimeAngle });
}

void Cube::End()
{
}
