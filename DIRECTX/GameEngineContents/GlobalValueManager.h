#pragma once
#include "Obstacle.h"

enum class ACTORGROUP
{
	CAMERA,
	MAP,
	PLAYER,
	ENEMY,
	PROJECTILE,
	NONE,
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

