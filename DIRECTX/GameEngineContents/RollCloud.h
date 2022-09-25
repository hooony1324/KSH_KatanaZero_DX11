#pragma once
#include "Particle.h"

// Ό³Έν :


class RollCloud : public Particle
{
public:
	// constrcuter destructer
	RollCloud();
	~RollCloud();

	// delete Function
	RollCloud(const RollCloud& _Other) = delete;
	RollCloud(RollCloud&& _Other) noexcept = delete;
	RollCloud& operator=(const RollCloud& _Other) = delete;
	RollCloud& operator=(RollCloud&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

};

