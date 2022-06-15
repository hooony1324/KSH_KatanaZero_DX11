#pragma once
#include "Obstacle.h"

enum class ACTORGROUP
{
	PLAYER,
	MONSTER,
};

// Ό³Έν :
class GlobalValueManager
{
public:

	class Actors
	{
	public:
		static Obstacle* testObstacle;
	};

protected:

private:
	// constrcuter destructer
	GlobalValueManager();
	~GlobalValueManager();

	// delete Function
	GlobalValueManager(const GlobalValueManager& _Other) = delete;
	GlobalValueManager(GlobalValueManager&& _Other) noexcept = delete;
	GlobalValueManager& operator=(const GlobalValueManager& _Other) = delete;
	GlobalValueManager& operator=(GlobalValueManager&& _Other) noexcept = delete;
};

