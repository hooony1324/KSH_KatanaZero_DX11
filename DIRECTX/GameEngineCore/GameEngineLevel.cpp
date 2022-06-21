#include "PreCompile.h"
#include "GameEngineLevel.h"
#include "GameEngineActor.h"
#include "GameEngineRenderer.h"
#include "GameEngineCamera.h"

GameEngineLevel::GameEngineLevel() 
	: MainCamera(nullptr)
	, UIMainCamera(nullptr)
{
}

GameEngineLevel::~GameEngineLevel() 
{
	for (const std::pair<int, std::list<GameEngineActor*>>& Group : AllActors)
	{
		for (GameEngineActor* Actor : Group.second)
		{
			if (nullptr == Actor)
			{
				continue;
			}

			Actor->DeleteChild();
		}
	}
}


void GameEngineLevel::ActorUpdate(float _DelataTime)
{
	for (const std::pair<int, std::list<GameEngineActor*>>& Group : AllActors)
	{
		float ScaleTime = GameEngineTime::GetInst()->GetDeltaTime(Group.first);
		for (GameEngineActor* const Actor : Group.second)
		{
			Actor->AddAccTime(_DelataTime);
			Actor->ReleaseUpdate(_DelataTime);
			Actor->ComponentUpdate(ScaleTime, _DelataTime);
			Actor->Update(ScaleTime);
		}
	}

}

void GameEngineLevel::PushRenderer(GameEngineRenderer* _Renderer)
{
	MainCamera->PushRenderer(_Renderer);
}

void GameEngineLevel::PushCamera(GameEngineCamera* _Camera)
{
	MainCamera = _Camera;
}

GameEngineTransform& GameEngineLevel::GetMainCameraActorTransform()
{
	return MainCamera->GetActor()->GetTransform();
}

void GameEngineLevel::Render(float _DelataTime)
{
	MainCamera->Render(_DelataTime);
}

void GameEngineLevel::Release(float _DelataTime)
{
	for (GameEngineUpdateObject* Object : DeleteObject)
	{
		Object->DeleteChild();
	}

	DeleteObject.clear();

	MainCamera->Release(_DelataTime);

	std::map<int, std::list<GameEngineActor*>>::iterator StartGroupIter = AllActors.begin();
	std::map<int, std::list<GameEngineActor*>>::iterator EndGroupIter = AllActors.end();

	for (; StartGroupIter != EndGroupIter; ++StartGroupIter)
	{
		std::list<GameEngineActor*>& Group = StartGroupIter->second;

		std::list<GameEngineActor*>::iterator GroupStart = Group.begin();
		std::list<GameEngineActor*>::iterator GroupEnd = Group.end();

		for (; GroupStart != GroupEnd; )
		{
			(*GroupStart)->ReleaseObject(DeleteObject);

			if (true == (*GroupStart)->IsDeath())
			{
				// DeleteObject.push_back((*GroupStart));
				GroupStart = Group.erase(GroupStart);
			}
			else 
			{
				++GroupStart;
			}
			
		}
	}

}

void GameEngineLevel::LevelUpdate(float _DeltaTime)
{
	AddAccTime(_DeltaTime);
	Update(_DeltaTime);
	ActorUpdate(_DeltaTime);
	Render(_DeltaTime);
	Release(_DeltaTime);
}