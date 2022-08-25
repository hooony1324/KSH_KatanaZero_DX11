#include "PreCompile.h"
#include "BossPsychoGiant.h"
#include <GameEngineCore/CoreMinimal.h>

#include "TentacleKnife.h"
#include "Portal.h"

BossPsychoGiant* BossPsychoGiant::GlobalInst = nullptr;

BossPsychoGiant::BossPsychoGiant() 
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

	// 포탈
	for (int i = 0; i < 7; i++)
	{
		Portal* PortalAttack = GetLevel()->CreateActor<Portal>();
		PortalAttack->Off();
		// 아래를 향함
		PortalAttack->GetTransform().SetWorldRotation({ 0,0, 90 });
		Portals.push_back(PortalAttack);
	}


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

	BossStateManager.CreateStateMember("Hurt"
		, std::bind(&BossPsychoGiant::HurtUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&BossPsychoGiant::HurtStart, this, std::placeholders::_1));

}

void BossPsychoGiant::AllAnimationBind()
{
	Renderer_Tentacle->AnimationBindEnd("tentacle_stabend",
		[=](const FrameAnimation_DESC& _Info)
		{ Renderer_Tentacle->ChangeFrameAnimation("tentacle_idle"); });

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

}


bool StabberAttackStart;
void BossPsychoGiant::StabAttackStart(const StateInfo& _Info)
{
	Pattern = PSYCHOGIANT_PATTERN::STAB;
	Renderer_Tentacle->ChangeFrameAnimation("tentacle_stab");


	Stabber->SetAttackEnd(false);
	StabberAttackStart = false;
}

void BossPsychoGiant::StabAttackUpdate(float _DeltaTime, const StateInfo& _Info)
{
	// takeout 완료 시 공격 시작
	if (true == Stabber->IsTakeOutEnd() && false == StabberAttackStart)
	{
		StabberAttackStart = true;
		Stabber->StabAttackStart(3);
	}

	if (true == Stabber->IsStabAttackEnd())
	{
		Stabber->SetTakeOutEnd(false);
		Renderer_Tentacle->ChangeFrameAnimation("tentacle_stabend");
		BossStateManager.ChangeState("Idle");
	}

}

void BossPsychoGiant::SpawnKnife1Start(const StateInfo& _Info)
{
	SpawnPortalsUp();
	// 포탈이 생성되면 포탈이 자동 공격하는 방향으로..
}

void BossPsychoGiant::SpawnKnife1Update(float _DeltaTime, const StateInfo& _Info)
{
	// 0 ~ 6 랜덤 시간차 생성..

	if (_Info.StateTime > 4.0f)
	{
		BossStateManager.ChangeState("Idle");
	}
}

void BossPsychoGiant::SpawnKnife2Start(const StateInfo& _Info)
{
	SpawnPortalsRound();
}

void BossPsychoGiant::SpawnKnife2Update(float _DeltaTime, const StateInfo& _Info)
{
	if (_Info.StateTime > 4.0f)
	{
		BossStateManager.ChangeState("Idle");
	}
}

void BossPsychoGiant::SpawnTurretStart(const StateInfo& _Info)
{
	SpawnPortalsDown();
}

void BossPsychoGiant::SpawnTurretUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (_Info.StateTime > 4.0f)
	{
		BossStateManager.ChangeState("Idle");
	}
}

// 붉게되면서 화면 흔들기 ~초
float RecoverTime;
const float CamShakeSpeed = 10.0f;
void BossPsychoGiant::HurtStart(const StateInfo& _Info)
{
	// 색
	RecoverTime = 0;
	Renderer_Body->GetColorData().MulColor = float4::RED;
	Renderer_Face->GetColorData().MulColor = float4::RED;

	Renderer_Face->ChangeFrameAnimation("face_hurt");
}

void BossPsychoGiant::HurtUpdate(float _DeltaTime, const StateInfo& _Info)
{
	float DT = _Info.StateTime;

	// 카메라 흔들림
	if (DT <= 2.0f)
	{
		float ShakeX = sinf(DT * 10.0f * CamShakeSpeed) * powf(0.94f, DT * CamShakeSpeed);
		float ShakeY = sinf(DT * 10.0f * CamShakeSpeed) * powf(0.94f, DT * CamShakeSpeed);
		GetLevel()->GetMainCameraActor()->GetTransform().SetWorldMove({ ShakeX * 20, ShakeY * 20, 0 });
	}

	if (DT > 1.5f)
	{
		// 색 점점 원상복구
		float4 Color = float4::Lerp(float4::RED, float4::ONE, RecoverTime);
		Renderer_Body->GetColorData().MulColor = Color;
		Renderer_Face->GetColorData().MulColor = Color;
		if (Color.g <= 1.0f)
		{
			RecoverTime += _DeltaTime;
		}
		
	}

	if (DT > 3.0f)
	{ 
		Renderer_Body->GetColorData().MulColor = float4::ONE;
		Renderer_Face->GetColorData().MulColor = float4::ONE;
		BossStateManager.ChangeState("Idle");
		return;
	}
}


// 무기 생성
void BossPsychoGiant::SpawnPortalsUp()
{
	for (int i = 0; i < 7; i++)
	{
		Portals[i]->GetTransform().SetWorldPosition({ 170 + i * 154.0f, -250, GetDepth(ACTOR_DEPTH::BOSSPORTAL) });
		Portals[i]->GetTransform().SetWorldRotation({ 0, 0, 90 });
		Portals[i]->On();
	}
}

void BossPsychoGiant::SpawnPortalsDown()
{
	// 0번과 6번은 사용하지 않음
	for (int i = 1; i < 6; i++)
	{
		Portals[i]->GetTransform().SetWorldPosition({ 170 + i * 154.0f, -820, GetDepth(ACTOR_DEPTH::BOSSPORTAL) });
		Portals[i]->GetTransform().SetWorldRotation({ 0, 0, -90 });
		Portals[i]->On();
	}
}

float4 LeftDown = float4(180, -730, GetDepth(ACTOR_DEPTH::BOSSPORTAL));
float4 RightDown = float4(930, -730, GetDepth(ACTOR_DEPTH::BOSSPORTAL));
float4 Origin = float4(LeftDown.x + RightDown.x / 2, -730, GetDepth(ACTOR_DEPTH::BOSSPORTAL));
float4 PortalVec = LeftDown - Origin;
void BossPsychoGiant::SpawnPortalsRound()
{
	// 7개 -> 180 / 6 = 30도
	for (int i = 0; i < 7; i++)
	{
		float RotateDegree = i * -30.0f;
		float4 RotatePos = float4::VectorRotationToDegreeZAxis(PortalVec, RotateDegree);
		Portals[i]->GetTransform().SetWorldPosition(RotatePos + Origin);
		Portals[i]->GetTransform().SetWorldRotation({ 0, 0, RotateDegree });
		Portals[i]->On();
	}
}


