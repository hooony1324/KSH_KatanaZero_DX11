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

enum class RENDER_ORDER
{
	BACKGROUND_0,
	BACKGROUND_1,
	BACKGROUND_2,
	BACKGROUND_3,
	BACKGROUND_4,

	BOSS,
	ENEMY,
	PLAYER,


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

