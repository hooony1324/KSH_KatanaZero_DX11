#pragma once
#include "Room.h"

// Ό³Έν :
class Room_Factory1 : public Room
{
public:
	// constrcuter destructer
	Room_Factory1();
	~Room_Factory1();

	// delete Function
	Room_Factory1(const Room_Factory1& _Other) = delete;
	Room_Factory1(Room_Factory1&& _Other) noexcept = delete;
	Room_Factory1& operator=(const Room_Factory1& _Other) = delete;
	Room_Factory1& operator=(Room_Factory1&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

private:

};

