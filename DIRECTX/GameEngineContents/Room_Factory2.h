#pragma once
#include "Room.h"

// ���� :
class Fan;
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

	void OnEvent() override;
	void OffEvent() override;

	void StairSetting() override {};

private:
	Fan* PtrFan;
	GameEngineTextureRenderer* FanGuard;

};

