#include "PreCompile.h"
#include "GameEngineInstancing.h"

// 최소 2개이상이 모여야 인스턴싱을 시작하겠다.
int GameEngineInstancing::MinInstancingCount = 1;

// 일단 시작했다면 100개는 할수 있다는 가정하에 이걸 하겠다.
int GameEngineInstancing::StartInstancingCount = 100;

GameEngineInstancing::GameEngineInstancing() 
	: Buffer(nullptr)
	, Count(0)
	, DataInsert(0)

{
}

GameEngineInstancing::~GameEngineInstancing() 
{
}

