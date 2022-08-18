#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class TestActor : public GameEngineActor
{
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
	class GameEngineTextureRenderer* TRenderer;
	class GameEngineDefaultRenderer* DRenderer;


};

