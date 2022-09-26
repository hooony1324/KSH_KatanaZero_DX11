#pragma once
#include "Particle.h"
// Ό³Έν :
class NoDirBlood : public Particle
{
public:
	// constrcuter destructer
	NoDirBlood();
	~NoDirBlood();

	// delete Function
	NoDirBlood(const NoDirBlood& _Other) = delete;
	NoDirBlood(NoDirBlood&& _Other) noexcept = delete;
	NoDirBlood& operator=(const NoDirBlood& _Other) = delete;
	NoDirBlood& operator=(NoDirBlood&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

private:

};

