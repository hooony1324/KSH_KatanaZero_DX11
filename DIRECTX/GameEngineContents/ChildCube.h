#pragma once
#include <GameEngineCore/GameEngineActor.h>


// Ό³Έν :
class ChildCube : public GameEngineActor
{
public:
	// constrcuter destructer
	ChildCube();
	~ChildCube();

	// delete Function
	ChildCube(const ChildCube& _Other) = delete;
	ChildCube(ChildCube&& _Other) noexcept = delete;
	ChildCube& operator=(const ChildCube& _Other) = delete;
	ChildCube& operator=(ChildCube&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

private:

};

