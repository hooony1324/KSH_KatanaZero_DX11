#include "TestContentsCore.h"

#pragma comment(lib, "GameEngineBase.lib")

TestContentsCore::TestContentsCore()
	: GameEngineCore()
{
}

TestContentsCore::~TestContentsCore()
{
}

void TestContentsCore::UserGameStart()
{
	// 리소스를 로드하는데.

	// 게임컨텐츠 정의
	// 이 게임에는 타이틀화면
	//            스테이지1
	//            스테이지2
}

void TestContentsCore::UserGameUpdate()
{
	// 서버가 종료되었어.
	// 무조건 경고 메세지창을 띄워줘야한다.
}

void TestContentsCore::UserGameEnd()
{
	int a = 0;
}