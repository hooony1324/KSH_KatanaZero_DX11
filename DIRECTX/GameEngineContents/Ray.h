#pragma once
#include <GameEngineCore/GameEngineCollision.h>

// Ό³Έν :
class Ray : public GameEngineCollision
{
public:
	// constrcuter destructer
	Ray();
	~Ray();

	// delete Function
	Ray(const Ray& _Other) = delete;
	Ray(Ray&& _Other) noexcept = delete;
	Ray& operator=(const Ray& _Other) = delete;
	Ray& operator=(Ray&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

private:

};

