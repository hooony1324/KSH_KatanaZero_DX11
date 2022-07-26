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

enum class ACTOR_DEPTH
{
	CAMERA,

	// SCREEN EFFECT
	TRANSITION,

	// ACTOR
	CURSOR,
	UI,
	FX,
	PLAYER,
	ENEMY,
	DOOR,
	BOSS,

	// BACKGROUND
	BACKGROUND_COL,
	BACKGROUND_4,
	BACKGROUND_3,
	BACKGROUND_2,
	BACKGROUND_1,
	BACKGROUND_0,

};

// DEPTH::GetDepth(ACTOR_DEPTH::PLAYER)
namespace Depth
{
	static float GetDepth(ACTOR_DEPTH _Type)
	{
		return static_cast<float>(_Type) * 100.0f;
	}
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

