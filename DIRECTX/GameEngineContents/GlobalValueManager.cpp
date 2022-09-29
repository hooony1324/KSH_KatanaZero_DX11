#include "PreCompile.h"
#include "GlobalValueManager.h"
#include "CharacterActor.h"

GameEngineTextureRenderer* GlobalValueManager::ColMap = nullptr;
float4 GlobalValueManager::PlayerPos;
CharacterActor* GlobalValueManager::Player = nullptr;

std::list<Stair*> GlobalValueManager::Stairs;
std::string GlobalValueManager::PlayerWallState;
std::string GlobalValueManager::PlayerState;



void GlobalValueManager::ClearStairs()
{
	for (Stair* Ptr : Stairs)
	{
		Ptr->Off();
		Ptr = nullptr;
	}
	Stairs.clear();
}

GlobalValueManager::~GlobalValueManager()
{
	ColMap = nullptr;
	Player = nullptr;
}
int GlobalValueManager::SlowEnergyMax = 11;
int GlobalValueManager::SlowEnergy = SlowEnergyMax;