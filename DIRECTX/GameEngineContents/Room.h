#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class GameEngineTextureRenderer;
class Room : public GameEngineActor
{
public:
	// constrcuter destructer
	Room();
	~Room();

	// delete Function
	Room(const Room& _Other) = delete;
	Room(Room&& _Other) noexcept = delete;
	Room& operator=(const Room& _Other) = delete;
	Room& operator=(Room&& _Other) noexcept = delete;

	void RoomOn();
	void RoomOff();

protected:
	GameEngineTextureRenderer* Background;

private:

};

