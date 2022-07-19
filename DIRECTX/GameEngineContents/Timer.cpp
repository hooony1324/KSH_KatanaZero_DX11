#include "PreCompile.h"
#include "Timer.h"

void Timer::Start()
{
}

void Timer::Update(float _DeltaTime)
{
	if (false == IsStart)
	{
		return;
	}

	SumTime += _DeltaTime;

	if (SumTime >= SettingTime)
	{
		IsStart = false;
		SumTime = 0.0f;
	}
}

