#include "PreCompile.h"
#include "BossPsychoGiant.h"
#include <GameEngineCore/CoreMinimal.h>

#include "TentacleKnife.h"
#include "TentacleBoss.h"
#include "Portal.h"
#include "PortalKnife.h"
#include "PortalTurret.h"
#include <GameEngineBase/GameEngineRandom.h>

BossPsychoGiant* BossPsychoGiant::GlobalInst = nullptr;

BossPsychoGiant::BossPsychoGiant() 
	: Hp(3)
{
	GlobalInst = this;
	IsSelectPattern = false;
}

BossPsychoGiant::~BossPsychoGiant() 
{
}

void BossPsychoGiant::Start()
{
	// 촉수
	Renderer_Tentacle = CreateComponent<GameEngineTextureRenderer>();
	Renderer_Tentacle->SetSamplingModePoint();
	Renderer_Tentacle->CreateFrameAnimationFolder("tentacle_idle",
		FrameAnimation_DESC{ "psychoboss_tentacle_idle", 0.12f });
	Renderer_Tentacle->GetTransform().SetLocalScale({ 675, 387, 1 });

	Renderer_Tentacle->CreateFrameAnimationFolder("tentacle_stab",
		FrameAnimation_DESC{ "psychoboss_tentacle_stab", 0.12f, false });
	Renderer_Tentacle->GetTransform().SetLocalScale({ 675, 387, 1 });

	Renderer_Tentacle->CreateFrameAnimationFolder("tentacle_stabend",
		FrameAnimation_DESC{ "psychoboss_tentacle_stabend", 0.12f, false });
	Renderer_Tentacle->GetTransform().SetLocalScale({ 675, 387, 1 });

	Renderer_Tentacle->ChangeFrameAnimation("tentacle_idle");
	Renderer_Tentacle->SetOrder(static_cast<int>(ACTORGROUP::TIMEGROUP));
	Renderer_Tentacle->Off();


	// 얼굴
	Renderer_Face = CreateComponent<GameEngineTextureRenderer>();
	Renderer_Face->SetSamplingModePoint();
	Renderer_Face->CreateFrameAnimationFolder("face_idle",
		FrameAnimation_DESC{ "psychoboss_face_idle", 0.12f });
	Renderer_Face->GetTransform().SetLocalScale({ 675, 387, 1 });

	Renderer_Face->CreateFrameAnimationFolder("face_hurt",
		FrameAnimation_DESC{ "psychoboss_face_hurt", 0.12f });
	Renderer_Face->GetTransform().SetLocalScale({ 675, 387, 1 });
	Renderer_Face->ChangeFrameAnimation("face_idle");
	Renderer_Face->SetOrder(static_cast<int>(ACTORGROUP::TIMEGROUP));
	Renderer_Face->Off();

	// 몸
	Renderer_Body = CreateComponent<GameEngineTextureRenderer>();
	Renderer_Body->SetSamplingModePoint();
	Renderer_Body->CreateFrameAnimationFolder("body",
		FrameAnimation_DESC{ "psychoboss_body", 0.09f });
	Renderer_Body->GetTransform().SetLocalScale({ 675, 387, 1 });
	Renderer_Body->ChangeFrameAnimation("body");
	Renderer_Body->SetOrder(static_cast<int>(ACTORGROUP::TIMEGROUP));
	Renderer_Body->Off();

	Renderer_Lilguy = CreateComponent<GameEngineTextureRenderer>();
	Renderer_Lilguy->SetSamplingModePoint();
	Renderer_Lilguy->CreateFrameAnimationFolder("lilguy",
		FrameAnimation_DESC{ "psychoboss_lilguy", 0.09f });
	Renderer_Lilguy->GetTransform().SetLocalScale({ 675, 387, 1 });
	Renderer_Lilguy->ChangeFrameAnimation("lilguy");
	Renderer_Lilguy->SetOrder(static_cast<int>(ACTORGROUP::TIMEGROUP));
	Renderer_Lilguy->Off();

	// 포탈 최대 11개
	for (int i = 0; i < 11; i++)
	{
		Portal* PortalAttack = GetLevel()->CreateActor<Portal>();
		PortalAttack->Off();
		Portals.push_back(PortalAttack);
	}

	// 작은녀석
	SmallBoss = GetLevel()->CreateActor<TentacleBoss>();

	AllAnimationBind();
	AllStateCreate();

	GetTransform().SetWorldScale({ 2, 2, 1 });
	Off();
}

void BossPsychoGiant::Update(float _DeltaTime)
{



	BossStateManager.Update(_DeltaTime);

}

void BossPsychoGiant::End()
{
}

void BossPsychoGiant::OnEvent()
{
	Renderer_Tentacle->On();
	Renderer_Body->On();
	Renderer_Face->On();
	Renderer_Lilguy->On();

	Stabber = GetLevel()->CreateActor<TentacleKnife>();

	IsSelectPattern = false;
	BossStateManager.ChangeState("Idle");

	for (Portal* Ptr : Portals)
	{
		Ptr->Off();
	}

	Hp = 3;
	AttackPatternCount = 0;
}

void BossPsychoGiant::OffEvent()
{
	Renderer_Tentacle->Off();
	Renderer_Body->Off();
	Renderer_Face->Off();
	Renderer_Lilguy->Off();

	if (Stabber != nullptr)
	{
		Stabber->Death();
	}

	for (Portal* Ptr : Portals)
	{
		Ptr->Off();
	}

}

void BossPsychoGiant::AllStateCreate()
{
	BossStateManager.CreateStateMember("Idle"
		, std::bind(&BossPsychoGiant::IdleUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&BossPsychoGiant::IdleStart, this, std::placeholders::_1));

	BossStateManager.CreateStateMember("Stab"
		, std::bind(&BossPsychoGiant::StabAttackUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&BossPsychoGiant::StabAttackStart, this, std::placeholders::_1));

	BossStateManager.CreateStateMember("SpawnKnife1"
		, std::bind(&BossPsychoGiant::SpawnKnife1Update, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&BossPsychoGiant::SpawnKnife1Start, this, std::placeholders::_1));

	BossStateManager.CreateStateMember("SpawnKnife2"
		, std::bind(&BossPsychoGiant::SpawnKnife2Update, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&BossPsychoGiant::SpawnKnife2Start, this, std::placeholders::_1));

	BossStateManager.CreateStateMember("SpawnTurret"
		, std::bind(&BossPsychoGiant::SpawnTurretUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&BossPsychoGiant::SpawnTurretStart, this, std::placeholders::_1));

	BossStateManager.CreateStateMember("SpawnPsycho"
		, std::bind(&BossPsychoGiant::SpawnPsychoUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&BossPsychoGiant::SpawnPsychoStart, this, std::placeholders::_1));

	BossStateManager.CreateStateMember("Hurt"
		, std::bind(&BossPsychoGiant::HurtUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&BossPsychoGiant::HurtStart, this, std::placeholders::_1));

}

void BossPsychoGiant::AllAnimationBind()
{
	Renderer_Tentacle->AnimationBindEnd("tentacle_stabend",
		[=](const FrameAnimation_DESC& _Info)
		{ 
			Renderer_Tentacle->ChangeFrameAnimation("tentacle_idle");
			BossStateManager.ChangeState("Idle");
		});

	// 촉수 위로 펼침
	Renderer_Tentacle->AnimationBindStart("tentacle_stab",
		[=](const FrameAnimation_DESC& _Info)
		{ Stabber->SetTakeOutEnd(false); });

	Renderer_Tentacle->AnimationBindEnd("tentacle_stab",
		[=](const FrameAnimation_DESC& _Info)
		{ Stabber->SetTakeOutEnd(true); });
}

// 아무것도 안하는 상태
void BossPsychoGiant::IdleStart(const StateInfo& _Info)
{
	Pattern = PSYCHOGIANT_PATTERN::IDLE;
	Renderer_Face->ChangeFrameAnimation("face_idle");
	
	Stabber->On();
}

void BossPsychoGiant::IdleUpdate(float _DeltaTime, const StateInfo& _Info)
{
	// 가만히 있는다


	if (false == IsSelectPattern)
	{
		return;
	}

	// 패툰 선택
	if (AttackPatternCount > 3)
	{
		ChangePattern(PSYCHOGIANT_PATTERN::SPAWN_PSYCHO);
		return;
	}

	int PatternIndex = GameEngineRandom::MainRandom.RandomInt(2, 5);
	ChangePattern(static_cast<PSYCHOGIANT_PATTERN>(PatternIndex));

}


bool StabberAttackStart;
int StabCount;
void BossPsychoGiant::StabAttackStart(const StateInfo& _Info)
{
	Pattern = PSYCHOGIANT_PATTERN::STAB;
	Renderer_Tentacle->ChangeFrameAnimation("tentacle_stab");


	Stabber->SetAttackEnd(false);
	StabberAttackStart = false;

	StabCount = GameEngineRandom::MainRandom.RandomInt(3, 4);
	AttackPatternCount++;
}

void BossPsychoGiant::StabAttackUpdate(float _DeltaTime, const StateInfo& _Info)
{
	// takeout 완료 시 공격 시작
	if (true == Stabber->IsTakeOutEnd() && false == StabberAttackStart)
	{
		StabberAttackStart = true;
		Stabber->StabAttackStart(StabCount);
	}

	if (true == Stabber->IsStabAttackEnd())
	{
		Stabber->SetTakeOutEnd(false);
		Renderer_Tentacle->ChangeFrameAnimation("tentacle_stabend");
	}

}

void BossPsychoGiant::SpawnKnife1Start(const StateInfo& _Info)
{
	SpawnPortalsUp();
	AttackPatternCount++;
}

void BossPsychoGiant::SpawnKnife1Update(float _DeltaTime, const StateInfo& _Info)
{
	// 0 ~ 6 랜덤 시간차 생성..

	if (_Info.StateTime > 7.0f)
	{
		BossStateManager.ChangeState("Idle");
	}
}

void BossPsychoGiant::SpawnKnife2Start(const StateInfo& _Info)
{
	SpawnPortalsRound();
	AttackPatternCount++;
}

void BossPsychoGiant::SpawnKnife2Update(float _DeltaTime, const StateInfo& _Info)
{
	if (_Info.StateTime > 7.0f)
	{
		BossStateManager.ChangeState("Idle");
	}
}

void BossPsychoGiant::SpawnTurretStart(const StateInfo& _Info)
{
	SpawnPortalsDown();
	AttackPatternCount++;
}

void BossPsychoGiant::SpawnTurretUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (_Info.StateTime > 7.0f)
	{
		BossStateManager.ChangeState("Idle");
	}
}

void BossPsychoGiant::SpawnPsychoStart(const StateInfo& _Info)
{
	// 1번과 5번중 랜덤 하나
	for (int i = 1; i < 6; i++)
	{
		Portals[i]->GetTransform().SetWorldPosition({ 170 + i * 154.0f, -820, GetDepth(ACTOR_DEPTH::BOSSPORTAL) });
		Portals[i]->Off();
		
	}

	int Index = GameEngineRandom::MainRandom.RandomInt(1, 5);

	Portals[Index]->On();
	Portals[Index]->GetTransform().SetWorldRotation({ 0, 0, 90.05f });
	SmallBoss->GetTransform().SetWorldPosition(Portals[Index]->GetTransform().GetWorldPosition() + float4{-10,-10,-10,0});
	SmallBoss->Spawn();

	AttackPatternCount = 0;
}

void BossPsychoGiant::SpawnPsychoUpdate(float _DeltaTime, const StateInfo& _Info)
{
	
	if (true == SmallBoss->IsHurt())
	{
		BossStateManager.ChangeState("Hurt");
		return;
	}


	if (false == SmallBoss->IsUpdate())
	{
		BossStateManager.ChangeState("Idle");
		return;
	}
}

float RecoverTime;
const float CamShakeSpeed = 10.0f;
void BossPsychoGiant::HurtStart(const StateInfo& _Info)
{
	RecoverTime = 0;
	Renderer_Body->GetPixelData().MulColor = float4::RED;
	Renderer_Face->GetPixelData().MulColor = float4::RED;

	Renderer_Face->ChangeFrameAnimation("face_hurt");
	Hp--;
}

void BossPsychoGiant::HurtUpdate(float _DeltaTime, const StateInfo& _Info)
{
	float DT = _Info.StateTime;

	// 카메라 흔들림
	if (DT <= 2.0f)
	{
		float ShakeX = sinf(DT * 10.0f * CamShakeSpeed) * powf(0.88f, DT * CamShakeSpeed);
		float ShakeY = sinf(DT * 10.0f * CamShakeSpeed) * powf(0.88f, DT * CamShakeSpeed);

		GetLevel()->GetMainCameraActor()->GetTransform().SetWorldMove({ ShakeX * 20, ShakeY * 10, 0 });
	}

	if (DT > 1.5f)
	{
		// 색 점점 원상복구
		float4 Color = float4::Lerp(float4::RED, float4::ONE, RecoverTime);
		Renderer_Body->GetPixelData().MulColor = Color;
		Renderer_Face->GetPixelData().MulColor = Color;
		if (Color.g <= 1.0f)
		{
			RecoverTime += _DeltaTime;
		}
		
	}

	if (DT > 3.0f)
	{ 
		if (Hp <= 0)
		{
			// 보스 죽고 다음 스테이지
			return;
		}

		Renderer_Body->GetPixelData().MulColor = float4::ONE;
		Renderer_Face->GetPixelData().MulColor = float4::ONE;
		BossStateManager.ChangeState("Idle");
		return;
	}
}


// 무기 생성
void BossPsychoGiant::SpawnPortalsUp()
{
	// 랜덤한 중복되지 않는 숫자 뽑기
	std::vector<int> Indexs = { 0, 1, 2, 3, 4, 5, 6 };
	std::shuffle(Indexs.begin(), Indexs.end(), GameEngineRandom::MainRandom.GetFunc());

	float SpawnTime = 0.0f;
	for (int i : Indexs)
	{

		float4 SpawnPos = { 170 + i * 154.0f, -250, GetDepth(ACTOR_DEPTH::BOSSPORTAL) };
		Portals[i]->GetTransform().SetWorldPosition(SpawnPos);
		Portals[i]->On();
		Portals[i]->OnTimer(SpawnTime);
		Portals[i]->GetTransform().SetWorldRotation({ 0, 0, -90 });

		PortalKnife* Knife = GetLevel()->CreateActor<PortalKnife>();
		Knife->Spawn(SpawnPos + float4{0, 10, 0}, -90.05f, SpawnTime);
		SpawnTime += 0.15f;
	}
}

void BossPsychoGiant::SpawnPortalsDown()
{
	// 0번과 6번은 사용하지 않음(거리 멀어서)
	int NotUseIndex = GameEngineRandom::MainRandom.RandomInt(1, 5);

	for (int i = 1; i < 6; i++)
	{
		if (i == NotUseIndex)
		{
			continue;
		}
		Portals[i]->GetTransform().SetWorldPosition({ 170 + i * 154.0f, -820, GetDepth(ACTOR_DEPTH::BOSSPORTAL) });
		Portals[i]->On();
		Portals[i]->OnTimer(i * 0.15f);
		Portals[i]->GetTransform().SetWorldRotation({ 0, 0, 90.05f });

		float FloatDis = GameEngineRandom::MainRandom.RandomFloat(0.5f, 2.3f);
		PortalTurret* Turret = GetLevel()->CreateActor<PortalTurret>();
		Turret->Spawn({ 170 + i * 154.0f, -820, GetDepth(ACTOR_DEPTH::BOSSPORTAL) }, 100 * FloatDis, i * 0.15f);
	}

}

float4 LeftDown = float4(180, -750, GetDepth(ACTOR_DEPTH::BOSSPORTAL));
float4 RightDown = float4(930, -750, GetDepth(ACTOR_DEPTH::BOSSPORTAL));
float4 Origin = float4(LeftDown.x + RightDown.x / 2, -750, GetDepth(ACTOR_DEPTH::BOSSPORTAL));
float4 PortalVec = LeftDown - Origin;

void BossPsychoGiant::SpawnPortalsRound()
{
	int Dirs[3] = { 1, -1 , 2};
	int Idx = GameEngineRandom::MainRandom.RandomInt(0, 2);
	int Dir = Dirs[Idx];

	int StartIndex;
	int EndIndex;
	if (Dir == 1)
	{
		StartIndex = 0;
		EndIndex = 11;
	}
	else if (Dir == -1)
	{
		StartIndex = 10;
		EndIndex = -1;
	}
	else
	{
		StartIndex = 0;
		EndIndex = 11;
	}

	// 7개 -> 180 / 6 = 30도
	float SpawnTime = 0.0f;
	for (int Idx = StartIndex; Idx != EndIndex; Idx += Dir)
	{
		if (Idx < 0 || Idx >= 11)
		{
			break;
		}
		// 소환 각도
		float RotateDegree = Idx * (180 / 10.0f);
		float4 PortalPos = float4::VectorRotationToDegreeZAxis(PortalVec, -RotateDegree) + Origin;
		Portals[Idx]->GetTransform().SetWorldPosition(PortalPos);

		// 회전 각도
		float Rotate = float4::VectorXYtoDegree(PortalPos, Origin);
		Portals[Idx]->On();
		Portals[Idx]->OnTimer(SpawnTime);
		Portals[Idx]->GetTransform().SetWorldRotation({ 0, 0, Rotate });

		PortalKnife* Knife = GetLevel()->CreateActor<PortalKnife>();
		float4 SpawnPos = float4::VectorRotationToDegreeZAxis(PortalVec + float4{ -10, 0, 0, 0 }, -RotateDegree) + Origin;
		Knife->Spawn(SpawnPos, Rotate, SpawnTime);
		SpawnTime += 0.15f;
	}
}


