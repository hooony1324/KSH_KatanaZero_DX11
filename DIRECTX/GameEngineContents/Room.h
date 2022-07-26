#pragma once
#include <GameEngineCore/GameEngineActor.h>

// ���� :
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

	virtual void Setting() {}
	virtual void Clear() {}

	void GetCameraClampArea(float4& _LeftTop, float4& _RightBottom);

protected:
	void RoomInit();
	void InitCameraClampArea();

protected:
	GameEngineTextureRenderer* Background;
	
	// �浹��(������ ��������)
	GameEngineTextureRenderer* Background_Collision;

	float4 CamClamp_LeftTop;
	float4 CamClamp_RightBottom;

private:

};

