#pragma once
#include <GameEngineCore/GameEngineGUI.h>

// Ό³Έν :
class PlayLevelGUI : public GameEngineGUIWindow
{
public:
	// constrcuter destructer
	PlayLevelGUI();
	~PlayLevelGUI();

	// delete Function
	PlayLevelGUI(const PlayLevelGUI& _Other) = delete;
	PlayLevelGUI(PlayLevelGUI&& _Other) noexcept = delete;
	PlayLevelGUI& operator=(const PlayLevelGUI& _Other) = delete;
	PlayLevelGUI& operator=(PlayLevelGUI&& _Other) noexcept = delete;

protected:

private:
	void OnGUI(GameEngineLevel* _Level, float _DeltaTime) override;
	void Initialize(class GameEngineLevel* _Level) override;
};

