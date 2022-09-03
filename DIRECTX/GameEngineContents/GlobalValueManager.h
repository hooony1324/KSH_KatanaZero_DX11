#pragma once

class GlobalValueManager
{
public:
	// �ݸ��� �� ����
	static GameEngineTextureRenderer* ColMap;

	static float4 PlayerPos;
	static class CharacterActor* Player;
	
	// ���� ���� ��� ����
	static std::list<GameEngineCollision*> Collision_UpStairs;
	static std::list<GameEngineCollision*> Collision_DownStairs;

	static void ClearStairs();

	// ���ο�
	static int SlowEnergy;

private:
	// constrcuter destructer
	GlobalValueManager() {}
	~GlobalValueManager();
};

// TimeScale�� ������
enum class ACTORGROUP
{
	CAMERA,
	TIMEGROUP, // �÷��̾� ��
	TIMEGROUP_ENEMY, // �Ѿ��� �ݻ�� �� �����Ը� ���󰡵���
	TIMEGROUP_BULLET,	// �� ����� �� �Ѿ˸� ���ֵ���
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