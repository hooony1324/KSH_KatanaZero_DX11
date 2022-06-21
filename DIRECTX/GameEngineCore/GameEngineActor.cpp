#include "PreCompile.h"
#include "GameEngineActor.h"
#include "GameEngineComponent.h"
#include "GameEngineTransformComponent.h"

GameEngineActor::GameEngineActor() 
	:ParentLevel(nullptr)
{

}

GameEngineActor::~GameEngineActor() 
{
}

void GameEngineActor::Start() {}
void GameEngineActor::Update(float _DeltaTime) {}
void GameEngineActor::End() {}

void GameEngineActor::ComponentUpdate(float _ScaleDeltaTime, float _DeltaTime)
{
	for (GameEngineUpdateObject* Com : Childs)
	{
		Com->AddAccTime(_DeltaTime);
		Com->Update(_ScaleDeltaTime);
	}
}
void GameEngineActor::DetachObject() 
{
	GameEngineUpdateObject::DetachObject();

	GetTransform().DetachTransform();
}

void GameEngineActor::SetParent(GameEngineUpdateObject* _Object) 
{
	GameEngineUpdateObject::SetParent(_Object);

	{
		GameEngineTransformBase* Actor = nullptr;
		if (Actor = dynamic_cast<GameEngineTransformBase*>(_Object))
		{
			GetTransform().SetParentTransform(Actor->GetTransform());
			return;
		}
	}

	MsgBoxAssert("트랜스폼이 없는 컴포넌트에 트랜스폼이 있는 부모를 붙이려고 했습니다.");
}

void GameEngineActor::ReleaseObject(std::list<GameEngineUpdateObject*>& _RelaseList) 
{
	GameEngineUpdateObject::ReleaseObject(_RelaseList);

	for (GameEngineUpdateObject* Component : _RelaseList)
	{
		GameEngineComponent* ConvertPtr = dynamic_cast<GameEngineComponent*>(Component);

		if (nullptr == ConvertPtr)
		{
			continue;
		}
	}
}