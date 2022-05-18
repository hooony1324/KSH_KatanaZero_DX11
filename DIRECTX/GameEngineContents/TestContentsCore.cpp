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

void TestContentsCore::Start()
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

void TestContentsCore::Update(float _DeltaTime)
{
}

void TestContentsCore::End()
{
	int a = 0;
}