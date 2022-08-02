#include "PreCompile.h"
#include "Transition.h"
#include <GameEngineCore/CoreMinimal.h>

float ParticleInterTime = 0.2f;
float FadeInterTime = 2.0f;

void Transition::Activate()
{
	On();
	ChangeState(STATE::FADEIN);
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
	int YNUM = ScreenSize.y / 16;
	int XNUM = ScreenSize.x / 16;
	TransitionParticles = std::vector<std::vector<GameEngineUIRenderer*>>(YNUM, std::vector<GameEngineUIRenderer*>());

	for (int y = 0; y < YNUM; y++)
	{
		for (int x = 0; x < XNUM; x++)
		{
			GameEngineUIRenderer* Square = CreateComponent<GameEngineUIRenderer>();
			Square->SetTexture("spr_transition_square.png");
			Square->ScaleToTexture();
			Square->CreateFrameAnimationFolder("in", FrameAnimation_DESC{ "transition_in", 0.05f, false });
			Square->CreateFrameAnimationFolder("idle", FrameAnimation_DESC{ "transition_idle", 0.05f, false });
			Square->CreateFrameAnimationFolder("out", FrameAnimation_DESC{ "transition_out", 0.05f, false });
			Square->ChangeFrameAnimation("idle");
			Square->GetTransform().SetWorldPosition({ 8.0f + (x * 16.0f), -8.0f - (y * 16.0f) });
			//Square->Off();
			TransitionParticles[y].push_back(Square);
		}
	}

	// 켜두면 프레임 20나옴
	Off();
	ChangeState(STATE::NONE);
}

void Transition::Update(float _DeltaTime)
{
}

void Transition::End()
{
}


void Transition::FadeInStart()
{
	XIndex = XNUM - 1; // 79
}

void Transition::FadeInUpdate(float _DeltaTime)
{
	if (XIndex < 0)
	{
		ChangeState(STATE::FADEOUT);
	}

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
}

void Transition::FadeOutUpdate(float _DeltaTime)
{
	SumDeltaTime += _DeltaTime;
	if (SumDeltaTime < FadeInterTime)
	{
		return;
	}

	if (XIndex < 0)
	{
		Off();
		for (int y = 0; y < YNUM; y++)
		{
			for (int x = 0; x < XNUM; x++)
			{
				TransitionParticles[y][x]->Off();
				TransitionParticles[y][x]->ChangeFrameAnimation("idle");
			}
		}
		
		ChangeState(STATE::NONE);
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
	if (_State == STATE::NONE)
	{
		return;
	}

	if (State != _State)
	{
		switch (State)
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
	}

	State = _State;
}