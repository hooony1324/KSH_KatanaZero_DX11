#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class GameEngineTextureRenderer;
class GameEngineUIRenderer;
class Cursor : public GameEngineActor
{
public:
	static const float4 GetCursorScreenPosition()
	{
		return ScreenPosition;
	}

	static const float4 GetCursorWorldPosition()
	{
		return WorldPosition;
	}

	static const float4 GetCursorPositionToActor()
	{
		return WorldPositionToActor;
	}

	static bool IsClickAble()
	{
		return CursorValid;
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
	void CursorVisibleOutScreen();

private:
	GameEngineUIRenderer* Renderer;
	static float4 ScreenPosition;
	static float4 WorldPosition;
	static float4 WorldPositionToActor;

	float4 Boundary;
	static bool CursorValid;
};

