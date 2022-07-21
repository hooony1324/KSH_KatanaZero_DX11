#include "PreCompile.h"
#include "PlayLevel.h"
#include <GameEngineCore/CoreMinimal.h>
#include <GameEngineCore/GameEngineCameraActor.h>

#include "PlayerZero.h"
#include "Room_Factory1.h"
#include "Room_Factory2.h"
#include "Room_Boss.h"

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
	//Room2 = CreateActor<Room_Factory2>(ACTORGROUP::MAP);
	//Room3 = CreateActor<Room_Boss>(ACTORGROUP::MAP);
	//RoomChange(Room1);
	
	PlayerZero* Player = CreateActor<PlayerZero>(ACTORGROUP::PLAYER);
}

void PlayLevel::Update(float _DeltaTime)
{
	//// 룸 변경 디버그용
	//{
	//	if (true == GameEngineInput::GetInst()->IsDown("Numpad4"))
	//	{
	//		RoomChange(Room1);
	//	}

	//	if (true == GameEngineInput::GetInst()->IsDown("Numpad5"))
	//	{
	//		RoomChange(Room2);
	//	}

	//	if (true == GameEngineInput::GetInst()->IsDown("Numpad6"))
	//	{
	//		RoomChange(Room3);
	//	}	
	//}

	// 프리카메라
	if (true == GameEngineInput::GetInst()->IsDown("Numpad0"))
	{
		GetMainCamera()->SetProjectionMode(CAMERAPROJECTIONMODE::PersPective);
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
		CurRoom->RoomOn();
	}
	
	CurRoom->RoomOff();
	CurRoom = _Room;
	CurRoom->RoomOn();
}
