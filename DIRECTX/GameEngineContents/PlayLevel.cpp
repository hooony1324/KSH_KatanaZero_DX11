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
	GameEngineCameraActor* Cam = CreateActor<GameEngineCameraActor>(ACTORGROUP::CAMERA);
	Cam->GetCameraComponent()->SetProjectionMode(CAMERAPROJECTIONMODE::Orthographic);
	Cam->GetTransform().SetLocalPosition({ 0, 0, 0 });

	// Rooms
	Room1 = CreateActor<Room_Factory1>(ACTORGROUP::MAP);
	Room2 = CreateActor<Room_Factory2>(ACTORGROUP::MAP);
	Room3 = CreateActor<Room_Boss>(ACTORGROUP::MAP);
	RoomChange(Room1);
	
	// Player 소환
	PlayerZero* Player = CreateActor<PlayerZero>(ACTORGROUP::PLAYER);
	Player->GetTransform().SetWorldPosition({ 0, 0, static_cast<float>(DEPTH_ACTOR::PLAYER) });

	// Cursor
	Cursor* Mouse = CreateActor<Cursor>();
	Mouse->GetTransform().SetWorldPosition({ 0, 0, static_cast<float>(DEPTH_ACTOR::CURSOR) });
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

}

void PlayLevel::End()
{
}

void PlayLevel::RoomChange(Room* _Room)
{
	if (nullptr == CurRoom)
	{
		CurRoom = _Room;
		CurRoom->OnEvent();
		return;
	}
	
	CurRoom->OffEvent();
	CurRoom = _Room;
	CurRoom->OnEvent();
}
