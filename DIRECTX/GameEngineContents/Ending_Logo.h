#pragma once
#include <GameEngineCore/GameEngineActor.h>
// Ό³Έν :
class Ending_Logo : public GameEngineActor
{
public:
	// constrcuter destructer
	Ending_Logo();
	~Ending_Logo();

	// delete Function
	Ending_Logo(const Ending_Logo& _Other) = delete;
	Ending_Logo(Ending_Logo&& _Other) noexcept = delete;
	Ending_Logo& operator=(const Ending_Logo& _Other) = delete;
	Ending_Logo& operator=(Ending_Logo&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;
private:
	GameEngineTextureRenderer* Logo;
};

