#include "PreCompile.h"
#include "GlobalValueManager.h"

GameEngineTextureRenderer* GlobalValueManager::ColMap = nullptr;
float4 GlobalValueManager::PlayerPos;
std::list<GameEngineCollision*> GlobalValueManager::Collision_UpStairs;
std::list<GameEngineCollision*> GlobalValueManager::Collision_DownStairs;