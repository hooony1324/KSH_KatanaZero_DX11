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
#include "ControlGUI.h"
#include "SlowMotion.h"
#include "Bullet.h"
#include "CharacterShadow.h"

#include "Effect_Wave.h"
#include "Effect_Distortion.h"
#include "Effect_DistortionGlitch.h"
#include "LiveActor.h"

void PlayLevel::ChangeRoom(int _Index)
{
	if (_Index < 0)
	{
		return;
	}

	Player->Off();
	CurRoom->Off();

	Room::CurRoomIndex = _Index;
	CurRoom = Rooms[_Index];

	RoomStateManager.ChangeState("RoomChange");
}

PlayLevel* PlayLevel::PlayLevelInst = nullptr;
PlayLevel::PlayLevel()
	: CurRoom(nullptr)
{
}

PlayLevel::~PlayLevel() 
{
}

void PlayLevel::Start()
{
	PlayLevelInst = this;
	GetMainCamera()->SetProjectionMode(CAMERAPROJECTIONMODE::Orthographic);

	// Rooms
	Room1 = CreateActor<Room_Factory1>();
	Room2 = CreateActor<Room_Factory2>();
	Room3 = CreateActor<Room_Boss>();
	Rooms.push_back(Room1);
	Rooms.push_back(Room2);
	Rooms.push_back(Room3);

	// Player
	Player = CreateActor<PlayerZero>(ACTORGROUP::TIMEGROUP);
	GlobalValueManager::Player = Player;
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
		, std::bind(&PlayLevel::RoomReverseStart, this, std::placeholders::_1)
		, std::bind(&PlayLevel::RoomReverseEnd, this, std::placeholders::_1));

	RoomStateManager.CreateStateMember("RoomRestart"
		, std::bind(&PlayLevel::RoomClickToRestartUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&PlayLevel::RoomClickToRestartStart, this, std::placeholders::_1));

	RoomStateManager.CreateStateMember("RoomSlow"
		, std::bind(&PlayLevel::RoomSlowUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&PlayLevel::RoomSlowStart, this, std::placeholders::_1)
		, std::bind(&PlayLevel::RoomSlowEnd, this, std::placeholders::_1));

	RoomStateManager.CreateStateMember("RoomShake"
		, std::bind(&PlayLevel::RoomShakeUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&PlayLevel::RoomShakeStart, this, std::placeholders::_1));

}

void PlayLevel::LevelStartEvent()
{
	if (false == ControlGUI::GetInst()->IsUpdate())
	{
		ControlGUI::GetInst()->On();
	}

	CurRoom = Rooms[Room::CurRoomIndex];
	RoomStateManager.ChangeState("RoomChange");

	GetMainCamera()->GetCameraRenderTarget()->AddEffect<Effect_Wave>();
	//GetMainCamera()->GetCameraRenderTarget()->AddEffect<Effect_DistortionGlitch>();
}

void PlayLevel::Update(float _DeltaTime)
{
	RoomStateManager.Update(_DeltaTime);

	// 룸 변경(디버그용)
	{
		if (true == GameEngineInput::GetInst()->IsDown("Numpad4"))
		{
			ChangeRoom(0);
		}

		if (true == GameEngineInput::GetInst()->IsDown("Numpad5"))
		{
			ChangeRoom(1);
		}

		if (true == GameEngineInput::GetInst()->IsDown("Numpad6"))
		{
			ChangeRoom(2);
		}	
	}

	// 프리카메라
	if (true == GameEngineInput::GetInst()->IsDown("Numpad0"))
	{
		GetMainCameraActor()->FreeCameraModeOnOff();
	}
	

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
	float4 PlayerPos = Player->GetTransform().GetWorldPosition();
	float4 NextCamPos;
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

	CurRoom->On();

	// 다음 방 세팅
	//CurRoom->Setting();
	CurRoom->PlayerSpawn(Player);
	CurRoom->SetCameraClampArea(CamClamp_LeftTop, CamClamp_RightBottom);
	GetMainCameraActor()->GetTransform().SetWorldPosition(CurRoom->CamClamp_Center);

	Player->On();

	// UI
	RoomPlayTotalTime = 0.0f;
	UI->SetTimeBarLength(1);

	// 역재생
	CaptureGroup.clear();
	for (int i = 1; i < 4; i++)
	{
		std::list<GameEngineActor*> Group = GetGroup(i);

		for (GameEngineActor* Ptr : Group)
		{
			LiveActor* Actor = dynamic_cast<LiveActor*>(Ptr);
			if (Actor != nullptr)
			{
				CaptureGroup.push_back(Actor);
			}
		}
	}
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
	// 역재생용 프레임들 버리기
	Player->RemoveCapturedData();

}

float SlowRecoverTime;
float FrameTime;
void PlayLevel::RoomPlayStart(const StateInfo& _Info)
{
	SlowRecoverTime = 0.0f;

	// 화면 녹화 시작 지점
	FrameTime = 0.0f;
	




}

// @@@ 게임 플레이 @@@
void PlayLevel::RoomPlayUpdate(float _DeltaTime, const StateInfo& _Info)
{
	// 카메라 플레이어 따라다니기
	CameraFollow(_DeltaTime);

	// 역재생용 프레임 저장
	//Player->PushFrameCpaturedData();
	for (LiveActor* Actor : CaptureGroup)
	{
		Actor->PushFrameCpaturedData();
	}


	RoomPlayTotalTime += _DeltaTime;
	FrameTime += _DeltaTime;

	if (GlobalValueManager::SlowEnergy < 11)
	{
		SlowRecoverTime += _DeltaTime;

		if (SlowRecoverTime >= 1.0f)
		{
			GlobalValueManager::SlowEnergy++;
			SlowRecoverTime = 0;
		}
	}

	// 공격 성공시 화면 흔들림 효과
	if (true == Player->CollisionSlashCheck())
	{
		RoomStateManager.ChangeState("RoomShake");
		return;
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



	// 시간제한 있는 방 UI
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


	// 되감기
	if (true == GameEngineInput::GetInst()->IsDown("R"))
	{
		RoomStateManager.ChangeState("RoomReverse");
		return;
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

		ChangeRoom(++Room::CurRoomIndex);
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
	SlowInSound.Volume(0.1f);
	SlowDeltaTime = 0;

	CharacterShadow::SwitchShadowMode();

}

void PlayLevel::RoomSlowUpdate(float _DeltaTime, const StateInfo& _Info)
{
	CameraFollow(_DeltaTime);

	RoomPlayTotalTime += _DeltaTime * 0.5f;
	SlowDeltaTime += _DeltaTime;

	Player->CreateUIShadow();

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
		SlowOutSound.Volume(0.1f);

		return;
	}
}

void PlayLevel::RoomSlowEnd(const StateInfo& _Info)
{
	CharacterShadow::SwitchShadowMode();
}

void PlayLevel::RoomShakeStart(const StateInfo& _Info)
{

	GameEngineTime::GetInst()->SetTimeScale(static_cast<int>(ACTORGROUP::TIMEGROUP), 0.1f);
	GameEngineTime::GetInst()->SetTimeScale(static_cast<int>(ACTORGROUP::TIMEGROUP_ENEMY), 0.1f);
	GameEngineTime::GetInst()->SetTimeScale(static_cast<int>(ACTORGROUP::TIMEGROUP_BULLET), 0.1f);

}

void PlayLevel::RoomShakeUpdate(float _DeltaTime, const StateInfo& _Info)
{
	float DT = _Info.StateTime * 80;
	
	// 카메라 흔들림
	float ShakeX = sinf(DT * 10.0f) * powf(0.98f, DT);
	float ShakeY = sinf(DT * 10.0f) * powf(0.97f, DT);
	GetMainCameraActor()->GetTransform().SetWorldMove({ 0, ShakeY * 10, 0 });

	if (_Info.StateTime >= 0.4f)
	{
		GameEngineTime::GetInst()->SetTimeScale(static_cast<int>(ACTORGROUP::TIMEGROUP), 1);
		GameEngineTime::GetInst()->SetTimeScale(static_cast<int>(ACTORGROUP::TIMEGROUP_ENEMY), 1);
		GameEngineTime::GetInst()->SetTimeScale(static_cast<int>(ACTORGROUP::TIMEGROUP_BULLET), 1);
		RoomStateManager.ChangeState("RoomPlay");
		return;
	}
}

// 되감기

const float ReverseSpeed = 1.0f;
void PlayLevel::RoomReverseStart(const StateInfo& _Info)
{
	


	//Player->ReverseStartSetting();
	for (LiveActor* Actor : CaptureGroup)
	{
		Actor->ReverseStartSetting();
	}

	GameEngineTime::GetInst()->SetTimeScale(static_cast<int>(ACTORGROUP::TIMEGROUP), ReverseSpeed);
}

void PlayLevel::RoomReverseUpdate(float _DeltaTime, const StateInfo& _Info)
{

	//Player->PlayReverseCapturedData();
	for (LiveActor* Actor : CaptureGroup)
	{
		Actor->PlayReverseCapturedData();
	}

	CameraFollow(_DeltaTime);

	if (Player->IsReverseEnd())
	{
		GameEngineTime::GetInst()->SetTimeScale(static_cast<int>(ACTORGROUP::TIMEGROUP), 1.0f);
		RoomStateManager.ChangeState("RoomChange");
		return;
	}
}

void PlayLevel::RoomReverseEnd(const StateInfo& _Info)
{
	//Player->ReverseEndSetting();
	for (LiveActor* Actor : CaptureGroup)
	{
		Actor->ReverseEndSetting();
		Actor->RemoveCapturedData();
	}


	// 총알
	std::list<GameEngineActor*> Bullets = GetGroup(ACTORGROUP::TIMEGROUP_BULLET);
	for (auto Bullet : Bullets)
	{
		Bullet->Death();
	}
}
