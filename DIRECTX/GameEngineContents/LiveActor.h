#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class LiveActor : public GameEngineActor
{
public:
	// constrcuter destructer
	LiveActor();
	~LiveActor();

	// delete Function
	LiveActor(const LiveActor& _Other) = delete;
	LiveActor(LiveActor&& _Other) noexcept = delete;
	LiveActor& operator=(const LiveActor& _Other) = delete;
	LiveActor& operator=(LiveActor&& _Other) noexcept = delete;

protected:


private:

};

