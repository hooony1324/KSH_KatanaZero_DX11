#include "PreCompile.h"
#include "SlowMotion.h"

void SlowMotion::SlowIn()
{
	Slow = true;
}

void SlowMotion::SlowOut()
{
	Slow = false;
}

SlowMotion::SlowMotion()
	: Renderer(nullptr)
	, Color(nullptr)
{
}

SlowMotion::~SlowMotion()
{
}

void SlowMotion::Start()
{
	Renderer = CreateComponent<GameEngineUIRenderer>();
	Renderer->SetTexture("spr_transition_square.png");
	Renderer->GetTransform().SetLocalScale({ 1280, 720, 1 });

	// 처음 배경 투명색
	Color = &Renderer->GetPixelData().MulColor;
	Color->a = 0;
	
	TimeRatio = 1;
	TimeRatio_Fan = 1;
}

void SlowMotion::Update(float _DeltaTime)
{

	// Shift땜
	if (false == Slow)
	{
		// 시간 원상복구
		TimeRatio = GameEngineMath::Lerp(1, 0.5f, _DeltaTime);
		TimeRatio_Fan = GameEngineMath::Lerp(1, 0.125f, _DeltaTime);


		// 배경 밝게
		if (Color->a <= 0.0f)
		{
			return;
		}
		else
		{
			Color->a -= _DeltaTime;
		}

		
	}
	// Shift눌림
	else
	{
		// 시간 느리게
		TimeRatio = GameEngineMath::Lerp(0.5f, 1, _DeltaTime);
		TimeRatio_Fan = GameEngineMath::Lerp(0.125f, 1, _DeltaTime);

		// 배경 어둡게
		if (Color->a >= 0.5f)
		{
			return;
		}
		else
		{
			Color->a += _DeltaTime;
		}
	}
	
	// TimeRatio : 그룹별로 느려지는 정도를 다르게 하고 싶다면
	GameEngineTime::GetInst()->SetTimeScale(static_cast<int>(ACTORGROUP::TIMEGROUP), TimeRatio);
	GameEngineTime::GetInst()->SetTimeScale(static_cast<int>(ACTORGROUP::TIMEGROUP_ENEMY), TimeRatio);
	GameEngineTime::GetInst()->SetTimeScale(static_cast<int>(ACTORGROUP::TIMEGROUP_BULLET), TimeRatio);
	GameEngineTime::GetInst()->SetTimeScale(static_cast<int>(ACTORGROUP::TIMEGROUP_FAN), TimeRatio_Fan);
}

void SlowMotion::End()
{
}
