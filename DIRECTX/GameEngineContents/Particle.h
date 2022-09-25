#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class Particle : public GameEngineActor
{
public:
	// constrcuter destructer
	Particle();
	virtual ~Particle();

	// delete Function
	Particle(const Particle& _Other) = delete;
	Particle(Particle&& _Other) noexcept = delete;
	Particle& operator=(const Particle& _Other) = delete;
	Particle& operator=(Particle&& _Other) noexcept = delete;

	float4 Dir;
	float Speed;

protected:
	GameEngineTextureRenderer* Renderer;
};

