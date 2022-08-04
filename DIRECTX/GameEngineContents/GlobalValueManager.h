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

enum class COLLISIONGROUP
{
	PLAYER,
	PLAYER_ATTACK,
	ENEMY,
	ENEMY_ATTACK,

};

enum class ACTOR_DEPTH
{
	CAMERA = -1,

	// SCREEN EFFECT
	LOGO = 0,
	TRANSITION,

	// ACTOR
	CURSOR,
	UI,
	FX,
	FOREGROUND,
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

	COLLISION, 
};

// DEPTH::GetDepth(ACTOR_DEPTH::PLAYER)
namespace Depth
{
	static float GetDepth(ACTOR_DEPTH _Type)
	{
		return static_cast<float>(_Type) * 100.0f;
	}
};


class GlobalValueManager
{
public:
	// 콜리전 맵 정보
	static GameEngineTextureRenderer* ColMap;

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




enum class TITLE_DEPTH
{
	BUTTONS = 0,
	GRAPHIC = 100,
	PLANTS = 200,
	GRASS = 300,
	FENCE = 400,
	PLATE = 500,
};