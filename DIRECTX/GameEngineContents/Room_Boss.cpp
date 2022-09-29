#include "PreCompile.h"
#include "Room_Boss.h"

#include "BossPsychoGiant.h"
#include "Effect_Distortion.h"
#include "CharacterActor.h"
#include "Effect_Distortion.h"
#include "Effect_Wave.h"

#include "PlayLevel.h"

Room_Boss::Room_Boss()
	: Background_Mid(nullptr)
	, Background_Floor(nullptr)
	, Background_Front(nullptr)
{
}

Room_Boss::~Room_Boss() 
{
}

void Room_Boss::Start()
{
	EmptyRoomInit();

	Background->SetTexture("spr_psychboss_background.png");
	Background->GetTransform().SetLocalScale({ 1280, 720 });
	Background->SetPivot(PIVOTMODE::LEFTTOP);
	Background->GetTransform().SetLocalMove({ 0, 0, GetDepth(ACTOR_DEPTH::BACKGROUND_0)});
	Background->Off();

	Background_ColMap->SetTexture("spr_psychboss_background_colmap.png");
	Background_ColMap->ScaleToTexture();
	Background_ColMap->SetPivot(PIVOTMODE::LEFTTOP);
	Background_ColMap->GetTransform().SetLocalMove({ 0, 0, GetDepth(ACTOR_DEPTH::BACKGROUND_COL) });
	Background_ColMap->Off();

	// 배경 설정되고 카메라 클램프 영역 세팅
	InitCameraClampArea();

	// 스폰위치, 지형지물 등
	PlayerSpawnPos = float4{ 640, -760, GetDepth(ACTOR_DEPTH::PLAYER) };

	// bg > floor > fg
	Background_Mid = CreateComponent<GameEngineTextureRenderer>();
	Background_Mid->SetTexture("spr_psychboss_bg_1_0.png");
	Background_Mid->GetTransform().SetLocalScale({1280, 734, 1});
	Background_Mid->GetTransform().SetLocalMove({ 640, -560, GetDepth(ACTOR_DEPTH::BOSSMIDGROUND) });
	Background_Mid->Off();

	Background_Front = CreateComponent<GameEngineTextureRenderer>();
	Background_Front->SetTexture("spr_psychboss_fg_0.png");
	Background_Front->GetTransform().SetLocalScale({1350, 774, 1});
	Background_Front->GetTransform().SetLocalMove({ 640, -545, GetDepth(ACTOR_DEPTH::BOSSFOREGROUND) });
	Background_Front->Off();

	Background_FrontRed = CreateComponent<GameEngineTextureRenderer>();
	Background_FrontRed->SetTexture("None.png");
	Background_FrontRed->GetTransform().SetLocalScale({ 1280, 720, 1 });
	Background_FrontRed->GetTransform().SetWorldPosition({ 640, -510, GetDepth(ACTOR_DEPTH::TRANSITION) });
	Background_FrontRed->GetPixelData().PlusColor.r = 1;
	Background_FrontRed->GetPixelData().MulColor.g = 0;
	Background_FrontRed->GetPixelData().MulColor.b = 0;
	Background_FrontRed->GetPixelData().MulColor.a = 1;
	
	Background_FrontRed->Off();

	Background_Floor = CreateComponent<GameEngineTextureRenderer>();
	Background_Floor->SetTexture("spr_psychboss_floor_0.png");
	Background_Floor->GetTransform().SetLocalScale({1072, 140, 1});
	Background_Floor->GetTransform().SetLocalMove({ 640, -852, GetDepth(ACTOR_DEPTH::BOSSFLOOR) });
	Background_Floor->Off();

	// 보스
	BossGiant = GetLevel()->CreateActor<BossPsychoGiant>();
	BossGiant->GetTransform().SetWorldPosition({ 637, -541, GetDepth(ACTOR_DEPTH::BOSS) });
	BossGiant->Off();

	// 세팅 시간
	TimeLimit = false;

	StateManager.CreateStateMember("Idle"
		, std::bind(&Room_Boss::IdleUpdate, this, std::placeholders::_1, std::placeholders::_2));

	StateManager.CreateStateMember("CutScene"
		, std::bind(&Room_Boss::CutSceneUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&Room_Boss::CutSceneStart, this, std::placeholders::_1));

	StateManager.CreateStateMember("Roar"
		, std::bind(&Room_Boss::RoarUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&Room_Boss::RoarStart, this, std::placeholders::_1));

	StateManager.CreateStateMember("Play"
		, std::bind(&Room_Boss::PlayUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&Room_Boss::PlayStart, this, std::placeholders::_1));

	StateManager.CreateStateMember("Distortion"
		, std::bind(&Room_Boss::DistortionUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&Room_Boss::DistortionStart, this, std::placeholders::_1));

	StateManager.ChangeState("Idle");

	Off();

	// 컷씬
	CutSceneSetting();

}


// change room 때마다 호출
void Room_Boss::OnEvent()
{

	PlayLevel::PlayLevelInst->BGMSoundPlayer.Stop();

	// 맵 관련
	GlobalValueManager::ColMap = Background_ColMap;

	// 상태에 따라 : 최초 시작, 방 리버스 된 후
	if (Background_FrontRed->GetPixelData().MulColor.a <= 0.05f)
	{
		StateManager.ChangeState("Play");
	}
	else
	{
		StateManager.ChangeState("CutScene");
	}

}

void Room_Boss::OffEvent()
{

	// 맵 관련
	Background->Off();
	if (nullptr != GlobalValueManager::ColMap)
	{
		GlobalValueManager::ColMap->Off();
	}

	Background_Mid->Off();
	Background_Front->Off();
	Background_Floor->Off();

	// 보스
	BossGiant->Off();

}


void Room_Boss::Update(float _DeltaTime)
{
	StateManager.Update(_DeltaTime);
	
}

void Room_Boss::End()
{
}


void Room_Boss::IdleUpdate(float _DeltaTime, const StateInfo& _Info)
{

}

void Room_Boss::CutSceneStart(const StateInfo& _Info)
{
	CutScene_Back->On();
	CutScene_Player->On();
	CutScene_Boss->On();

	CutSceneStateManager.ChangeState("SceneMutate");
}

void Room_Boss::CutSceneUpdate(float _Deltatime, const StateInfo& _Info)
{
	CutSceneStateManager.Update(_Deltatime);
}

bool BossBgmPlayed;
void Room_Boss::RoarStart(const StateInfo& _Info)
{
	Background->On();

	Background_Mid->On();
	Background_Front->On();
	Background_FrontRed->On();
	Background_Floor->On();
	BossGiant->On();

	BossBgmPlayed = false;
}

void Room_Boss::RoarUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (Background_FrontRed->GetPixelData().MulColor.a <= 0.000005f)
	{

		Background_FrontRed->Off();
		StateManager.ChangeState("Play");
		return;
	}

	if (_Info.StateTime < 0.5f)
	{
		return;
	}

	if (false == BossBgmPlayed && false == PlayLevel::PlayLevelInst->BossBgmPlaying)
	{
		BossBgmPlayed = true;
		PlayLevel::PlayLevelInst->BGMSoundPlayer = GameEngineSound::SoundPlayControl("song_monster.ogg", 100);
		PlayLevel::PlayLevelInst->BossBgmPlaying = true;
		PlayLevel::PlayLevelInst->MainBgmPlaying = false;
		PlayLevel::PlayLevelInst->BGMSoundPlayer.Volume(0.05f);
	}

	if (_Info.StateTime < 4.5f)
	{
		return;
	}

	if (Background_FrontRed->GetPixelData().MulColor.a > 0.0f)
	{
		Background_FrontRed->GetPixelData().MulColor.a -= _DeltaTime * 0.3f;
		if (Background_FrontRed->GetPixelData().MulColor.a <= 0.000005f)
		{
			Background_FrontRed->GetPixelData().MulColor.a = 0.000001f;
		}
	}

}

float DeadTime;
void Room_Boss::PlayStart(const StateInfo& _Info)
{
	DeadTime = 0.0f;

	{
		Background->On();
		Background_Mid->On();
		Background_Front->On();
		Background_Floor->On();
		BossGiant->On();
	}

}

void Room_Boss::PlayUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (true == BossGiant->IsDead())
	{
		PlayLevel::PlayLevelInst->BGMSoundPlayer.Stop();

		DeadTime += _DeltaTime;
		if (DeadTime > 2.0f)
		{
			StateManager.ChangeState("Distortion");
			return;
		}
	}

	PlayerBlock();
}

void Room_Boss::DistortionStart(const StateInfo& _Info)
{
	Effect_Distortion::GetInst()->EffectOn();
}

void Room_Boss::DistortionUpdate(float _DeltaTime, const StateInfo& _Info)
{

	if (_Info.StateTime > 1.0f)
	{
		Effect_Distortion::GetInst()->EffectOff();
		GEngine::ChangeLevel("EndingLevel");
		return;
	}
}

void Room_Boss::PlayerBlock()
{
	float4 Pos = GlobalValueManager::PlayerPos;
	if (Pos.x <= 113)
	{
		GlobalValueManager::Player->GetTransform().SetWorldPosition(Pos + float4::RIGHT);
	}

	if (Pos.x >= 1130)
	{
		GlobalValueManager::Player->GetTransform().SetWorldPosition(Pos + float4::LEFT);
	}
}


void Room_Boss::CutSceneSetting()
{
	/// 렌더러 세팅 ///
	CutScene_Back = CreateComponent<GameEngineTextureRenderer>();
	CutScene_Player = CreateComponent<GameEngineTextureRenderer>();
	CutScene_Player->SetSamplingModePoint();
	CutScene_Boss = CreateComponent<GameEngineTextureRenderer>();
	CutScene_Boss->SetSamplingModePoint();

	// BACKGROUND
	CutScene_Back->SetTexture("bg_psychiatrist_boss_0.png");
	CutScene_Back->GetTransform().SetLocalScale({ 1278, 720, 1 });
	CutScene_Back->GetTransform().SetWorldPosition({ 640, -510, 0 });

	// PLAYER
	CutScene_Player->GetTransform().SetLocalScale({ 62, 72, 1 });
	CutScene_Player->SetPivot(PIVOTMODE::BOT);
	CutScene_Player->CreateFrameAnimationFolder("walkback", FrameAnimation_DESC{ "player_casualwalk", 0.1f, false });
	CutScene_Player->CreateFrameAnimationFolder("idle", FrameAnimation_DESC{ "player_casualidle", 0.1f, true });
	CutScene_Player->ChangeFrameAnimation("idle");
	CutScene_Player->GetTransform().SetWorldPosition({ 570, -695, 0 });

	// BOSS
	CutScene_Boss->SetPivot(PIVOTMODE::BOT);
	CutScene_Boss->CreateFrameAnimationFolder("idle", FrameAnimation_DESC{ "boss_idle", 0.1f, true});
	CutScene_Boss->CreateFrameAnimationFolder("mutate", FrameAnimation_DESC{ "boss_mutate", 0.1f, false });
	CutScene_Boss->CreateFrameAnimationFolder("mutated", FrameAnimation_DESC{ "boss_mutated", 0.1f, true });
	CutScene_Boss->ChangeFrameAnimation("idle");
	CutScene_Boss->SetScaleModeImage();
	CutScene_Boss->SetScaleRatio(2);
	CutScene_Boss->GetTransform().SetWorldPosition({ 690, -695, 0 });

	CutScene_Boss->AnimationBindEnd("mutate", [=](const FrameAnimation_DESC& _Info)
		{
			CutSceneStateManager.ChangeState("SceneDistortion");
		});

	CutScene_Boss->AnimationBindFrame("mutate", 
		[=](const FrameAnimation_DESC& _Info)
		{
			if (_Info.CurFrame == 26)
			{
				// 카메라 흔들림 + 뒷걸음질
				WalkStart = true;
				CutScene_Player->GetTransform().PixLocalNegativeX();
				CutScene_Player->ChangeFrameAnimation("walkback");
			}
		});


	CutScene_Back->Off();
	CutScene_Player->Off();
	CutScene_Boss->Off();


	/// FSM 세팅 ///
	CutSceneStateManager.CreateStateMember("SceneIdle"
		, std::bind(&Room_Boss::SceneIdleUpdate, this, std::placeholders::_1, std::placeholders::_2));

	CutSceneStateManager.CreateStateMember("SceneMutate"
		, std::bind(&Room_Boss::SceneMutateUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&Room_Boss::SceneMutateStart, this, std::placeholders::_1));

	CutSceneStateManager.CreateStateMember("SceneDistortion"
		, std::bind(&Room_Boss::SceneDistortionUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&Room_Boss::SceneDistortionStart, this, std::placeholders::_1));

	CutSceneStateManager.CreateStateMember("SceneMutated"
		, std::bind(&Room_Boss::SceneMutatedUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&Room_Boss::SceneMutatedStart, this, std::placeholders::_1));

	CutSceneStateManager.ChangeState("SceneIdle");
}

void Room_Boss::SceneIdleUpdate(float _DeltaTime, const StateInfo& _Info)
{

}

float WalkTime;
bool MutateStart;
void Room_Boss::SceneMutateStart(const StateInfo& _Info)
{
	MutateStart = false;
	WalkStart = false;
	WalkTime = 0.0f;

}

void Room_Boss::SceneMutateUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (_Info.StateTime < 1.0f)
	{
		return;
	}

	if (false == MutateStart)
	{
		PlayLevel::PlayLevelInst->BGMSoundPlayer = GameEngineSound::SoundPlayControl("sound_boss_therapist_mutate_01.ogg");
		PlayLevel::PlayLevelInst->BGMSoundPlayer.Volume(0.2f);
		MutateStart = true;
		CutScene_Boss->ChangeFrameAnimation("mutate");
	}


	if (true == WalkStart)
	{
		float4 LerpPos = float4::LerpLimit({ 570, -695, 0 }, { 470, -695, 0 }, WalkTime);
		CutScene_Player->GetTransform().SetWorldPosition(LerpPos);
		WalkTime += _DeltaTime;
	}

}

void Room_Boss::SceneDistortionStart(const StateInfo& _Info)
{
	WalkStart = false;
	CutScene_Player->GetTransform().PixLocalPositiveX();
	CutScene_Player->ChangeFrameAnimation("idle");
	

	// 디스토션
	Effect_Distortion::GetInst()->EffectOn();
}

void Room_Boss::SceneDistortionUpdate(float _DeltaTime, const StateInfo& _Info)
{

	if (_Info.StateTime > 0.6f)
	{
		Effect_Distortion::GetInst()->EffectOff();
		CutScene_Boss->ChangeFrameAnimation("mutated");
		CutSceneStateManager.ChangeState("SceneMutated");
		return;
	}

}

void Room_Boss::SceneMutatedStart(const StateInfo& _Info)
{
	// 울부짖음 + 웨이브
	Effect_Wave::GetInst()->EffectOn();
	Effect_Wave::GetInst()->Option.Version = 0;

	// 울부짖음 사운드
	PlayLevel::PlayLevelInst->BGMSoundPlayer = GameEngineSound::SoundPlayControl("sound_boss_therapist_mutate_02.ogg");
	PlayLevel::PlayLevelInst->BGMSoundPlayer.Volume(0.2f);

	// 월드 -> 뷰포트, 포스트 이펙트에서 제외용도
	// MainCam이 플레이어 때문에 이동해 있음
	GameEngineCamera* MainCam = GetLevel()->GetMainCamera();
	float4 PlayerPos = CutScene_Player->GetTransform().GetWorldPosition();
	float4 BossPos = CutScene_Boss->GetTransform().GetWorldPosition();
	float4 CamPos = MainCam->GetTransform().GetWorldPosition();

	float4x4 MVP = MainCam->GetTransform().GetWorldViewProjection();

	PlayerPos = (PlayerPos - CamPos) * MVP;
	BossPos = (BossPos - CamPos) * MVP;

	CutScene_Player->ChangeCamera(CAMERAORDER::UICAMERA);
	CutScene_Boss->ChangeCamera(CAMERAORDER::UICAMERA);

	CutScene_Player->GetTransform().SetWorldPosition(PlayerPos);
	CutScene_Boss->GetTransform().SetWorldPosition(BossPos);
}

void Room_Boss::SceneMutatedUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (_Info.StateTime > 2.5f)
	{
		// 컷씬 종료
		CutScene_Back->Off();
		CutScene_Player->Off();
		CutScene_Boss->Off();
		Effect_Wave::GetInst()->EffectOff();
		Effect_Wave::GetInst()->Option.Version = 1;
		RoarSoundPlayer = GameEngineSound::SoundPlayControl("sound_boss_therapist_mutate_03.ogg");
		RoarSoundPlayer.Volume(0.1f);
		StateManager.ChangeState("Roar");
		return;
	}


}