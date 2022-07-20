#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class  GameEngineDefaultRenderer;
class GameEngineTextureRenderer;
class TitleButtons : public GameEngineActor
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

protected:
	void Start() override;
	void Update(float _DeltaTime)override;
	void End() override;

private:
	GameEngineDefaultRenderer* Renderer_Background;
	GameEngineTextureRenderer* Renderer_Buttons;
	GameEngineDefaultRenderer* Renderer_Selector;

	float4 Color_Background;
	float4 Color_Selector;

	// Lerp
	float SumDeltaTime;
	float4 Lerp_Start;
	float4 Lerp_Dest;
	float4 CurPos;
};

