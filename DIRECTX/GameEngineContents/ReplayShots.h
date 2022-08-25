#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class ReplayShots : public GameEngineActor
{
public:
	void AddScreenShot(GameEngineTexture* _Texture);
	void SetScreenIndex(int _Index);

	void PlayReverse(float _PlaySpeed);

public:
	// constrcuter destructer
	ReplayShots();
	~ReplayShots();

	// delete Function
	ReplayShots(const ReplayShots& _Other) = delete;
	ReplayShots(ReplayShots&& _Other) noexcept = delete;
	ReplayShots& operator=(const ReplayShots& _Other) = delete;
	ReplayShots& operator=(ReplayShots&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

	void OnEvent() override;
	void OffEvent() override;

private:
	GameEngineUIRenderer* BaseRenderer;
	GameEngineUIRenderer* ScreenRenderer;
	std::vector<GameEngineTexture*> Shots;

	bool ReplayStart;
	int PlayDir; // 1, -1
	int CurIndex;
	float PlaySpeed;
	float FrameTime;
};

