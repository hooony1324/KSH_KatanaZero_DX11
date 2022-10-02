#include "PreCompile.h"
#include "PlayLevel.h"
#include <GameEngineCore/CoreMinimal.h>

#include <GameEngineCore/GEngine.h>
#include "CharacterActor.h"
#include "PlayerZero.h"
#include "Room_Factory1.h"
#include "Room_Factory2.h"
#include "Room_Factory3.h"
#include "Room_Boss.h"
#include "Cursor.h"
#include "UIManager.h"
#include "ControlGUI.h"
#include "SlowMotion.h"
#include "Bullet.h"
#include "CharacterShadow.h"

#include "Effect_Wave.h"
#include "Effect_Distortion.h"
#include "Effect_DistortionGlitch.h"
#include "Effect_PointLight.h"
#include "LiveActor.h"

#include "DiamondTransition.h"
#include "ChromakeyGreen.h"

bool PlayLevel::MainBgmPlaying = false;
bool PlayLevel::BossBgmPlaying = false;
bool PlayLevel::RoomShake = false;
GameEngineSoundPlayer PlayLevel::BGMSoundPlayer;

CharacterActor* PlayLevel::GetPlayer()
{
	return Player;
}

void PlayLevel::ChangeRoom(int _Index)
{
	if (_Index < 0)
	{
		return;
	}

	PrevRoom = CurRoom;
	Player->Off();
	CurRoom->Off();

	Room::CurRoomIndex = _Index;
	CurRoom = Rooms[_Index];

	// 마지막방(보스) 브금 변경
	if (Room::CurRoomIndex == Rooms.size() - 1)
	{
		BGMSoundPlayer.Stop();
	}

	RoomStateManager.ChangeState("RoomChange");
}

PlayLevel* PlayLevel::PlayLevelInst = nullptr;
PlayLevel::PlayLevel()
	: CurRoom(nullptr)
	, PrevRoom(nullptr)
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
	Room3 = CreateActor< Room_Factory3>();
	Room4 = CreateActor<Room_Boss>();
	Rooms.push_back(Room1);
	Rooms.push_back(Room2);
	Rooms.push_back(Room3);
	Rooms.push_back(Room4);

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

	// Effect
	SlowEffect = CreateActor<SlowMotion>();
	SlowEffect->GetTransform().SetWorldPosition({0, 0, GetDepth(ACTOR_DEPTH::SLOWTRANSITON)});

	Transition = CreateActor<DiamondTransition>();
	Transition->GetTransform().SetWorldPosition({ -640, 360, GetDepth(ACTOR_DEPTH::TRANSITION) });

	RoomStateManager.CreateStateMember("RoomChange"
		, std::bind(&PlayLevel::RoomChangeUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&PlayLevel::RoomChangeStart, this, std::placeholders::_1)
		, std::bind(&PlayLevel::RoomChangeEnd, this, std::placeholders::_1));


	RoomStateManager.CreateStateMember("RoomInitialPlay"
		, std::bind(&PlayLevel::RoomInitialPlayUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&PlayLevel::RoomInitialPlayStart, this, std::placeholders::_1));

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


	// Rim Light 용도
	GameEngineActor* Chromakey = CreateActor<ChromakeyGreen>();
	ChromakeyGreen::ChromakeyOff();



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
	GetMainCamera()->GetCameraRenderTarget()->AddEffect<Effect_DistortionGlitch>();
	GetMainCamera()->GetCameraRenderTarget()->AddEffect<Effect_Distortion>();

	GetMainCamera()->GetCameraRenderTarget()->AddEffect<Effect_PointLight>();
	Effect_PointLight::GetInst()->MainCam = GetMainCamera();


}

void PlayLevel::LevelEndEvent()
{
	BGMSoundPlayer.Stop();
}

float ShakeDT = 0.0f;
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
	
	// 카메라 흔들림
	if (true == Player->RoomShakeActivate())
	{
		RoomShake = true;
		ShakeDT = 0.0f;
	}

	if (true == RoomShake)
	{
		float ShakeX = sinf(ShakeDT * 800.0f) * powf(0.98f, ShakeDT * 80.0f);
		float ShakeY = sinf(ShakeDT * 800.0f) * powf(0.97f, ShakeDT * 80.0f);
		GetMainCameraActor()->GetTransform().SetWorldMove({ ShakeX * 10, 0, 0 });
		ShakeDT += _DeltaTime;
	}

	if (ShakeDT > 0.4f)
	{
		RoomShake = false;
		ShakeDT = 0.0f;
		
		if (0 != RoomStateManager.GetCurStateStateName().compare("RoomSlow"))
		{
			TimeGroupNormal();
		}
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
float CurRoomTimeLimit;
void PlayLevel::RoomChangeStart(const StateInfo& _Info)
{

	CurRoom->On();

	// 방 세팅
	CurRoom->PlayerSpawn(Player);
	CurRoom->SetCameraClampArea(CamClamp_LeftTop, CamClamp_RightBottom);
	GetMainCameraActor()->GetTransform().SetWorldPosition(CurRoom->CamClamp_Center);

	Player->On();

	// UI
	RoomPlayTotalTime = 0.0f;
	UI->SetTimeBarLength(1);
	CurRoomTimeLimit = CurRoom->GetTimeLimit();


	if (true == Transition->IsBlack())
	{
		EffectSoundPlayer = GameEngineSound::SoundPlayControl("sound_transition_end.wav");
		EffectSoundPlayer.Volume(0.05f);
		Transition->ChangeState(DiamondTransition::STATE::CHANGEWHITE);
	}

	std::list<GameEngineActor*> Bloods = GetGroup(ACTORGROUP::BLOOD);
	for (auto Blood : Bloods)
	{
		Blood->Death();
	}

}

void PlayLevel::RoomChangeUpdate(float _DeltaTime, const StateInfo& _Info)
{
	CameraFollow(_DeltaTime);


	// 추후 변환 효과
	if (true == Transition->IsChangeWhiteEnd() && _Info.StateTime > 0.3f)
	{
		if (false == MainBgmPlaying)
		{
			RoomStateManager.ChangeState("RoomInitialPlay");
		}
		else
		{
			RoomStateManager.ChangeState("RoomPlay");
		}
		return;
	}

}

void PlayLevel::RoomChangeEnd(const StateInfo& _Info)
{
	// 역재생 초기화
	// 역재생 프레임을 저장할 그룹 판별
	CaptureGroup.clear();
	// Group 1 ~ 3번 캡쳐
	for (int i = 1; i < 4; i++)
	{
		std::list<GameEngineActor*> Group = GetGroup(i);

		for (GameEngineActor* Ptr : Group)
		{
			LiveActor* Actor = dynamic_cast<LiveActor*>(Ptr);
			if (Actor != nullptr && true == Actor->IsUpdate())
			{
				Actor->RemoveCapturedData();
				CaptureGroup.push_back(Actor);
			}
		}
	}


	Effect_Wave::GetInst()->EffectOff();
	Effect_DistortionGlitch::GetInst()->EffectOff();

	GlobalValueManager::SlowEnergy = GlobalValueManager::SlowEnergyMax;
}

bool GlitchStart;
float GlitchSumTime;
void PlayLevel::RoomInitialPlayStart(const StateInfo& _Info)
{
	UI->SetInitialUIOn();

	GlitchSumTime = 0.0f;
	GlitchStart = false;
}

void PlayLevel::RoomInitialPlayUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (_Info.StateTime <= 2.0f)
	{
		return;
	}

	// Sound
	if (false == MainBgmPlaying)
	{
		BGMSoundPlayer = GameEngineSound::SoundPlayControl("song_youwillneverknow.ogg", 100);
		BGMSoundPlayer.Volume(0.025f);
		MainBgmPlaying = true;
	}

	// 클릭하면 디스토션 + Play로
	if (true == GameEngineInput::GetInst()->IsDown("MouseLeft"))
	{
		UI->SetInitialUIOff();
		GlitchStart = true;
		EffectSoundPlayer = GameEngineSound::SoundPlayControl("sound_level_start.wav");
		EffectSoundPlayer.Volume(0.05f);
		Effect_DistortionGlitch::GetInst()->EffectOn();
	}

	if (true == GlitchStart)
	{
		if (GlitchSumTime > 0.3f)
		{
			RoomStateManager.ChangeState("RoomPlay");
			return;
		}

		GlitchSumTime += _DeltaTime;
	}

}

float FrameCaptureTime = 0.016f; // 60FPS
float SlowRecoverTime;
float ShotFrameTime;

void PlayLevel::RoomPlayStart(const StateInfo& _Info)
{
	SlowRecoverTime = 0.0f;

	// 화면 녹화 시작 지점
	ShotFrameTime = 0.0f;

	Player->SetInputValid(true);

	Effect_DistortionGlitch::GetInst()->EffectOff();
	Effect_Wave::GetInst()->EffectOff();

}

// @@@ 게임 플레이 @@@
void PlayLevel::RoomPlayUpdate(float _DeltaTime, const StateInfo& _Info)
{

	// 60Fps에서 15Fps까지 
	
	FrameCaptureTime = 0.0164f * RoomPlayTotalTime;

	// 카메라 플레이어 따라다니기
	CameraFollow(_DeltaTime);

	// 역재생용 프레임 저장
	if (ShotFrameTime >= FrameCaptureTime)
	{
		for (LiveActor* Actor : CaptureGroup)
		{
			Actor->PushFrameCpaturedData();
		}
		ShotFrameTime = 0.0f;
	}



	RoomPlayTotalTime += _DeltaTime;
	ShotFrameTime += _DeltaTime;

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
	Transition->ChangeState(DiamondTransition::STATE::CHANGEBLACK);
	EffectSoundPlayer = GameEngineSound::SoundPlayControl("sound_transition_begin.wav");
	EffectSoundPlayer.Volume(0.1f);
}

void PlayLevel::RoomExitUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (true == Transition->IsChangeBlackEnd())
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

	TimeGroupSlow();
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
	TimeGroupNormal();
	CharacterShadow::SwitchShadowMode();
}

float OriginalDeltaTime;

// 되감기
float ReverseDeltaTime;
float FramePlaySpeed;
void PlayLevel::RoomReverseStart(const StateInfo& _Info)
{
	Player->SetInputValid(false);

	for (LiveActor* Actor : CaptureGroup)
	{
		// 액터의 Update를 막음
		Actor->ReverseStartSetting();
	}

	ReverseDeltaTime = 0.0f;
	FramePlaySpeed = 0.008f;

	Effect_Wave::GetInst()->EffectOn();

	// Sound
	EffectSoundPlayer = GameEngineSound::SoundPlayControl("sound_rewind.wav");
	EffectSoundPlayer.Volume(0.1f);
}

void PlayLevel::RoomReverseUpdate(float _DeltaTime, const StateInfo& _Info)
{
	CameraFollow(_DeltaTime);

	ReverseDeltaTime += _DeltaTime;
	if (ReverseDeltaTime < FramePlaySpeed)
	{
		return;
	}
	ReverseDeltaTime = 0.0f;
	

	for (LiveActor* Actor : CaptureGroup)
	{
		Actor->PlayReverseCapturedData();
	}



	if (Player->IsReverseEnd())
	{
		RoomStateManager.ChangeState("RoomChange");
		return;
	}
}

void PlayLevel::RoomReverseEnd(const StateInfo& _Info)
{

	for (LiveActor* Actor : CaptureGroup)
	{
		Actor->ReverseEndSetting();
	}


	// 총알
	std::list<GameEngineActor*> Bullets = GetGroup(ACTORGROUP::TIMEGROUP_PARTICLE);
	for (auto Bullet : Bullets)
	{
		Bullet->Death();
	}

	Effect_DistortionGlitch::GetInst()->EffectOn();
}

void PlayLevel::TimeGroupSlow()
{
	GameEngineTime::GetInst()->SetTimeScale(static_cast<int>(ACTORGROUP::TIMEGROUP), 0.1f);
	GameEngineTime::GetInst()->SetTimeScale(static_cast<int>(ACTORGROUP::TIMEGROUP_ENEMY), 0.1f);
	GameEngineTime::GetInst()->SetTimeScale(static_cast<int>(ACTORGROUP::TIMEGROUP_PARTICLE), 0.1f);
}

void PlayLevel::TimeGroupNormal()
{
	GameEngineTime::GetInst()->SetTimeScale(static_cast<int>(ACTORGROUP::TIMEGROUP), 1);
	GameEngineTime::GetInst()->SetTimeScale(static_cast<int>(ACTORGROUP::TIMEGROUP_ENEMY), 1);
	GameEngineTime::GetInst()->SetTimeScale(static_cast<int>(ACTORGROUP::TIMEGROUP_PARTICLE), 1);
}
