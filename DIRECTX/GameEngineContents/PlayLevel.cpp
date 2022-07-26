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

	// Rooms
	Room1 = CreateActor<Room_Factory1>(ACTORGROUP::MAP);
	Room2 = CreateActor<Room_Factory2>(ACTORGROUP::MAP);
	Room3 = CreateActor<Room_Boss>(ACTORGROUP::MAP);


	// Cursor
	Cursor* Mouse = CreateActor<Cursor>();
}

void PlayLevel::OnEvent()
{
	RoomChange(Room1);

	// Player
	Player = CreateActor<PlayerZero>(ACTORGROUP::PLAYER);
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

	// 충돌 맵 OnOff
	if (true == GameEngineInput::GetInst()->IsDown("M"))
	{
		GlobalValueManager::ColMap->OnOffSwitch();
	}
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
	}
	else
	{
		// 방 교환
		CurRoom->Clear();
		CurRoom = _Room;
	}

	CurRoom->Setting();
	CurRoom->GetCameraClampArea(CamClamp_LeftTop, CamClamp_RightBottom);

}

void PlayLevel::CameraFollow()
{
	if (true == Cam->IsFreeCameraMode())
	{
		return;
	}
	float4 PlayerPos = Player->GetTransform().GetWorldPosition();
	float4 CamPos = Cam->GetTransform().GetWorldPosition();

	//if (PlayerPos.x >= CamClamp_LeftTop.x && PlayerPos.x <= CamClamp_RightBottom.x &&
	//	PlayerPos.y <= CamClamp_LeftTop.y && PlayerPos.y >= CamClamp_RightBottom.y)
	//{
	//	Cam->GetTransform().SetWorldPosition(PlayerPos);
	//}


	Cam->GetTransform().SetWorldPosition({ PlayerPos.x, PlayerPos.y, -100});

}
