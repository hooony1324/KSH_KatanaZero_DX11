#include "TestContentsCore.h"

#pragma comment(lib, "GameEngineBase.lib")

#include "GameEngineContents/TestLevel.h"

TestContentsCore::TestContentsCore()
	: GameEngineCore()
{
}

TestContentsCore::~TestContentsCore()
{
}

void TestContentsCore::UserStart()
{
	// 리소스를 로드하는데.

	// RTTI 런 타임 타입 인포메이션
	CreateLevel<TestLevel>("Test");

	ChangeLevel("Test");


	// 게임컨텐츠 정의
	// 이 게임에는 타이틀화면
	//            스테이지1
	//            스테이지2
}

void TestContentsCore::UserUpdate(float _DeltaTime)
{
}

void TestContentsCore::UserEnd()
{
	int a = 0;
}