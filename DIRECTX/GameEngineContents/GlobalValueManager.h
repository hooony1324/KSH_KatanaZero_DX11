#pragma once
#include "Obstacle.h"

enum class ACTORGROUP
{
	CAMERA,
	PLAYER,
	MONSTER,
	OBSTACLE,
};

// ���� :
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

