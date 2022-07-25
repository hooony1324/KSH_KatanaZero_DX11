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
	CAMERA = -1000,
	TRANSITION = -500,

	// ACTOR
	CURSOR = -400,
	UI = -300,
	FX = -200,
	PLAYER = 0,
	ENEMY = 100,
	DOOR = 200,
	BOSS = 300,

	// BACKGROUND
	BACKGROUND_4 = 400,
	BACKGROUND_3 = 500,
	BACKGROUND_2 = 600,
	BACKGROUND_1 = 700,
	BACKGROUND_0 = 800,

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

