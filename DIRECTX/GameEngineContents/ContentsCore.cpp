#include "PreCompile.h"
#include "ContentsCore.h"

#pragma comment(lib, "GameEngineBase.lib")
#include "PlayLevel.h"

ContentsCore::ContentsCore() 
{
}

ContentsCore::~ContentsCore() 
{
}

void ContentsCore::Start()
{
	// GameEngineDebug::ConsoleOpen();

	// ���ҽ��� �ε��ϴµ�.

	// RTTI �� Ÿ�� Ÿ�� �������̼�
	CreateLevel<PlayLevel>("PlayLevel");
	ChangeLevel("PlayLevel");

	// ���������� ����
	// �� ���ӿ��� Ÿ��Ʋȭ��
	//            ��������1
	//            ��������2
}

void ContentsCore::Update(float _DeltaTime)
{
}

void ContentsCore::End()
{
}
