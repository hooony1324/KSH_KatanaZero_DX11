#pragma once
#include "Room.h"

// ¼³¸í :
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
	void CutSceneSetting();

private:
	GameEngineStateManager StateManager;

	void IdleUpdate(float _DeltaTime, const StateInfo& _Info);

	void CutSceneStart(const StateInfo& _Info);
	void CutSceneUpdate(float _DeltaTime, const StateInfo& _Info);

	void RoarStart(const StateInfo& _Info);
	void RoarUpdate(float _DeltaTime, const StateInfo& _Info);

	void PlayStart(const StateInfo& _Info);
	void PlayUpdate(float _DeltaTime, const StateInfo& _Info);

	void DistortionStart(const StateInfo& _Info);
	void DistortionUpdate(float _DeltaTime, const StateInfo& _Info);

	void PlayerBlock();

	// SOUND
private:
	GameEngineSoundPlayer RoarSoundPlayer;

private:
	class BossPsychoGiant* BossGiant;

	GameContentsEffectRenderer* Background_Wave;
	GameEngineTextureRenderer* Background_Mid;
	GameEngineTextureRenderer* Background_Front;
	GameEngineTextureRenderer* Background_FrontRed;
	GameEngineTextureRenderer* Background_Floor;

	// ÄÆ¾À
private:
	GameEngineTextureRenderer* CutScene_Player;
	GameEngineTextureRenderer* CutScene_Boss;
	GameEngineTextureRenderer* CutScene_Back;

	GameEngineStateManager CutSceneStateManager;

	void SceneIdleUpdate(float _DeltaTime, const StateInfo& _Info);

	void SceneMutateStart(const StateInfo& _Info);
	void SceneMutateUpdate(float _DeltaTime, const StateInfo& _Info);

	void SceneDistortionStart(const StateInfo& _Info);
	void SceneDistortionUpdate(float _DeltaTime, const StateInfo& _Info);

	void SceneMutatedStart(const StateInfo& _Info);
	void SceneMutatedUpdate(float _DeltaTime, const StateInfo& _Info);

	bool WalkStart;
	float RedTime;

	// ±âÅ¸
private:
	// ÇÇ ³ª¹µÀÙ
	std::vector<class BloodLeaf*> BloodLeaves;

	// ¹°··¹Ù´Ú
	class BossFloor* Floor;
};

