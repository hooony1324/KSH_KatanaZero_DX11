#pragma once
#include "Room.h"

// ���� :
class GameEngineTextureRenderer;
class Room_Boss : public Room
{
public:
	// constrcuter destructer
	Room_Boss();
	~Room_Boss();

	// delete Function
	Room_Boss(const Room_Boss& _Other) = delete;
	Room_Boss(Room_Boss&& _Other) noexcept = delete;
	Room_Boss& operator=(const Room_Boss& _Other) = delete;
	Room_Boss& operator=(Room_Boss&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

	void OnEvent() override;
	void OffEvent() override;

	void StairSetting() override {}

private:
	class BossPsychoGiant* BossGiant;

	GameEngineTextureRenderer* Background_Mid;
	GameEngineTextureRenderer* Background_Front;
	GameEngineTextureRenderer* Background_Floor;
};

