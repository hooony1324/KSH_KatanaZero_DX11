#pragma once
#include <GameEngineCore/GameEngineActor.h>

// 설명 :
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

	void SetCameraClampArea(float4& _LeftTop, float4& _RightBottom);
	void PlayerSpawn(GameEngineActor* _Player);

protected:
	void EmptyRoomInit();
	void InitCameraClampArea();

protected:
	GameEngineTextureRenderer* Background;
	
	// 충돌맵(반투명 괜찮을듯)
	GameEngineTextureRenderer* Background_Collision;

	float4 CamClamp_LeftTop;
	float4 CamClamp_RightBottom;

	float4 PlayerSpawnPos; // 룸 마다 꼭 설정해주어야 함
private:

};

