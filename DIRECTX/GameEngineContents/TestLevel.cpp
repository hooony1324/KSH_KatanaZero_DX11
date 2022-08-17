#include "PreCompile.h"
#include "TestLevel.h"
#include <GameEngineCore/CoreMinimal.h>

#include "TestActor.h"

TestLevel::TestLevel() 
{
}

TestLevel::~TestLevel() 
{
}

void TestLevel::Start()
{
	TestActor* actor = CreateActor<TestActor>();
}

void TestLevel::OnEvent()
{
}

void TestLevel::Update(float _DeltaTime)
{
}

void TestLevel::OffEvent()
{
}

void TestLevel::End()
{
}
