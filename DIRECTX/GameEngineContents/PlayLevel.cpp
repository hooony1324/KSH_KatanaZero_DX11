#include "PreCompile.h"
#include "PlayLevel.h"
#include <GameEngineCore/CoreMinimal.h>

#include <GameEngineCore/GEngine.h>
#include "CharacterActor.h"
#include "PlayerZero.h"
#include "Room_Factory1.h"
#include "Room_Factory2.h"
#include "Room_Boss.h"
#include "Cursor.h"
#include "UIManager.h"
#include "Transition.h"
#include "PlayLevelGUI.h"
#include "SlowMotion.h"
#include "Bullet.h"

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
	Room3 = CreateActor<Room_Boss>();
	Rooms.push_back(Room1);
	Rooms.push_back(Room2);
	Rooms.push_back(Room3);
	RoomIter = Rooms.begin();

	// Player
	Player = CreateActor<PlayerZero>(ACTORGROUP::TIMEGROUP);
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

	SlowEffect = CreateActor<SlowMotion>();
	SlowEffect->GetTransform().SetWorldPosition({0, 0, GetDepth(ACTOR_DEPTH::SLOWTRANSITON)});


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

	RoomStateManager.CreateStateMember("RoomSlow"
		, std::bind(&PlayLevel::RoomSlowUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&PlayLevel::RoomSlowStart, this, std::placeholders::_1)
		, std::bind(&PlayLevel::RoomSlowEnd, this, std::placeholders::_1));

	GUIWindow = GameEngineGUI::CreateGUIWindow<PlayLevelGUI>("PlayLevelGUI", this);
	GUIWindow->Off();
}

void PlayLevel::OnEvent()
{
	GUIWindow->On();

	CurRoom = *RoomIter;
	RoomStateManager.ChangeState("RoomChange");

}

void PlayLevel::Update(float _DeltaTime)
{
	RoomStateManager.Update(_DeltaTime);

	// 룸 변경(디버그용)
	{
		if (true == GameEngineInput::GetInst()->IsDown("Numpad4"))
		{
			//RoomChange(Room1);
			CurRoom->Clear();
			*RoomIter = Room1;
			RoomStateManager.ChangeState("RoomChange");
		}

		if (true == GameEngineInput::GetInst()->IsDown("Numpad5"))
		{
			CurRoom->Clear();
			*RoomIter = Room2;
			RoomStateManager.ChangeState("RoomChange");
		}

		if (true == GameEngineInput::GetInst()->IsDown("Numpad6"))
		{
			CurRoom->Clear();
			*RoomIter = Room3;
			RoomStateManager.ChangeState("RoomChange");
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

float RoomPlayTotalTime; // 방 1개 단위, 제한시간 비교용도
void PlayLevel::RoomChangeStart(const StateInfo& _Info)
{
	//Effect_Transition->FadeOut();
	//CurRoom->Clear();

	CurRoom = *RoomIter;


	// 다음 방 세팅
	CurRoom->Setting();
	CurRoom->PlayerSpawn(Player);
	CurRoom->SetCameraClampArea(CamClamp_LeftTop, CamClamp_RightBottom);
	GetMainCameraActor()->GetTransform().SetWorldPosition(CurRoom->CamClamp_Center);

	Player->On();

	// UI
	RoomPlayTotalTime = 0.0f;
	UI->SetTimeBarLength(1);
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

float SlowRecoverTime;
void PlayLevel::RoomPlayStart(const StateInfo& _Info)
{
	SlowRecoverTime = 0.0f;

	// 화면 녹화 시작 지점

}

// @@@ 게임 플레이 @@@
void PlayLevel::RoomPlayUpdate(float _DeltaTime, const StateInfo& _Info)
{
	RoomPlayTotalTime += _DeltaTime;

	if (GlobalValueManager::SlowEnergy < 11)
	{
		SlowRecoverTime += _DeltaTime;

		if (SlowRecoverTime >= 1.0f)
		{
			GlobalValueManager::SlowEnergy++;
			SlowRecoverTime = 0;
		}
	}


	// 슬로우 모드
	if (true == GameEngineInput::GetInst()->IsPress("Shift") && GlobalValueManager::SlowEnergy > 0)
	{
		RoomStateManager.ChangeState("RoomSlow");
		return;
	}


	if (true == Player->IsPlayerDeadEnd())
	{
		RoomStateManager.ChangeState("RoomRestart");
		return;
	}

	// 몬스터를 다 죽였고, 포탈 영역에 도달했다면
	if (Player->IsRoomChangeAvailable() && CurRoom->IsEnemyAllDead())
	{
		// 방 변경
		RoomStateManager.ChangeState("RoomExit");
		return;
	}

	// 방 사이즈 별로 월드 밖 이동 제한(보스 방)


	// 시간제한 있는 방
	if (true == CurRoom->IsTimeLimit())
	{
		float LimitTime = CurRoom->GetTimeLimit();

		if (RoomPlayTotalTime >= LimitTime && false == Player->IsPlayerDead())
		{
			Player->SetDead();
			return;
		}

		// UI
		float Ratio = 1 - RoomPlayTotalTime / LimitTime;
		if (Ratio >= 0.0f)
		{
			UI->SetTimeBarLength(Ratio);
		}
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
		// 콜리전 맵을 계속 인식해서 방 변경시 잠깐 꺼주어야 함(픽셀체크하는 모든 객체)
		Player->Off();
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
		RoomStateManager.ChangeState("RoomReverse");

		// 총알은 없앰
		std::list<GameEngineActor*> Bullets = GetGroup(ACTORGROUP::TIMEGROUP_BULLET);
		for (auto Bullet : Bullets)
		{
			Bullet->Death();
		}

		//리버스
	}
}

float SlowDeltaTime;
void PlayLevel::RoomSlowStart(const StateInfo& _Info)
{
	// 배경 어둡게(총알, 플레이어보단 뒤에 있음)
	// 시간 느리게
	// 플레이어 파란 렌더링
	
	SlowEffect->SlowIn();
	SlowInSound = GameEngineSound::SoundPlayControl("sound_slomo_engage.ogg");
	SlowInSound.Volume(0.5f);
	SlowDeltaTime = 0;
}

void PlayLevel::RoomSlowUpdate(float _DeltaTime, const StateInfo& _Info)
{
	RoomPlayTotalTime += _DeltaTime * 0.5f;
	SlowDeltaTime += _DeltaTime;

	if (SlowDeltaTime >= 1.0f)
	{
		if (GlobalValueManager::SlowEnergy > 0)
		{
			GlobalValueManager::SlowEnergy--;
		}
		else
		{
			// 에너지 소진
			RoomStateManager.ChangeState("RoomPlay");
			return;
		}

		SlowDeltaTime = 0.0f;
	}

	if (true == GameEngineInput::GetInst()->IsUp("Shift"))
	{
		SlowEffect->SlowOut();
		RoomStateManager.ChangeState("RoomPlay");

		SlowInSound.Stop();
		SlowOutSound = GameEngineSound::SoundPlayControl("sound_slomo_disengage.wav");
		SlowOutSound.Volume(0.5f);

		return;
	}
}

void PlayLevel::RoomSlowEnd(const StateInfo& _Info)
{
	//GameEngineSound
}

// 역재생 화면
void PlayLevel::RoomReverseStart(const StateInfo& _Info)
{
	int a = 0;
}

void PlayLevel::RoomReverseUpdate(float _DeltaTime, const StateInfo& _Info)
{
}

void PlayLevel::RoomReverseEnd(const StateInfo& _Info)
{
}
