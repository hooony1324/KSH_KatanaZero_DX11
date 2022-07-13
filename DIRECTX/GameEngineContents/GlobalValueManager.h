#pragma once
#include "Obstacle.h"

enum class ACTORGROUP
{
	CAMERA,
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

private:
	// constrcuter destructer
	GlobalValueManager() {}
	~GlobalValueManager() {}
};

enum class STATE_PLAYER
{
	IDLE,
	JUMP,
	WALK_RIGHT,
	WALK_LEFT,
	RUN_RIGHT,
	RUN_LEFT,
	DOWN,
};