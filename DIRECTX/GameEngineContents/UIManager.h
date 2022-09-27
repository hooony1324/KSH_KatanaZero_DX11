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
	}

	void SetInitialUIOn()
	{
		InitialUIUpdate = true;
	}

	void SetInitialUIOff()
	{
		InitialUIUpdate = false;
		for (auto UI : InitalRoomUI)
		{
			UI->Off();
		}
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
	
	// 최초 크기, 위치
	float4 ScaleTimerBar;
	
	std::vector< GameEngineUIRenderer*> SlowBatteries;
	int CurSlowBatteryIndex;

	// 룸 최초시작 UI
	bool InitialUIUpdate;
	std::vector<GameEngineUIRenderer*> InitalRoomUI;

	GameEngineUIRenderer* MouseClick;
	GameEngineUIRenderer* ClickToStart;
	
	GameEngineUIRenderer* SongTitleBG;
	GameEngineUIRenderer* PlayingSongText;
	GameEngineUIRenderer* PlayingSongTextBG;
	GameEngineUIRenderer* PlayingSongTextUnderline;
	GameEngineUIRenderer* SongTitleText;

	GameEngineUIRenderer* StageName;
	GameEngineUIRenderer* StageNameBG;

	GameEngineStateManager InitialUIManager;

	void SongTitleStart(const StateInfo& _Info);
	void SongTitleUpdate(float _DeltaTime, const StateInfo& _Info);

	void StageNameStart(const StateInfo& _Info);
	void StageNameUpdate(float _DeltaTime, const StateInfo& _Info);

	void Vibration(float _RendererSumTime, GameEngineUIRenderer& _Renderer);
	//float ShakeX = sinf(ShakeDT * 10.0f) * powf(0.98f, ShakeDT * 80.0f);
	//float ShakeY = sinf(ShakeDT * 80.0f) * powf(0.97f, ShakeDT * 80.0f);
	//GetMainCameraActor()->GetTransform().SetWorldMove({ 0, ShakeY * 10, 0 });
	//ShakeDT += _DeltaTime;

	AtlasData SongTitleBGData;
	AtlasData SongUnderlineData;
};

