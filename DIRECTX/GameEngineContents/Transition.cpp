#include "PreCompile.h"
#include "Transition.h"
#include <GameEngineCore/CoreMinimal.h>

const float FadeInterTime = 0.25f;
const float ParticleSize = 16.0f * 2.f;

void Transition::FadeIn()
{
	if (false == IsUpdate())
	{
		On();
	}
	ChangeState(STATE::FADEIN);
}

void Transition::FadeOut()
{
	if (false == IsUpdate())
	{
		On();
	}
	ChangeState(STATE::FADEOUT);
}

Transition::Transition()
{
}

Transition::~Transition() 
{
}

void Transition::Start()
{
	// 해상도에 맞게 트렌지션 채움
	float4 ScreenSize = { 1280, 720 };
	YNUM = static_cast<int>(ScreenSize.y / ParticleSize);
	XNUM = static_cast<int>(ScreenSize.x / ParticleSize);

	TransitionParticles =
		std::vector<std::vector<GameEngineUIRenderer*>>(YNUM, std::vector<GameEngineUIRenderer*>(XNUM, nullptr));
	for (int y = 0; y < YNUM; ++y)
	{
		for (int x = 0; x < XNUM; ++x)
		{
			GameEngineUIRenderer* Particle = CreateComponent<GameEngineUIRenderer>();
			Particle->SetTexture("spr_transition_square.png");
			Particle->SetSamplingModePoint();
			Particle->GetTransform().SetLocalScale({ ParticleSize, ParticleSize, 1 });
			Particle->CreateFrameAnimationFolder("in", FrameAnimation_DESC{ "transition_in", 0.0125f, false });
			Particle->CreateFrameAnimationFolder("idle", FrameAnimation_DESC{ "transition_idle", 2.0f });
			Particle->CreateFrameAnimationFolder("out", FrameAnimation_DESC{ "transition_out", 0.0125f, false });
			Particle->GetTransform().SetWorldPosition({ ParticleSize / 2.0f + x * ParticleSize, -(ParticleSize / 2.0f + y * ParticleSize), 0});
			Particle->ChangeFrameAnimation("idle");
			TransitionParticles[y][x] = Particle;
		}
	}
	
	
	TransitionParticles[0][XNUM - 1]->AnimationBindStart("in", [=](const FrameAnimation_DESC&) { TranstionEnd = false; });
	TransitionParticles[0][XNUM - 1]->AnimationBindStart("out", 
		[=](const FrameAnimation_DESC&) 
		{ 
			TranstionEnd = true;
			State = STATE::NONE;
			Off();
		});

	TransitionParticles[YNUM-1][0]->AnimationBindEnd("in", [=](const FrameAnimation_DESC&) { TranstionEnd = true; });
	TransitionParticles[YNUM-1][0]->AnimationBindEnd("out",
		[=](const FrameAnimation_DESC&)
		{
			TranstionEnd = true;
			State = STATE::NONE;
			Off();
		});

	// 켜두면 프레임 20나옴
	Off();
	State = STATE::NONE;
}

void Transition::Update(float _DeltaTime)
{
	if (State == STATE::NONE)
	{
		return;
	}

	UpdateState(_DeltaTime);
}

void Transition::End()
{
}


void Transition::FadeInStart()
{
	XIndex = XNUM - 1;
	SumDeltaTime = 0.0f;
}

void Transition::FadeInUpdate(float _DeltaTime)
{
	if (XIndex < 0)
	{
		return;
	}

	//SumDeltaTime += _DeltaTime;
	//if (SumDeltaTime < ParticleInterTime)
	//{
	//	return;
	//}
	//SumDeltaTime = 0.0f;

	for (int y = 0; y < YNUM; ++y)
	{
		TransitionParticles[y][XIndex]->On();
		TransitionParticles[y][XIndex]->ChangeFrameAnimation("in");
	}
	
	if (XIndex >= 0)
	{
		--XIndex;
	}

}

void Transition::FadeOutStart()
{
	XIndex = XNUM - 1;
	SumDeltaTime = 0.0f;
	StartAbleDeltaTime = 0.0f;
}

void Transition::FadeOutUpdate(float _DeltaTime)
{
	if (XIndex < 0)
	{
		return;
	}

	if (StartAbleDeltaTime < FadeInterTime)
	{
		StartAbleDeltaTime += _DeltaTime;
		return;
	}

	for (int y = 0; y < YNUM; ++y)
	{
		TransitionParticles[y][XIndex]->ChangeFrameAnimation("out");
	}

	if (XIndex >= 0)
	{
		--XIndex;
	}
}



void Transition::UpdateState(float _DeltaTime)
{
	switch (State)
	{
	case Transition::STATE::NONE:
		return;
	case Transition::STATE::FADEIN:
		FadeInUpdate(_DeltaTime);
		break;
	case Transition::STATE::FADEOUT:
		FadeOutUpdate(_DeltaTime);
		break;
	default:
		break;
	}
}

void Transition::ChangeState(STATE _State)
{
	switch (_State)
	{
	case Transition::STATE::FADEIN:
		FadeInStart();
		break;
	case Transition::STATE::FADEOUT:
		FadeOutStart();
		break;
	default:
		break;
	}

	State = _State;
}