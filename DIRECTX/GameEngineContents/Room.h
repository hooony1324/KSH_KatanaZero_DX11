#pragma once
#include <GameEngineCore/GameEngineActor.h>

// ¼³¸í :
class GameEngineTextureRenderer;
class Room : public GameEngineActor
{
public:
	// constrcuter destructer
	Room();
	virtual ~Room();

	// delete Function
	Room(const Room& _Other) = delete;
	Room(Room&& _Other) noexcept = delete;
	Room& operator=(const Room& _Other) = delete;
	Room& operator=(Room&& _Other) noexcept = delete;

	virtual void OnEvent() {}
	virtual void OffEvent() {}

	void GetCameraClampArea(float4& _LeftTop, float4& _RightBottom);

protected:
	void InitCameraClampArea();

protected:
	GameEngineTextureRenderer* Background;
	
	// Ãæµ¹¸Ê(¹ÝÅõ¸í ±¦ÂúÀ»µí)

	float4 CamClamp_LeftTop;
	float4 CamClamp_RightBottom;

private:

};

