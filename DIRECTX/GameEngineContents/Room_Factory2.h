#pragma once
#include "Room.h"

// Ό³Έν :
class Room_Factory2 : public Room
{
public:
	// constrcuter destructer
	Room_Factory2();
	~Room_Factory2();

	// delete Function
	Room_Factory2(const Room_Factory2& _Other) = delete;
	Room_Factory2(Room_Factory2&& _Other) noexcept = delete;
	Room_Factory2& operator=(const Room_Factory2& _Other) = delete;
	Room_Factory2& operator=(Room_Factory2&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

	void Setting() override;
	void Clear() override;

private:

};

