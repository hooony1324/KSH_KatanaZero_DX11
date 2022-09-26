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

	// Rim Light �뵵
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

void PlayLevel::Update(float _DeltaTime)
{
	RoomStateManager.Update(_DeltaTime);

	// �� ����(����׿�)
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

	// ����ī�޶�
	if (true == GameEngineInput::GetInst()->IsDown("Numpad0"))
	{
		GetMainCameraActor()->FreeCameraModeOnOff();
	}
	

	// �浹 �� OnOff
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

float RoomPlayTotalTime; // �� 1�� ����, ���ѽð� �񱳿뵵
void PlayLevel::RoomChangeStart(const StateInfo& _Info)
{


	CurRoom->On();

	// ���� �� ����
	//CurRoom->Setting();
	CurRoom->PlayerSpawn(Player);
	CurRoom->SetCameraClampArea(CamClamp_LeftTop, CamClamp_RightBottom);
	GetMainCameraActor()->GetTransform().SetWorldPosition(CurRoom->CamClamp_Center);

	Player->On();

	// UI
	RoomPlayTotalTime = 0.0f;
	UI->SetTimeBarLength(1);

	if (true == Transition->IsBlack())
	{
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


	// ���� ��ȯ ȿ��
	if (true == Transition->IsChangeWhiteEnd() && _Info.StateTime > 0.3f)
	{
		RoomStateManager.ChangeState("RoomPlay");
		return;
	}

}

void PlayLevel::RoomChangeEnd(const StateInfo& _Info)
{
	// ����� �ʱ�ȭ
	// ����� �������� ������ �׷� �Ǻ�
	CaptureGroup.clear();
	// Group 1 ~ 3�� ĸ��
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
	Player->SetInputValid(true);
}

float FrameCaptureTime = 0.016f; // 60FPS
float SlowRecoverTime;
float ShotFrameTime;
void PlayLevel::RoomPlayStart(const StateInfo& _Info)
{
	SlowRecoverTime = 0.0f;

	// ȭ�� ��ȭ ���� ����
	ShotFrameTime = 0.0f;


}

// @@@ ���� �÷��� @@@
void PlayLevel::RoomPlayUpdate(float _DeltaTime, const StateInfo& _Info)
{

	FrameCaptureTime = GameEngineMath::LerpLimit(0.016f, 0.06f, RoomPlayTotalTime);

	// ī�޶� �÷��̾� ����ٴϱ�
	CameraFollow(_DeltaTime);

	// ������� ������ ����
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

	// ���� ������ ȭ�� ��鸲 ȿ��
	if (true == Player->RoomShakeActivate())
	{
		RoomStateManager.ChangeState("RoomShake");
		return;
	}

	// ���ο� ���
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

	// ���͸� �� �׿���, ��Ż ������ �����ߴٸ�
	if (Player->IsRoomChangeAvailable() && CurRoom->IsEnemyAllDead())
	{
		// �� ����
		RoomStateManager.ChangeState("RoomExit");
		return;
	}



	// �ð����� �ִ� �� UI
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


	// �ǰ���
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
	// ��� ��Ӱ�(�Ѿ�, �÷��̾�� �ڿ� ����)
	// �ð� ������
	// �÷��̾� �Ķ� ������
	
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
			// ������ ����
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
	GameEngineTime::GetInst()->SetTimeScale(static_cast<int>(ACTORGROUP::TIMEGROUP_PARTICLE), 0.1f);

}

void PlayLevel::RoomShakeUpdate(float _DeltaTime, const StateInfo& _Info)
{
	float DT = _Info.StateTime * 80;
	
	// ī�޶� ��鸲
	float ShakeX = sinf(DT * 10.0f) * powf(0.98f, DT);
	float ShakeY = sinf(DT * 10.0f) * powf(0.97f, DT);
	GetMainCameraActor()->GetTransform().SetWorldMove({ 0, ShakeY * 10, 0 });

	if (_Info.StateTime >= 0.4f)
	{
		GameEngineTime::GetInst()->SetTimeScale(static_cast<int>(ACTORGROUP::TIMEGROUP), 1);
		GameEngineTime::GetInst()->SetTimeScale(static_cast<int>(ACTORGROUP::TIMEGROUP_ENEMY), 1);
		GameEngineTime::GetInst()->SetTimeScale(static_cast<int>(ACTORGROUP::TIMEGROUP_PARTICLE), 1);
		RoomStateManager.ChangeState("RoomPlay");
		return;
	}
}

// �ǰ���
float ReverseDeltaTime;
float FramePlaySpeed;
void PlayLevel::RoomReverseStart(const StateInfo& _Info)
{
	Player->SetInputValid(false);

	for (LiveActor* Actor : CaptureGroup)
	{
		// ������ Update�� ����
		Actor->ReverseStartSetting();
	}

	ReverseDeltaTime = 0.0f;
	FramePlaySpeed = 0.008f;

	Effect_Wave::GetInst()->EffectOn();
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


	// �Ѿ�
	std::list<GameEngineActor*> Bullets = GetGroup(ACTORGROUP::TIMEGROUP_PARTICLE);
	for (auto Bullet : Bullets)
	{
		Bullet->Death();
	}

	Effect_DistortionGlitch::GetInst()->EffectOn();
}
