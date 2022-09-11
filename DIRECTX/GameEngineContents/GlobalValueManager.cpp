#include "PreCompile.h"
#include "GlobalValueManager.h"
#include "CharacterActor.h"

GameEngineTextureRenderer* GlobalValueManager::ColMap = nullptr;
float4 GlobalValueManager::PlayerPos;
CharacterActor* GlobalValueManager::Player = nullptr;
std::list<GameEngineCollision*> GlobalValueManager::Collision_UpStairs;
std::list<GameEngineCollision*> GlobalValueManager::Collision_DownStairs;

std::string GlobalValueManager::PlayerWallState;
std::string GlobalValueManager::PlayerState;

void GlobalValueManager::ClearStairs()
{
	for (GameEngineCollision* Collision : Collision_UpStairs)
	{
		Collision->Death();
	}
	
	for (GameEngineCollision* Collision : Collision_DownStairs)
	{
		Collision->Death();
	}
}

GlobalValueManager::~GlobalValueManager()
{
	ColMap = nullptr;
	Player = nullptr;
}

int GlobalValueManager::SlowEnergy = 11;