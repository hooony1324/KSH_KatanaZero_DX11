#pragma once

class GlobalValueManager
{
public:
	// 콜리전 맵 정보
	static GameEngineTextureRenderer* ColMap;

	static float4 PlayerPos;
	


private:
	// constrcuter destructer
	GlobalValueManager() {}
	~GlobalValueManager() {}
};

// TimeScale을 구분함
enum class ACTORGROUP
{
	CAMERA,
	TIMEGROUP,
	TIMEGROUP_BULLET,
	NONE,
};

enum class COLLISIONGROUP
{
	PLAYER,
	PLAYER_ATTACK,
	ENEMY,
	ENEMY_ATTACK,
	ENEMY_AIM,
	DOOR,
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
	SLOWTRANSITON,
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


enum class TITLE_DEPTH
{
	BUTTONS = 0,
	GRAPHIC = 100,
	PLANTS = 200,
	GRASS = 300,
	FENCE = 400,
	PLATE = 500,
};

static PixelColor GREEN(0, 255, 0, 255);
static PixelColor BLUE(0, 0, 255, 255);