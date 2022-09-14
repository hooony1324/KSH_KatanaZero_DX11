#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class GameEngineTextureRenderer;
class DiamondTransition : public GameEngineActor
{
public:
	bool IsChangeBlackEnd()
	{
		return !ChangeBlack;
	}

	bool IsChangeWhiteEnd()
	{
		return !ChangeWhite;
	}

	bool IsBlack()
	{
		return Black;
	}

	enum class STATE
	{
		WHITE,
		CHANGEBLACK,
		BLACK,
		CHANGEWHITE
	};

	void ChangeState(STATE _State)
	{
		switch (_State)
		{
		case DiamondTransition::STATE::WHITE:
			StateManager.ChangeState("White");
			break;
		case DiamondTransition::STATE::CHANGEBLACK:
			StateManager.ChangeState("ChangeBlack");
			break;
		case DiamondTransition::STATE::CHANGEWHITE:
			StateManager.ChangeState("ChangeWhite");
			break;
		default:
			break;
		}
	}

public:
	// constrcuter destructer
	DiamondTransition();
	~DiamondTransition();

	// delete Function
	DiamondTransition(const DiamondTransition& _Other) = delete;
	DiamondTransition(DiamondTransition&& _Other) noexcept = delete;
	DiamondTransition& operator=(const DiamondTransition& _Other) = delete;
	DiamondTransition& operator=(DiamondTransition&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

private:
	GameEngineUIRenderer* BaseRenderer;
	std::vector<std::vector<GameEngineUIRenderer*>> Diamonds;

	bool ChangeBlack;
	bool ChangeWhite;
	bool Black;

// FSM
private:
	GameEngineStateManager StateManager;

	void WhiteStart(const StateInfo& _Info);
	void WhiteUpdate(float _DeltaTime, const StateInfo& _Info);

	void ChangeBlackStart(const StateInfo& _Info);
	void ChangeBlackUpdate(float _DeltaTime, const StateInfo& _Info);

	void ChangeWhiteStart(const StateInfo& _Info);
	void ChangeWhiteUpdate(float _DeltaTime, const StateInfo& _Info);

};

