#include "PreCompile.h"
#include "BossPsychoGiant.h"
#include <GameEngineCore/CoreMinimal.h>

BossPsychoGiant* BossPsychoGiant::GlobalInst = nullptr;

BossPsychoGiant::BossPsychoGiant() 
{
	GlobalInst = this;
}

BossPsychoGiant::~BossPsychoGiant() 
{
}

void BossPsychoGiant::Start()
{
	Renderer_Body = CreateComponent<GameEngineTextureRenderer>();
	Renderer_Body->SetSamplingModePoint();
	Renderer_Body->CreateFrameAnimationFolder("body",
		FrameAnimation_DESC{ "psychoboss_body", 0.09f });
	Renderer_Body->GetTransform().SetLocalScale({ 675, 387, 1 });
	Renderer_Body->ChangeFrameAnimation("body");
	Renderer_Body->SetOrder(static_cast<int>(ACTORGROUP::TIMEGROUP));

	Renderer_Lilguy = CreateComponent<GameEngineTextureRenderer>();
	Renderer_Lilguy->SetSamplingModePoint();
	Renderer_Lilguy->CreateFrameAnimationFolder("lilguy",
		FrameAnimation_DESC{ "psychoboss_lilguy", 0.09f });
	Renderer_Lilguy->GetTransform().SetLocalScale({ 675, 387, 1 });
	Renderer_Lilguy->ChangeFrameAnimation("lilguy");
	Renderer_Lilguy->SetOrder(static_cast<int>(ACTORGROUP::TIMEGROUP));

	// ¾ó±¼
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

	// ÃË¼ö
	Renderer_Tentacle = CreateComponent<GameEngineTextureRenderer>();
	Renderer_Tentacle->SetSamplingModePoint();
	Renderer_Tentacle->CreateFrameAnimationFolder("tentacle_idle",
		FrameAnimation_DESC{ "psychoboss_tentacle_idle", 0.12f });
	Renderer_Tentacle->GetTransform().SetLocalScale({ 675, 387, 1 });

	Renderer_Tentacle->CreateFrameAnimationFolder("tentacle_stab",
		FrameAnimation_DESC{ "psychoboss_tentacle_stab", 0.12f, false});
	Renderer_Tentacle->GetTransform().SetLocalScale({ 675, 387, 1 });

	Renderer_Tentacle->CreateFrameAnimationFolder("tentacle_stabend",
		FrameAnimation_DESC{ "psychoboss_tentacle_stabend", 0.12f, false });
	Renderer_Tentacle->GetTransform().SetLocalScale({ 675, 387, 1 });

	Renderer_Tentacle->ChangeFrameAnimation("tentacle_idle");
	Renderer_Tentacle->SetOrder(static_cast<int>(ACTORGROUP::TIMEGROUP));

	AllAnimationBind();
	AllStateCreate();

	GetTransform().SetWorldScale({ 2, 2, 1 });
	Off();
}

void BossPsychoGiant::Update(float _DeltaTime)
{
	//BossStateManager.Update(_DeltaTime);

}

void BossPsychoGiant::End()
{
}

void BossPsychoGiant::AllStateCreate()
{
	BossStateManager.CreateStateMember("Idle"
		, std::bind(&BossPsychoGiant::IdleUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&BossPsychoGiant::IdleStart, this, std::placeholders::_1));
}

void BossPsychoGiant::AllAnimationBind()
{
	//Renderer_Face->AnimationBindEnd()

}

void BossPsychoGiant::IdleStart(const StateInfo& _Info)
{
	Renderer_Face->ChangeFrameAnimation("face_idle");
	Renderer_Tentacle->ChangeFrameAnimation("tentacle_idle");
}

void BossPsychoGiant::IdleUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (GameEngineInput::GetInst()->IsDown("F1"))
	{

	}
}

void BossPsychoGiant::TentacleAttackStart(const StateInfo& _Info)
{
}

void BossPsychoGiant::TentacleAttackUpdate(float _DeltaTime, const StateInfo& _Info)
{
}


