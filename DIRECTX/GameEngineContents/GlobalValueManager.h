#pragma once

enum class ACTORGROUP
{
	CAMERA,
	MAP,
	PLAYER,
	ENEMY,
	PROJECTILE,
	NONE,
};

enum class DEPTH_ACTOR
{
	CAMERA,
	TRANSITION,

	// ACTOR
	CURSOR,
	UI,
	FX,
	PLAYER,
	ENEMY,
	BOSS,

	// BACKGROUND
	BACKGROUND_4,
	BACKGROUND_3,
	BACKGROUND_2,
	BACKGROUND_1,
	BACKGROUND_0,

};


// Ό³Έν :
class GlobalValueManager
{
public:

	class Actors
	{
	public:
		//static Obstacle* testObstacle;

	private:

		Actors() {}
		~Actors() {}
	};


private:
	// constrcuter destructer
	GlobalValueManager() {}
	~GlobalValueManager() {}
};

