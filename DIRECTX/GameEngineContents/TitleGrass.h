#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class GameEngineDefaultRenderer;
class TitleGrass : public GameEngineActor
{
public:
	// constrcuter destructer
	TitleGrass();
	~TitleGrass();

	// delete Function
	TitleGrass(const TitleGrass& _Other) = delete;
	TitleGrass(TitleGrass&& _Other) noexcept = delete;
	TitleGrass& operator=(const TitleGrass& _Other) = delete;
	TitleGrass& operator=(TitleGrass&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

private:
	// 640 x 120 : spr_title_grass_0
	GameEngineDefaultRenderer* Renderer;
};

