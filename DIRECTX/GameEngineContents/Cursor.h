#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class GameEngineTextureRenderer;
class GameEngineUIRenderer;
class Cursor : public GameEngineActor
{
public:
	static const float4 GetCursorPosition()
	{
		return Position;
	}

	static const float4 GetCursorPositionToActor()
	{
		return PositionToActor;
	}

public:
	// constrcuter destructer
	Cursor();
	~Cursor();

	// delete Function
	Cursor(const Cursor& _Other) = delete;
	Cursor(Cursor&& _Other) noexcept = delete;
	Cursor& operator=(const Cursor& _Other) = delete;
	Cursor& operator=(Cursor&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

private:
	GameEngineUIRenderer* Renderer;
	static float4 Position;
	static float4 PositionToActor;
};

