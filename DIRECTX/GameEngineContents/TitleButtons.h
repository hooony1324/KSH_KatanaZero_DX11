#pragma once
#include "TitleUI.h"
#include <GameEngineBase/GameEngineSound.h>

// ���� :
class  GameEngineDefaultRenderer;
class GameEngineTextureRenderer;
class TitleButtons : public TitleUI
{
public:
	// constrcuter destructer
	TitleButtons();
	~TitleButtons();

	// delete Function
	TitleButtons(const TitleButtons& _Other) = delete;
	TitleButtons(TitleButtons&& _Other) noexcept = delete;
	TitleButtons& operator=(const TitleButtons& _Other) = delete;
	TitleButtons& operator=(TitleButtons&& _Other) noexcept = delete;

	inline bool CheckPlayLevelChange()
	{
		return IsPlayLevelChange;
	}

protected:
	void Start() override;
	void OnEvent() override;
	void Update(float _DeltaTime)override;
	void End() override;

private:
	void CheckInput();

private:
	GameEngineTextureRenderer* Renderer_Background;
	GameEngineTextureRenderer* Renderer_Buttons;
	GameEngineTextureRenderer* Renderer_Selector;

	// Selector
	std::vector<float4> ButtonsPos;
	int ButtonIndex;

	GameEngineSoundPlayer SoundPlayer;

	// LevelChange
	bool IsPlayLevelChange;
};

