#include "PreCompile.h"
#include "TestLevel.h"
#include <GameEngineCore/CoreMinimal.h>

#include "TestActor.h"
#include "Portal.h"

TestLevel::TestLevel() 
{
}

TestLevel::~TestLevel() 
{
}

Portal* portal;
void TestLevel::Start()
{
	TestActor* actor = CreateActor<TestActor>();

	portal = CreateActor<Portal>();
	portal->GetTransform().SetWorldMove(float4::RIGHT * 100);
	portal->Off();
}


void TestLevel::LevelStartEvent()
{
	portal->On();
}

void TestLevel::Update(float _DeltaTime)
{

	if (true == GameEngineInput::GetInst()->IsPress("Shift"))
	{
		portal->On();
	}

}


void TestLevel::End()
{
}
