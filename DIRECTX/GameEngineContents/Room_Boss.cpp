#include "PreCompile.h"
#include "Room_Boss.h"

#include "BossPsychoGiant.h"
#include "Effect_Distortion.h"
#include "CharacterActor.h"

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
	PlayerSpawnPos = float4{ 640, -780, GetDepth(ACTOR_DEPTH::PLAYER) };

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
}

void Room_Boss::OnEvent()
{
	// 맵 관련
	Background->On();
	GlobalValueManager::ColMap = Background_ColMap;

	Background_Mid->On();
	Background_Front->On();
	Background_Floor->On();


	// 보스
	BossGiant->On();
	StateManager.ChangeState("Roar");

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

	PlayerBlock();
	
}

void Room_Boss::End()
{
}


void Room_Boss::IdleUpdate(float _DeltaTime, const StateInfo& _Info)
{

}

void Room_Boss::RoarStart(const StateInfo& _Info)
{
	RoarSoundPlayer = GameEngineSound::SoundPlayControl("sound_boss_therapist_mutate_03.ogg");
	RoarSoundPlayer.Volume(0.025f);
}

void Room_Boss::RoarUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (_Info.StateTime > 3.0f)
	{
		StateManager.ChangeState("Play");
		return;
	}
}

float DeadTime;
void Room_Boss::PlayStart(const StateInfo& _Info)
{
	DeadTime = 0.0f;
}

void Room_Boss::PlayUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (true == BossGiant->IsDead())
	{
		DeadTime += _DeltaTime;
		if (DeadTime > 2.0f)
		{
			StateManager.ChangeState("Distortion");
			return;
		}
	}


}

void Room_Boss::DistortionStart(const StateInfo& _Info)
{
	GetLevel()->GetMainCamera()->GetCameraRenderTarget()->AddEffect<Effect_Distortion>();
}

void Room_Boss::DistortionUpdate(float _DeltaTime, const StateInfo& _Info)
{


	if (_Info.StateTime > 1.5f)
	{
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


