#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class PointLight : public GameEngineActor
{
public:
	// constrcuter destructer
	PointLight();
	~PointLight();

	// delete Function
	PointLight(const PointLight& _Other) = delete;
	PointLight(PointLight&& _Other) noexcept = delete;
	PointLight& operator=(const PointLight& _Other) = delete;
	PointLight& operator=(PointLight&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

	void OnEvent() override;
	void OffEvent() override;
private:
	GameEngineTextureRenderer* Renderer;
	GameEngineDefaultRenderer* DRenderer;
	GameContentsLightRenderer* LightRenderer;

};

