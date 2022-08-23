#pragma once
#include <GameEngineCore/GameEngineActor.h>

// ���� :
enum class PSYCHOGIANT_PATTERN
{
	IDLE,
	SELECT,
	STAB,
	SPAWN_KNIFE1,
	SPAWN_KNIFE2,
	SPAWN_TURRET,
	SPAWN_PSYCHO,
	HURT,
	MAX,
};

class GameEngineTextureRenderer;
class BossPsychoGiant : public GameEngineActor
{
private:
	static BossPsychoGiant* GlobalInst;
	// �ܺο��� ���� ���� ����
public:
	PSYCHOGIANT_PATTERN Pattern;
	static BossPsychoGiant* GetInst()
	{
		return GlobalInst;
	}

	void ChangePattern(PSYCHOGIANT_PATTERN _Pattern)
	{
		switch (_Pattern)
		{
		case PSYCHOGIANT_PATTERN::IDLE:
			BossStateManager.ChangeState("Idle");
			break;
		case PSYCHOGIANT_PATTERN::STAB:
			BossStateManager.ChangeState("Stab");
			break;
		case PSYCHOGIANT_PATTERN::SPAWN_KNIFE1:
			break;
		case PSYCHOGIANT_PATTERN::SPAWN_KNIFE2:
			break;
		case PSYCHOGIANT_PATTERN::SPAWN_TURRET:
			break;
		case PSYCHOGIANT_PATTERN::SPAWN_PSYCHO:
			break;
		case PSYCHOGIANT_PATTERN::HURT:
			break;
		default:
			break;
		}

		Pattern = _Pattern;
	}

	void SetSelectOn()
	{
		IsSelectPattern = true;
	}

	void SetSelectOff()
	{
		IsSelectPattern = false;
	}

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

	void OnEvent() override;
	void OffEvent() override;

private:
	GameEngineStateManager BossStateManager;
	void AllStateCreate();

	void IdleStart(const StateInfo& _Info);
	void IdleUpdate(float _DeltaTime, const StateInfo& _Info);

	void StabAttackStart(const StateInfo& _Info);
	void StabAttackUpdate(float _DeltaTime, const StateInfo& _Info);
	
	// ����
	class TentacleKnife* Stabber;

private:
	void AllAnimationBind();

	GameEngineTextureRenderer* Renderer_Tentacle;
	GameEngineTextureRenderer* Renderer_Body;
	GameEngineTextureRenderer* Renderer_Face;
	GameEngineTextureRenderer* Renderer_Lilguy;


private:
	int Hp; // 5
	bool IsSelectPattern; // ������ �ִ´� or ���� ���� ����
};
