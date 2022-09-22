#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class ChromakeyGreen : public GameEngineActor
{
public:
	static void ChromakeyOn()
	{
		Activate = true;
	}

	static void ChromakeyOff()
	{
		Activate = false;
	}


public:
	// constrcuter destructer
	ChromakeyGreen();
	~ChromakeyGreen();

	// delete Function
	ChromakeyGreen(const ChromakeyGreen& _Other) = delete;
	ChromakeyGreen(ChromakeyGreen&& _Other) noexcept = delete;
	ChromakeyGreen& operator=(const ChromakeyGreen& _Other) = delete;
	ChromakeyGreen& operator=(ChromakeyGreen&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

	void OnEvent() override;
	void OffEvent() override;

private:
	static bool Activate;
	GameEngineTextureRenderer* Renderer;

};

