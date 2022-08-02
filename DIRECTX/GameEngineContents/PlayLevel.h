#pragma once
#include <GameEngineCore/GameEngineLevel.h>

// ¼³¸í :
class Room;
class PlayerZero;
class Transition;
class PlayLevel : public GameEngineLevel
{
public:
	// constrcuter destructer
	PlayLevel();
	~PlayLevel();

	// delete Function
	PlayLevel(const PlayLevel& _Other) = delete;
	PlayLevel(PlayLevel&& _Other) noexcept = delete;
	PlayLevel& operator=(const PlayLevel& _Other) = delete;
	PlayLevel& operator=(PlayLevel&& _Other) noexcept = delete;

protected:
	void Start() override;
	void OnEvent() override;
	void Update(float _DeltaTime) override;
	void OffEvent() override {};
	void End() override;

// ·ë
private:
	void RoomChange(Room* _Room);
	void CameraFollow(float _DeltaTime);

private:
	Room* CurRoom;
	Room* Room1;
	Room* Room2;
	Room* Room3;

	GameEngineCameraActor* Cam;
	float4 CamClamp_LeftTop;
	float4 CamClamp_RightBottom;

	PlayerZero* Player;
	Transition* Effect_Transition;

// FSM : RoomChange, RoomPlay, RoomEnd
private:
	GameEngineStateManager LevelStateManager;


};

