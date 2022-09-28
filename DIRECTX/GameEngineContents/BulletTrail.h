#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class BulletTrail : public GameEngineActor
{
public:
	void Spawn(float4 _SpawnPos, float4 _SpawnScale, float _DeathTime)
	{
		GetTransform().SetWorldPosition(_SpawnPos);
		Renderer->GetTransform().SetWorldScale(_SpawnScale);
		Death(_DeathTime);
	}

public:
	// constrcuter destructer
	BulletTrail();
	~BulletTrail();

	// delete Function
	BulletTrail(const BulletTrail& _Other) = delete;
	BulletTrail(BulletTrail&& _Other) noexcept = delete;
	BulletTrail& operator=(const BulletTrail& _Other) = delete;
	BulletTrail& operator=(BulletTrail&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

private:
	GameEngineTextureRenderer* Renderer;
};

