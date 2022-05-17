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
	// ���ҽ��� �ε��ϴµ�.

	// RTTI �� Ÿ�� Ÿ�� �������̼�
	CreateLevel<TestLevel>("Test");

	ChangeLevel("Test");


	// ���������� ����
	// �� ���ӿ��� Ÿ��Ʋȭ��
	//            ��������1
	//            ��������2
}

void TestContentsCore::UserUpdate(float _DeltaTime)
{
}

void TestContentsCore::UserEnd()
{
	int a = 0;
}