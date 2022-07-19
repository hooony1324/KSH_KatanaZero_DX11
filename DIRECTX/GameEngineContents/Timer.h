#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class Timer : public GameEngineComponent
{
public:
	void Init(float _SettingTime)
	{
		SettingTime = _SettingTime;
		IsStart = false;
	}

	void Activate()
	{
		IsStart = true;
		SumTime = 0.0f;
	}

	bool IsCoolTime()
	{
		if (true == IsStart && SumTime >= 0.0f)
		{
			return true;
		}

		return false;
	}


public:
	// constrcuter destructer
	Timer() : IsStart(false), SumTime(0.0f), SettingTime(0.0f) {}
	~Timer() {}

	// delete Function
	Timer(const Timer& _Other) = delete;
	Timer(Timer&& _Other) noexcept = delete;
	Timer& operator=(const Timer& _Other) = delete;
	Timer& operator=(Timer&& _Other) noexcept = delete;
	
protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override {}

private:
	bool IsStart;
	float SettingTime;
	float SumTime;
};

