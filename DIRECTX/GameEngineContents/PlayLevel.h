#pragma once
#include <GameEngineCore/GameEngineLevel.h>

// 설명 :
class Room;
class Transition;
class UIManager;
class PlayLevel : public GameEngineLevel
{
public:
	// ControlGUI용
	static PlayLevel* PlayLevelInst;

	void ChangeRoom(int _Index);

	int GetRoomsSize()
	{
		return static_cast<int>(Rooms.size());
	}

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
	void Update(float _DeltaTime) override;
	void End() override;

	virtual void LevelStartEvent() override;
	virtual void LevelEndEvent() {};

// 룸
private:
	void CameraFollow(float _DeltaTime);

private:

	UIManager* UI;
	Room* CurRoom;
	Room* PrevRoom;
	Room* Room1;
	Room* Room2;
	Room* Room3;
	Room* Room4;
	std::vector<Room*> Rooms;

	//GameEngineCameraActor* Cam;
	float4 CamClamp_LeftTop;
	float4 CamClamp_RightBottom;

	class CharacterActor* Player;
	Transition* Transition_DiamondChange;

	class SlowMotion* SlowEffect;
	class GameEngineSoundPlayer SlowInSound;
	class GameEngineSoundPlayer SlowOutSound;

	class DiamondTransition* Transition;

	// 역재생
	std::list<class LiveActor*> CaptureGroup;


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

	// SLOW
	void RoomSlowStart(const StateInfo& _Info);
	void RoomSlowUpdate(float _DeltaTime, const StateInfo& _Info);
	void RoomSlowEnd(const StateInfo& _Info);

	// 진동
	void RoomShakeStart(const StateInfo& _Info);
	void RoomShakeUpdate(float _DeltaTime, const StateInfo& _Info);

	// REVERSE
	void RoomReverseStart(const StateInfo& _Info);
	void RoomReverseUpdate(float _DeltaTime, const StateInfo& _Info);
	void RoomReverseEnd(const StateInfo& _Info);
};

