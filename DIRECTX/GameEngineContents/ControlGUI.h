#pragma once
#include <GameEngineCore/GameEngineGUI.h>

// ���� :
class ControlGUI : public GameEngineGUIWindow
{
public:
	static ControlGUI* Inst;

public:
	static ControlGUI* GetInst()
	{
		if (nullptr == Inst)
		{
			MsgBoxAssert("ControlGUI������ Ÿ���� �������� �ʾҽ��ϴ�");
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

