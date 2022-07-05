#include "PreCompile.h"
#include "ContentsCore.h"

#pragma comment(lib, "GameEngineBase.lib")
#include "PlayLevel.h"
#include "TestLevel.h"

ContentsCore::ContentsCore() 
{
}

ContentsCore::~ContentsCore() 
{
}

void ContentsCore::Start()
{
	// GameEngineDebug::ConsoleOpen();

	// 리소스를 로드하는데.

	// RTTI 런 타임 타입 인포메이션
	CreateLevel<PlayLevel>("PlayLevel");
	CreateLevel<TestLevel>("TestLevel");
	ChangeLevel("TestLevel");

	// 게임컨텐츠 정의
	// 이 게임에는 타이틀화면
	//            스테이지1
	//            스테이지2
}

void ContentsCore::Update(float _DeltaTime)
{
}

void ContentsCore::End()
{
}
