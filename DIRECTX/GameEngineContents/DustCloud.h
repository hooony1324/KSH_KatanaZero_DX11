#pragma once
#include "Particle.h"

// Ό³Έν :
class DustCloud : public Particle
{
public:
	// constrcuter destructer
	DustCloud();
	~DustCloud();

	// delete Function
	DustCloud(const DustCloud& _Other) = delete;
	DustCloud(DustCloud&& _Other) noexcept = delete;
	DustCloud& operator=(const DustCloud& _Other) = delete;
	DustCloud& operator=(DustCloud&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

private:

};

