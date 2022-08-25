#pragma once
#include <GameEngineCore/GameEngineActor.h>

// 설명 :
class TentacleKnife : public GameEngineActor
{
public:
	void Idle();
	void StabAttackStart(int _Count);
	void TakeOut();
	
	void SetTakeOutEnd(bool _Value)
	{
		TakeOutEnd = _Value;
	}

	// 공격이 끝나면 촉수 집어넣는 애니메이션 시작하는 용도
	void SetAttackEnd(bool _Value)
	{
		AttackEnd = _Value;
	}

	bool IsTakeOutEnd()
	{
		return TakeOutEnd;
	}

	bool IsStabAttackEnd()
	{
		return AttackEnd;
	}

private:

	void Load();

public:
	// constrcuter destructer
	TentacleKnife();
	~TentacleKnife();

	// delete Function
	TentacleKnife(const TentacleKnife& _Other) = delete;
	TentacleKnife(TentacleKnife&& _Other) noexcept = delete;
	TentacleKnife& operator=(const TentacleKnife& _Other) = delete;
	TentacleKnife& operator=(TentacleKnife&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

	void OnEvent() override;
	void OffEvent() override;

private:
	void CreateAllState();
	GameEngineStateManager StateManager;

	// 평소
	void IdleStart(const StateInfo& _Info);
	void IdleUpdate(float _DeltaTime, const StateInfo& _Info);

	// 꺼내기
	void TakeOutStart(const StateInfo& _Info);
	void TakeOutUpdate(float _DeltaTime, const StateInfo& _Info);

	// 장전
	void LoadStart(const StateInfo& _Info);
	void LoadUpdate(float _DeltaTime, const StateInfo& _Info);

	// 찌르기
	void StabStart(const StateInfo& _Info);
	void StabUpdate(float _DeltaTime, const StateInfo& _Info);

	// 집어넣기
	void TakeInStart(const StateInfo& _Info);
	void TakeInUpdate(float _DeltaTime, const StateInfo& _Info);

private:
	class GameEngineTextureRenderer* Renderer;
	class GameEngineCollision* Collision;
	float4 Position;
	bool AttackEnd; // 3회의 공격이 끝났는지
	bool IsStab; // 찌르는 중이면 X축 이동 불가
	bool TakeOutEnd;
	bool IdleEnd;
	int Count;
};

