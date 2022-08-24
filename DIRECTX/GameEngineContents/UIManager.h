#pragma once
#include <GameEngineCore/GameEngineActor.h>

// ���� :
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
	GameEngineUIRenderer* TimerHud;
	
	// ���� ũ��, ��ġ
	float4 ScaleTimerBar;
	
	std::vector< GameEngineUIRenderer*> SlowBatteries;
	int CurSlowBatteryIndex;
};

