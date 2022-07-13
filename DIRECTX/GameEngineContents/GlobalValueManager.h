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
	LAND,
	ROLL,
	RUN,
	SLASH,
	WALLKICK,
	WALLSLIDE,
	DIE,
};