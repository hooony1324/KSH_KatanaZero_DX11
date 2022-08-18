#pragma once
#include <GameEngineCore/GameEngineActor.h>

// 설명 :
class GameEngineTextureRenderer;
class UIManager : public GameEngineActor
{
public:
	inline void RestartUIOn()
	{
		RestartClickUI->On();
	}
	inline void RestartUIOff()
	{
		RestartClickUI->Off();
	}

	void SetTimeBarLength(float _Ratio)
	{
		//TimerBar->SetPivot(PIVOTMODE::LEFTTOP);
		float NewX = ScaleTimerBar.x * _Ratio;
		TimerBar->GetTransform().SetWorldScale({ NewX, ScaleTimerBar.y , ScaleTimerBar.z });
		TimerBar->GetTransform().SetLocalPosition({ PosTimerBar.x - (ScaleTimerBar.x - NewX) * 0.5f, PosTimerBar.y, PosTimerBar.z});
	}

public:
	// constrcuter destructer
	UIManager();
	~UIManager();

	// delete Function
	UIManager(const UIManager& _Other) = delete;
	UIManager(UIManager&& _Other) noexcept = delete;
	UIManager& operator=(const UIManager& _Other) = delete;
	UIManager& operator=(UIManager&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

private:
	GameEngineUIRenderer* RestartClickUI;
	GameEngineUIRenderer* TimerBar;
	
	// 최초 크기, 위치
	float4 ScaleTimerBar;
	float4 PosTimerBar;
	
	std::vector< GameEngineUIRenderer*> SlowBatteries;
	int CurSlowBatteryIndex;
};

