#pragma once
#include <GameEngineCore/GameEngineActor.h>

// ���� :
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

	// ������ ������ �˼� ����ִ� �ִϸ��̼� �����ϴ� �뵵
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

	// ���
	void IdleStart(const StateInfo& _Info);
	void IdleUpdate(float _DeltaTime, const StateInfo& _Info);

	// ������
	void TakeOutStart(const StateInfo& _Info);
	void TakeOutUpdate(float _DeltaTime, const StateInfo& _Info);

	// ����
	void LoadStart(const StateInfo& _Info);
	void LoadUpdate(float _DeltaTime, const StateInfo& _Info);

	// ���
	void StabStart(const StateInfo& _Info);
	void StabUpdate(float _DeltaTime, const StateInfo& _Info);

	// ����ֱ�
	void TakeInStart(const StateInfo& _Info);
	void TakeInUpdate(float _DeltaTime, const StateInfo& _Info);

private:
	class GameEngineTextureRenderer* Renderer;
	class GameEngineCollision* Collision;
	float4 Position;
	bool AttackEnd; // 3ȸ�� ������ ��������
	bool IsStab; // ��� ���̸� X�� �̵� �Ұ�
	bool TakeOutEnd;
	bool IdleEnd;
	int Count;
};

