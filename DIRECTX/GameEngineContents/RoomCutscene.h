#pragma once
#include "Room.h"

// Ό³Έν :
class RoomCutscene : public Room
{
public:
	// constrcuter destructer
	RoomCutscene();
	~RoomCutscene();

	// delete Function
	RoomCutscene(const RoomCutscene& _Other) = delete;
	RoomCutscene(RoomCutscene&& _Other) noexcept = delete;
	RoomCutscene& operator=(const RoomCutscene& _Other) = delete;
	RoomCutscene& operator=(RoomCutscene&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

	virtual void Setting() {}
	virtual void Clear() {}

private:

};

