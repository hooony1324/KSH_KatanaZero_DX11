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
	Cam->GetTransform().SetWorldPosition({ 0, 0, static_cast<float>(DEPTH_ACTOR::CAMERA)});

	// Rooms
	Room1 = CreateActor<Room_Factory1>(ACTORGROUP::MAP);
	Room2 = CreateActor<Room_Factory2>(ACTORGROUP::MAP);
	Room3 = CreateActor<Room_Boss>(ACTORGROUP::MAP);
	RoomChange(Room1);
	
	// Player ��ȯ
	Player = CreateActor<PlayerZero>(ACTORGROUP::PLAYER);
	Player->GetTransform().SetWorldPosition({ 0, 0, static_cast<float>(DEPTH_ACTOR::PLAYER) });

	// Cursor
	Cursor* Mouse = CreateActor<Cursor>();
	Mouse->GetTransform().SetWorldPosition({ 0, 0, static_cast<float>(DEPTH_ACTOR::CURSOR) });
}

void PlayLevel::Update(float _DeltaTime)
{
	//// �� ���� ����׿�
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

	// ����ī�޶�
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
	// ������ ��
	if (nullptr == CurRoom)
	{
		CurRoom = _Room;
		CurRoom->OnEvent();
		CurRoom->GetCameraClampArea(CamClamp_LeftTop, CamClamp_RightBottom);
		return;
	}
	
	// �� ��ȯ
	CurRoom->OffEvent();
	CurRoom = _Room;
	CurRoom->OnEvent();
	CurRoom->GetCameraClampArea(CamClamp_LeftTop, CamClamp_RightBottom);
}

void PlayLevel::CameraFollow()
{
	//float4 PlayerPos = Player->GetTransform().GetWorldPosition();
	//float4 CamPos = Cam->GetTransform().GetWorldPosition();
	//
	//if (CamPos.x <= CamClamp_LeftTop.x || CamPos.x >= CamClamp_RightBottom.x
	//	|| CamPos.y <= CamClamp_LeftTop.y || CamPos.y >= CamClamp_RightBottom.y)
	//{
	//	float4 MoveVec = float4{ 0, 0 } - CamPos;
	//	MoveVec.z = 0;
	//	Cam->GetTransform().SetWorldMove(MoveVec);
	//	return;
	//}

	//Cam->GetTransform().SetWorldPosition(PlayerPos);

	//

	
}
