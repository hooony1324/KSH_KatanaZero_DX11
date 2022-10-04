#include "PreCompile.h"
#include "Room.h"
#include "CharacterActor.h"
#include "EnemyActor.h"

int Room::CurRoomIndex = 0;

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
		if (false == Enemy->IsDead())
		{
			return false;
		}
	}

	return true;
}

void Room::EmptyRoomInit()
{
	BackBackGround = CreateComponent<GameEngineTextureRenderer>();
	BackBackGround->SetTexture("None.png");
	BackBackGround->GetTransform().SetWorldScale({ 3000, 2000, 1 });
	
	
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


	BackBackGround->GetTransform().SetWorldPosition(CamClamp_Center);
	BackBackGround->GetTransform().SetWorldMove({ 0, 0, GetDepth(ACTOR_DEPTH::BACKGROUND_0) + 10});
}

void Room::SetCameraClampArea(float4& _LeftTop, float4& _RightBottom)
{
	_LeftTop = CamClamp_LeftTop;
	_RightBottom = CamClamp_RightBottom;
}

void Room::PlayerSpawn(CharacterActor* _Player)
{
	PlayerActor = _Player;
	PlayerActor->Respawn(); // 소환시 처음 상태로
	PlayerActor->GetMoveVector() = float4::ZERO;
	PlayerActor->SetCharacterIdle();
	PlayerActor->GetTransform().SetWorldPosition(PlayerSpawnPos);
}
