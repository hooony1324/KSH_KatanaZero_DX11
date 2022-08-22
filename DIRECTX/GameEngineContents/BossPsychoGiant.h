#pragma once
#include <GameEngineCore/GameEngineActor.h>

// 설명 :
class GameEngineTextureRenderer;
class BossPsychoGiant : public GameEngineActor
{
private:
	static BossPsychoGiant* GlobalInst;
	// 외부에서 보스 상태 변경
public:

	static BossPsychoGiant* GetInst()
	{
		return GlobalInst;
	}

	// void ChangeState

public:
	// constrcuter destructer
	BossPsychoGiant();
	~BossPsychoGiant();

	// delete Function
	BossPsychoGiant(const BossPsychoGiant& _Other) = delete;
	BossPsychoGiant(BossPsychoGiant&& _Other) noexcept = delete;
	BossPsychoGiant& operator=(const BossPsychoGiant& _Other) = delete;
	BossPsychoGiant& operator=(BossPsychoGiant&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

private:
	void AllStateCreate();

	void IdleStart(const StateInfo& _Info);
	void IdleUpdate(float _DeltaTime, const StateInfo& _Info);

	void TentacleAttackStart(const StateInfo& _Info);
	void TentacleAttackUpdate(float _DeltaTime, const StateInfo& _Info);
	
	// ATTACK : tentacle, 

	GameEngineStateManager BossStateManager;
private:
	void AllAnimationBind();

	GameEngineTextureRenderer* Renderer_Tentacle;
	GameEngineTextureRenderer* Renderer_Body;
	GameEngineTextureRenderer* Renderer_Face;
	GameEngineTextureRenderer* Renderer_Lilguy;

private:
	int Hp; // 5
};

