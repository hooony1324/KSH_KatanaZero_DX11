#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class GameEngineTextureRenderer;
class Portal : public GameEngineActor
{
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
	GameEngineTextureRenderer* Renderer_Base;
	GameEngineTextureRenderer* Renderer_OutLine;
	
};

