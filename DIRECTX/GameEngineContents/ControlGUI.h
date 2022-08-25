#pragma once
#include <GameEngineCore/GameEngineGUI.h>

// 설명 :
class ControlGUI : public GameEngineGUIWindow
{
public:
	static ControlGUI* Inst;

public:
	static ControlGUI* GetInst()
	{
		if (nullptr == Inst)
		{
			MsgBoxAssert("ControlGUI윈도우 타입이 생성되지 않았습니다");
		}

		return Inst;
	}

public:
	// constrcuter destructer
	ControlGUI();
	~ControlGUI();

	// delete Function
	ControlGUI(const ControlGUI& _Other) = delete;
	ControlGUI(ControlGUI&& _Other) noexcept = delete;
	ControlGUI& operator=(const ControlGUI& _Other) = delete;
	ControlGUI& operator=(ControlGUI&& _Other) noexcept = delete;

protected:

private:
	void OnGUI(GameEngineLevel* _Level, float _DeltaTime) override;
	void Initialize(class GameEngineLevel* _Level) override;

};

