#pragma once
#include <GameEngineCore/GameEngineLevel.h>

// ���� :
class Room;
class PlayerZero;
class Transition;
class UIManager;
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

// ��
private:
	void RoomChange(Room* _Room);
	void CameraFollow(float _DeltaTime);

private:
	UIManager* UI;
	Room* CurRoom;
	Room* Room1;
	Room* Room2;
	Room* Room3;
	std::list<Room*> Rooms;
	std::list<Room*>::iterator RoomIter;

	GameEngineCameraActor* Cam;
	float4 CamClamp_LeftTop;
	float4 CamClamp_RightBottom;

	PlayerZero* Player;
	Transition* Effect_Transition;

// FSM : RoomPlay, RoomReplay, RoomChange
private:
	GameEngineStateManager RoomStateManager;

	void RoomChangeStart(const StateInfo& _Info);
	void RoomChangeUpdate(float _DeltaTime, const StateInfo& _Info);
	void RoomChangeEnd(const StateInfo& _Info);

	void RoomPlayStart(const StateInfo& _Info);
	void RoomPlayUpdate(float _DeltaTime, const StateInfo& _Info);
	void RoomPlayEnd(const StateInfo& _Info);

	void RoomExitStart(const StateInfo& _Info);
	void RoomExitUpdate(float _DeltaTime, const StateInfo& _Info);
	void RoomExitEnd(const StateInfo& _Info);

	void RoomClickToRestartStart(const StateInfo& _Info);
	void RoomClickToRestartUpdate(float _DeltaTime, const StateInfo& _Info);

	// REVERSE
	void RoomReverseStart(const StateInfo& _Info);
	void RoomReverseUpdate(float _DeltaTime, const StateInfo& _Info);
	void RoomReverseEnd(const StateInfo& _Info);
};

