#include "PreCompile.h"
#include "BossPsychoGiant.h"
#include <GameEngineCore/CoreMinimal.h>

#include "TentacleKnife.h"

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

}

void BossPsychoGiant::AllStateCreate()
{
	BossStateManager.CreateStateMember("Idle"
		, std::bind(&BossPsychoGiant::IdleUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&BossPsychoGiant::IdleStart, this, std::placeholders::_1));

	BossStateManager.CreateStateMember("Stab"
		, std::bind(&BossPsychoGiant::StabAttackUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&BossPsychoGiant::StabAttackStart, this, std::placeholders::_1));

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


