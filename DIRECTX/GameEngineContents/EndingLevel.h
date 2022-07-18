#pragma once
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineBase/GameEngineSound.h>

// Ό³Έν :
class GameEngineActor;
class EndingLevel : public GameEngineLevel
{
public:
	// constrcuter destructer
	EndingLevel();
	~EndingLevel();

	// delete Function
	EndingLevel(const EndingLevel& _Other) = delete;
	EndingLevel(EndingLevel&& _Other) noexcept = delete;
	EndingLevel& operator=(const EndingLevel& _Other) = delete;
	EndingLevel& operator=(EndingLevel&& _Other) noexcept = delete;

protected:
	void Start() override;
	void OnEvent() override;
	void Update(float _DeltaTime) override;
	void OffEvent() override;
	void End() override;
private:
	GameEngineActor* Logo;
	float4 Lerp_Start;
	float4 Lerp_Dest;
	float SumDeltaTime;

	GameEngineSoundPlayer SoundPlayer;
};

