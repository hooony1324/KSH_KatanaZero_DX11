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

	static std::vector<std::string> PlayerAnimations;


private:
	// constrcuter destructer
	GlobalValueManager() {}
	~GlobalValueManager() {}
};

