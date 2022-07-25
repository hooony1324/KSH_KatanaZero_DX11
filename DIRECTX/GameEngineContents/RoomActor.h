#pragma once
#include <GameEngineCore/GameEngineActor.h>

// ���� : Player, Enemy �θ� Ŭ����
class RoomActor : public GameEngineActor
{
public:
	// constrcuter destructer
	RoomActor();
	~RoomActor();

	// delete Function
	RoomActor(const RoomActor& _Other) = delete;
	RoomActor(RoomActor&& _Other) noexcept = delete;
	RoomActor& operator=(const RoomActor& _Other) = delete;
	RoomActor& operator=(RoomActor&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

private:

};

