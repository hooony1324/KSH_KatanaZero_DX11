#include "TestLevel.h"

#include "Cube.h"

TestLevel::TestLevel() 
{
}

TestLevel::~TestLevel() 
{
}

void TestLevel::Start()
{
	CreateActor<Cube>(0);
}

void TestLevel::Update(float _DeltaTime)
{
}

void TestLevel::End()
{
}
