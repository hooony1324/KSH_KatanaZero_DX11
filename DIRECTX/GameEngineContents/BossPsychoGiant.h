#pragma once
#include <GameEngineCore/GameEngineActor.h>

// 설명 :
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
	// 외부에서 보스 상태 변경
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
			BossStateManager.ChangeState("SpawnKnife1");
			break;
		case PSYCHOGIANT_PATTERN::SPAWN_KNIFE2:
			BossStateManager.ChangeState("SpawnKnife2");
			break;
		case PSYCHOGIANT_PATTERN::SPAWN_TURRET:
			BossStateManager.ChangeState("SpawnTurret");
			break;
		case PSYCHOGIANT_PATTERN::SPAWN_PSYCHO:
			break;
		case PSYCHOGIANT_PATTERN::HURT:
			BossStateManager.ChangeState("Hurt");
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

	void SpawnKnife1Start(const StateInfo& _Info);
	void SpawnKnife1Update(float _DeltaTime, const StateInfo& _Info);

	void SpawnKnife2Start(const StateInfo& _Info);
	void SpawnKnife2Update(float _DeltaTime, const StateInfo& _Info);

	void SpawnTurretStart(const StateInfo& _Info);
	void SpawnTurretUpdate(float _DeltaTime, const StateInfo& _Info);

	// 피격
	void HurtStart(const StateInfo& _Info);
	void HurtUpdate(float _DeltaTime, const StateInfo& _Info);
	
	// 무기
	class TentacleKnife* Stabber;
	std::vector<class Portal*> Portals;

	void SpawnPortalsUp();
	void SpawnPortalsDown();
	void SpawnPortalsRound();

private:
	void AllAnimationBind();

	GameEngineTextureRenderer* Renderer_Tentacle;
	GameEngineTextureRenderer* Renderer_Body;
	GameEngineTextureRenderer* Renderer_Face;
	GameEngineTextureRenderer* Renderer_Lilguy;


private:
	int Hp; // 5
	bool IsSelectPattern; // 가만히 있는다 or 패턴 선택 시작
};

