#include "PreCompile.h"
#include "SlowBackground.h"

void SlowBackground::FadeIn()
{
	Fade = true;
}

void SlowBackground::FadeOut()
{
	Fade = false;
}

SlowBackground::SlowBackground()
	: Renderer(nullptr)
{
}

SlowBackground::~SlowBackground() 
{
}

void SlowBackground::Start()
{
	Renderer = CreateComponent<GameEngineUIRenderer>();
	Renderer->SetTexture("spr_transition_square.png");
	Renderer->GetTransform().SetLocalScale({ 1280, 720, 1 });
	Color = Renderer->GetColorData();
	Renderer->GetColorData().MulColor.a = 0;
}

void SlowBackground::Update(float _DeltaTime)
{
	float4& Color = Renderer->GetColorData().MulColor;
	if (false == Fade)
	{
		if (Color.a <= 0.0f)
		{
			return;
		}
		else
		{
			Color.a -= _DeltaTime;
		}
	}
	else
	{
		if (Color.a >= 0.5f)
		{
			return;
		}
		else
		{
			Color.a += _DeltaTime;
		}
	}
}

void SlowBackground::End()
{
}
