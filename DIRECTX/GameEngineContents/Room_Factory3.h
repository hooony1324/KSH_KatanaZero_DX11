#pragma once
#include "Room.h"

// Ό³Έν :
class Room_Factory3 : public Room
{
public:
	// constrcuter destructer
	Room_Factory3();
	~Room_Factory3();

	// delete Function
	Room_Factory3(const Room_Factory3& _Other) = delete;
	Room_Factory3(Room_Factory3&& _Other) noexcept = delete;
	Room_Factory3& operator=(const Room_Factory3& _Other) = delete;
	Room_Factory3& operator=(Room_Factory3&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;
	void OnEvent() override;
	void OffEvent() override;

	void StairSetting() override;
private:

};

