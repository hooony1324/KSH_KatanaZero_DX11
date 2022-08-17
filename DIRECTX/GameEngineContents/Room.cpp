#include "PreCompile.h"
#include "Room.h"
#include "CharacterActor.h"
#include "EnemyActor.h"

Room::Room() 
{
}

Room::~Room() 
{
}

bool Room::IsEnemyAllDead()
{
	// 적이 하나도 없는 방
	if (Enemies.size() == 0)
	{
		return true;
	}


	for (EnemyActor* Enemy : Enemies)
	{
		if (true == Enemy->IsUpdate())
		{
			return false;
		}
	}

	return true;
}

void Room::EmptyRoomInit()
{
	Background = CreateComponent<GameEngineTextureRenderer>();
	Background_ColMap = CreateComponent<GameEngineTextureRenderer>();

}

void Room::InitCameraClampArea()
{
	float4 BackgroundScale = Background->GetCurTexture()->GetScale();
	float4 ClampScale = BackgroundScale / 2 - float4{ 640, 360 };
	CamClamp_Center = { BackgroundScale.x / 2, -BackgroundScale.y / 2 };
	CamClamp_LeftTop = { CamClamp_Center.x - ClampScale.x, CamClamp_Center.y + ClampScale.y };
	CamClamp_RightBottom = { CamClamp_Center.x + ClampScale.x, CamClamp_Center.y - ClampScale.y };
}

void Room::SetCameraClampArea(float4& _LeftTop, float4& _RightBottom)
{
	_LeftTop = CamClamp_LeftTop;
	_RightBottom = CamClamp_RightBottom;
}

void Room::PlayerSpawn(CharacterActor* _Player)
{
	_Player->Respawn(); // 소환시 처음 상태로
	_Player->GetTransform().SetWorldPosition(PlayerSpawnPos);
}
