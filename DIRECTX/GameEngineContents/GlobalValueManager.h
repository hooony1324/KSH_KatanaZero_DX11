#pragma once
#include "Obstacle.h"

enum class ACTORGROUP
{
	PLAYER,
	MONSTER,
	OBSTACLE,
};

// Ό³Έν :
class GlobalValueManager
{
public:

	class Actors
	{
	public:
		static Obstacle* testObstacle;

	private:

		Actors() {}
		~Actors() {}
	};

protected:



private:
	// constrcuter destructer
	GlobalValueManager() {}
	~GlobalValueManager() {}
};

