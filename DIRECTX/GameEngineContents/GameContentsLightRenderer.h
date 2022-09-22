#pragma once
#include <GameEngineCore/GameEngineDefaultRenderer.h>

// Ό³Έν :
class GameContentsLightRenderer : public GameEngineDefaultRenderer
{
public:
	// constrcuter destructer
	GameContentsLightRenderer();
	~GameContentsLightRenderer();

	// delete Function
	GameContentsLightRenderer(const GameContentsLightRenderer& _Other) = delete;
	GameContentsLightRenderer(GameContentsLightRenderer&& _Other) noexcept = delete;
	GameContentsLightRenderer& operator=(const GameContentsLightRenderer& _Other) = delete;
	GameContentsLightRenderer& operator=(GameContentsLightRenderer&& _Other) noexcept = delete;


};

