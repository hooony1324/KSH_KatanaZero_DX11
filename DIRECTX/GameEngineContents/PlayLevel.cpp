#include "PreCompile.h"
#include "PlayLevel.h"
#include <GameEngineCore/CoreMinimal.h>
#include <GameEngineCore/GameEngineCameraActor.h>

#include "PlayerZero.h"
#include "Room_Factory1.h"
#include "Room_Factory2.h"
#include "Room_Boss.h"
#include "Cursor.h"

PlayLevel::PlayLevel() 
	: CurRoom(nullptr)
{
}

PlayLevel::~PlayLevel() 
{
}

void PlayLevel::Start()
{
	Cam = CreateActor<GameEngineCameraActor>(ACTORGROUP::CAMERA);
	Cam->GetCameraComponent()->SetProjectionMode(CAMERAPROJECTIONMODE::Orthographic);
	Cam->GetTransform().SetWorldPosition({ 0, 0, GetDepth(ACTOR_DEPTH::CAMERA) });

	// Player 소환
	Player = CreateActor<PlayerZero>(ACTORGROUP::PLAYER);

	// Rooms
	Room1 = CreateActor<Room_Factory1>(ACTORGROUP::MAP);
	Room2 = CreateActor<Room_Factory2>(ACTORGROUP::MAP);
	Room3 = CreateActor<Room_Boss>(ACTORGROUP::MAP);
	RoomChange(Room1);
	
	// Cursor
	Cursor* Mouse = CreateActor<Cursor>();
}

void PlayLevel::Update(float _DeltaTime)
{
	//// 룸 변경 디버그용
	{
		if (true == GameEngineInput::GetInst()->IsDown("Numpad4"))
		{
			RoomChange(Room1);
		}

		if (true == GameEngineInput::GetInst()->IsDown("Numpad5"))
		{
			RoomChange(Room2);
		}

		if (true == GameEngineInput::GetInst()->IsDown("Numpad6"))
		{
			RoomChange(Room3);
		}	
	}

	// 프리카메라
	if (true == GameEngineInput::GetInst()->IsDown("Numpad0"))
	{
		GetMainCameraActor()->FreeCameraModeOnOff();
	}


	CameraFollow();
}

void PlayLevel::End()
{
}

void PlayLevel::RoomChange(Room* _Room)
{
	// 최초의 방
	if (nullptr == CurRoom)
	{
		CurRoom = _Room;
		CurRoom->OnEvent();
		CurRoom->GetCameraClampArea(CamClamp_LeftTop, CamClamp_RightBottom);
		return;
	}
	
	// 방 교환
	CurRoom->OffEvent();
	CurRoom = _Room;
	CurRoom->OnEvent();
	CurRoom->GetCameraClampArea(CamClamp_LeftTop, CamClamp_RightBottom);
}

void PlayLevel::CameraFollow()
{
	float4 PlayerPos = Player->GetTransform().GetWorldPosition();
	float4 CamPos = Cam->GetTransform().GetWorldPosition();

	if (PlayerPos.x >= CamClamp_LeftTop.x && PlayerPos.x <= CamClamp_RightBottom.x &&
		PlayerPos.y <= CamClamp_LeftTop.y && PlayerPos.y >= CamClamp_RightBottom.y)
	{
		Cam->GetTransform().SetWorldPosition(PlayerPos);
	}




}
