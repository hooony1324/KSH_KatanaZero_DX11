#include "PreCompile.h"
#include "GlobalValueManager.h"

GameEngineTextureRenderer* GlobalValueManager::ColMap = nullptr;
float4 GlobalValueManager::PlayerPos;
std::list<GameEngineCollision*> GlobalValueManager::Collision_UpStairs;
std::list<GameEngineCollision*> GlobalValueManager::Collision_DownStairs;

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

int GlobalValueManager::SlowEnergy = 11;