#pragma once
#include <GameEngineCore/GameEngineActor.h>

// ���� :
class GameEngineTextureRenderer;
class Portal : public GameEngineActor
{
public:
	void OnTimer(float _WakeTime = 0.0f);

public:
	// constrcuter destructer
	Portal();
	~Portal();

	// delete Function
	Portal(const Portal& _Other) = delete;
	Portal(Portal&& _Other) noexcept = delete;
	Portal& operator=(const Portal& _Other) = delete;
	Portal& operator=(Portal&& _Other) noexcept = delete;

protected:
	void OnEvent() override;
	void OffEvent() override;

	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

	void AllAnimationBind();

private:
	float WakeTime;

	GameEngineTextureRenderer* Renderer_Base;
	GameEngineTextureRenderer* Renderer_OutLine;
	bool IsOpend;
	float LoopTime;

	GameEngineSoundPlayer SoundPlayer;

};

