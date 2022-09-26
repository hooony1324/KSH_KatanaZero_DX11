#pragma once
#include "Stair.h"

class GlobalValueManager
{
public:
	// �ݸ��� �� ����
	static GameEngineTextureRenderer* ColMap;

	static float4 PlayerPos;
	static class CharacterActor* Player;
	static std::string PlayerWallState;
	static std::string PlayerState;
	
	// ���� ���� ��� ����
	static std::list<Stair*> Stairs;
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
	TIMEGROUP,			// �÷��̾� ��
	TIMEGROUP_ENEMY,    // �Ѿ��� �ݻ�� �� �����Ը� ���󰡵���
	TIMEGROUP_PARTICLE,	// �� ����� �� �Ѿ˸� ���ֵ���
	TIMEGROUP_FAN,
	BLOOD,
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
	CURSOR,
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
	int Version;
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