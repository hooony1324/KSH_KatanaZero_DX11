#include "PreCompile.h"
#include "PlayLevel.h"
#include <GameEngineCore/CoreMinimal.h>

#include <GameEngineCore/GEngine.h>
#include "PlayerZero.h"
#include "Room_Factory1.h"
#include "Room_Factory2.h"
#include "Room_Boss.h"
#include "Cursor.h"
#include "UIManager.h"
#include "Transition.h"
#include "PlayLevelGUI.h"

PlayLevel::PlayLevel() 
	: CurRoom(nullptr)
{
}

PlayLevel::~PlayLevel() 
{
}

void PlayLevel::Start()
{
	GetMainCamera()->SetProjectionMode(CAMERAPROJECTIONMODE::Orthographic);

	// Rooms
	Room1 = CreateActor<Room_Factory1>();
	Room2 = CreateActor<Room_Factory2>();
	//Room3 = CreateActor<Room_Boss>();
	Rooms.push_back(Room1);
	Rooms.push_back(Room2);
	RoomIter = Rooms.begin();

	// Player
	Player = CreateActor<PlayerZero>();
	Player->Off();

	// Cursor
	Cursor* Mouse = CreateActor<Cursor>();
	Mouse->GetTransform().SetWorldPosition({ 0, 0, GetDepth(ACTOR_DEPTH::CURSOR) });

	// UI
	UI = CreateActor<UIManager>();
	UI->GetTransform().SetWorldPosition({ 0, 0, GetDepth(ACTOR_DEPTH::UI) });

	// Transition
	//Effect_Transition = CreateActor<Transition>();
	//Effect_Transition->GetTransform().SetWorldPosition({ -640, 360, GetDepth(ACTOR_DEPTH::TRANSITION) });
	//Effect_Transition->Off();


	RoomStateManager.CreateStateMember("RoomChange"
		, std::bind(&PlayLevel::RoomChangeUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&PlayLevel::RoomChangeStart, this, std::placeholders::_1)
		, std::bind(&PlayLevel::RoomChangeEnd, this, std::placeholders::_1));

	RoomStateManager.CreateStateMember("RoomPlay"
		, std::bind(&PlayLevel::RoomPlayUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&PlayLevel::RoomPlayStart, this, std::placeholders::_1)
		, std::bind(&PlayLevel::RoomPlayEnd, this, std::placeholders::_1));

	RoomStateManager.CreateStateMember("RoomExit"
		, std::bind(&PlayLevel::RoomExitUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&PlayLevel::RoomExitStart, this, std::placeholders::_1));

	RoomStateManager.CreateStateMember("RoomReverse"
		, std::bind(&PlayLevel::RoomReverseUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&PlayLevel::RoomReverseStart, this, std::placeholders::_1));

	RoomStateManager.CreateStateMember("RoomRestart"
		, std::bind(&PlayLevel::RoomClickToRestartUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&PlayLevel::RoomClickToRestartStart, this, std::placeholders::_1));

	GameEngineGUI::CreateGUIWindow<PlayLevelGUI>("PlayLevelGUI", this);
}

void PlayLevel::OnEvent()
{
	RoomStateManager.ChangeState("RoomChange");
	// GUI
}

void PlayLevel::Update(float _DeltaTime)
{
	RoomStateManager.Update(_DeltaTime);

	// 룸 변경(디버그용)
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
	CameraFollow(_DeltaTime);

	// 충돌 맵 OnOff
	if (true == GameEngineInput::GetInst()->IsDown("M"))
	{
		GlobalValueManager::ColMap->OnOffSwitch();
	}



}

void PlayLevel::End()
{
}

// 디버그용 방 변경
void PlayLevel::RoomChange(Room* _Room)
{
	// 최초의 방
	if (nullptr == CurRoom)
	{
		CurRoom = Rooms.front();
		CurRoom->PlayerSpawn(Player);
	}
	else
	{
		// 방 교환
		CurRoom->Clear();
		CurRoom = _Room;
	}

	CurRoom->PlayerSpawn(Player);
	CurRoom->Setting();
	CurRoom->SetCameraClampArea(CamClamp_LeftTop, CamClamp_RightBottom);
	GetMainCameraActor()->GetTransform().SetWorldPosition(CurRoom->CamClamp_Center);
}

void PlayLevel::CameraFollow(float _DeltaTime)
{
	if (true == GetMainCameraActor()->IsFreeCameraMode())
	{
		return;
	}

	float4 CamPos = GetMainCameraActor()->GetTransform().GetWorldPosition();
	float4 NextCamPos;
	float4 PlayerPos = Player->GetTransform().GetWorldPosition();
	NextCamPos = float4::Lerp(CamPos, PlayerPos, _DeltaTime * 4.5f);

	if (NextCamPos.x < CamClamp_LeftTop.x)
	{
		NextCamPos.x = CamClamp_LeftTop.x;
	} 
	
	if (NextCamPos.x > CamClamp_RightBottom.x)
	{
		NextCamPos.x = CamClamp_RightBottom.x;
	}
	
	if (NextCamPos.y > CamClamp_LeftTop.y)
	{
		NextCamPos.y = CamClamp_LeftTop.y;
	}

	if (NextCamPos.y < CamClamp_RightBottom.y)
	{
		NextCamPos.y = CamClamp_RightBottom.y;
	}

	GetMainCameraActor()->GetTransform().SetWorldPosition({ NextCamPos.x, NextCamPos.y });
}

void PlayLevel::RoomChangeStart(const StateInfo& _Info)
{
	//Effect_Transition->FadeOut();

	CurRoom = *RoomIter;


	// 다음 방 세팅
	CurRoom->Setting();
	CurRoom->PlayerSpawn(Player);
	CurRoom->SetCameraClampArea(CamClamp_LeftTop, CamClamp_RightBottom);
	GetMainCameraActor()->GetTransform().SetWorldPosition(CurRoom->CamClamp_Center);

	Player->On();
}

void PlayLevel::RoomChangeUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (true)//Effect_Transition->IsTransitionEnd())
	{
		RoomStateManager.ChangeState("RoomPlay");
		return;
	}
}

void PlayLevel::RoomChangeEnd(const StateInfo& _Info)
{

}

void PlayLevel::RoomPlayStart(const StateInfo& _Info)
{
	
}

// @@@ 게임 플레이 @@@
void PlayLevel::RoomPlayUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (true == Player->IsPlayerDead())
	{
		RoomStateManager.ChangeState("RoomRestart");
	}

	// 몬스터를 다 죽였고, 파란 영역에 도달했다면
	if (Player->IsRoomChangeAvailable())
	{
		// 방 변경
		RoomStateManager.ChangeState("RoomExit");
	}

}

void PlayLevel::RoomPlayEnd(const StateInfo& _Info)
{
	
}

void PlayLevel::RoomExitStart(const StateInfo& _Info)
{
	//Effect_Transition->FadeIn();
}

void PlayLevel::RoomExitUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (true)//Effect_Transition->IsTransitionEnd())
	{
		Player->Off();			// 콜리전 맵을 계속 인식해서 잠깐 꺼주어야 함(픽셀체크하는 모든 객체)
		CurRoom->Clear();

		++RoomIter;

		// 마지막 방이면 엔딩
		if (RoomIter == Rooms.end())
		{
			GEngine::ChangeLevel("EndingLevel");
			return;
		}

		RoomStateManager.ChangeState("RoomChange");
	}
}

void PlayLevel::RoomExitEnd(const StateInfo& _Info)
{


}

void PlayLevel::RoomClickToRestartStart(const StateInfo& _Info)
{
	UI->RestartUIOn();
}

void PlayLevel::RoomClickToRestartUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (true == GameEngineInput::GetInst()->IsDown("MouseLeft"))
	{
		UI->RestartUIOff();
		RoomStateManager.ChangeState("RoomChange");

		//리버스
	}
}

void PlayLevel::RoomReverseStart(const StateInfo& _Info)
{
}

void PlayLevel::RoomReverseUpdate(float _DeltaTime, const StateInfo& _Info)
{
}

void PlayLevel::RoomReverseEnd(const StateInfo& _Info)
{
}
