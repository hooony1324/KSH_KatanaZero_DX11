#include "Obstacle.h"
#include <GameEngineCore/CoreMinimal.h>

Obstacle::Obstacle() 
{
}

Obstacle::~Obstacle() 
{
}

void Obstacle::Start()
{
	GameEngineRenderer* renderer = CreateComponent<GameEngineRenderer>();
}

void Obstacle::Update(float _DeltaTime)
{
}

void Obstacle::End()
{
}
