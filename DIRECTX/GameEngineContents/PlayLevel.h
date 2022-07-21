#pragma once
#include <GameEngineCore/GameEngineLevel.h>

// ¼³¸í :
class Room;
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
	void OnEvent() override {};
	void Update(float _DeltaTime) override;
	void OffEvent() override {};
	void End() override;

private:
	// ·ë º¯°æ
	void RoomChange(Room* _Room);

private:
	// ·ë º¯°æ
	Room* CurRoom;
	Room* Room1;
	Room* Room2;
	Room* Room3;

	// FSM
private:
	enum class STATE
	{

	};


};

