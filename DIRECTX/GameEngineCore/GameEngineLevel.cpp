#include "GameEngineLevel.h"
#include "GameEngineActor.h"

GameEngineLevel::GameEngineLevel()
{
}

GameEngineLevel::~GameEngineLevel()
{
}

void GameEngineLevel::ActorUpdate(float _DelataTime)
{
	for (const std::pair<int, std::list<GameEngineActor*>>& Group : AllActors)
	{
		// Group.first;

		float ScaleTime = GameEngineTime::GetInst()->GetDeltaTime(Group.first);

		for (GameEngineActor* const Data : Group.second)
		{
			Data->AddAccTime(_DelataTime);
		}
	}
}