#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include "GameContentsCustomRenderer.h"

// Ό³Έν :

class TestActor : public GameEngineActor
{
public:
	void SetNewTexture(GameEngineTexture* _Texture, float4 _Pos)
	{
		ShotRenderer->SetTexture(_Texture);
		ShotRenderer->GetTransform().SetWorldScale({ 100, 100, 1 });
		ShotRenderer->GetTransform().SetWorldPosition(_Pos);
	}

public:
	// constrcuter destructer
	TestActor();
	~TestActor();

	// delete Function
	TestActor(const TestActor& _Other) = delete;
	TestActor(TestActor&& _Other) noexcept = delete;
	TestActor& operator=(const TestActor& _Other) = delete;
	TestActor& operator=(TestActor&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;


private:
	class GameEngineTextureRenderer* ShotRenderer;
	class GameContentsCustomRenderer* TestRenderer;
	class GameContentsCustomRenderer* CameraRenderer;
	class GameContentsCustomRenderer* KnifeRenderer;
	class GameContentsCustomRenderer* MaskedRenderer;

	class GameEngineTextureRenderer* TextureRenderer;
};

