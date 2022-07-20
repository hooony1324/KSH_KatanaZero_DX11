#pragma once
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineBase/GameEngineSound.h>
// Ό³Έν :
class TitleLevel : public GameEngineLevel
{
public:
	// constrcuter destructer
	TitleLevel();
	~TitleLevel();

	// delete Function
	TitleLevel(const TitleLevel& _Other) = delete;
	TitleLevel(TitleLevel&& _Other) noexcept = delete;
	TitleLevel& operator=(const TitleLevel& _Other) = delete;
	TitleLevel& operator=(TitleLevel&& _Other) noexcept = delete;


protected:
	void Start() override;
	void OnEvent() override;
	void Update(float _DeltaTime) override;
	void OffEvent() override;
	void End() override;
private:
	void PlayLevelChangeCheck();
	void LerpOut();

private:
	std::vector<GameEngineActor*> AllActors;

	bool PlayLevelChange;
	GameEngineSoundPlayer SoundPlayer_BGM;
	GameEngineSoundPlayer SoundPlayer_Transition;


	// Change Effect
private:
	bool ChangeEnd;
	bool Changing;
	// Lerp
	float SumDeltaTime;
	std::vector<float4> ActorsCurPos;
	std::vector<float4> ActorsDestPos;
};

