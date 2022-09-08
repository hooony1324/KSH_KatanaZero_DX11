#pragma once

class GlobalValueManager
{
public:
	// 콜리전 맵 정보
	static GameEngineTextureRenderer* ColMap;

	static float4 PlayerPos;
	static class CharacterActor* Player;
	
	// 현재 맵의 계단 정보
	static std::list<GameEngineCollision*> Collision_UpStairs;
	static std::list<GameEngineCollision*> Collision_DownStairs;

	static void ClearStairs();

	// 슬로우
	static int SlowEnergy;

private:
	// constrcuter destructer
	GlobalValueManager() {}
	~GlobalValueManager();
};

// TimeScale을 구분함
enum class ACTORGROUP
{
	CAMERA,
	TIMEGROUP,			// 플레이어 등
	TIMEGROUP_ENEMY,    // 총알이 반사될 때 적에게만 날라가도록
	TIMEGROUP_BULLET,	// 룸 재시작 시 총알만 없애도록
	TIMEGROUP_FAN,
	NONE,
};

enum class COLLISIONGROUP
{
	PLAYER,
	PLAYER_ATTACK,
	PLAYER_BULLET,
	ENEMY,
	ENEMY_CAHSESENSOR,
	ENEMY_ATTACK,
	DOOR,
	STAIR,
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
	BACKGROUND_COL,
	BOSSPORTAL,
	BOSSFOREGROUND,
	BOSSFLOOR,
	BOSSMIDGROUND,
	BOSS,

	// BACKGROUND
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

struct CustomRenderOption
{
	float DeltaTime;
	float SumDeltaTime;
	int IsMask;
	int OnOff;
	int Option01;
	int Option02;
	int Option03;
	int Option04;
	int Option05;
	int Option06;
	int Option07;
	int Option08;
	int Option09;
	int Option10;
	int Option11;
	int Option12;
	int Option13;
	int Option14;
	int Option15;
	int Option16;
	int Option17;
	int Option18;
	int Option19;
	int Option20;
};