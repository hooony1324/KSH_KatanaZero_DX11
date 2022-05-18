#pragma once
#include <GameEngineCore/GameEngineCore.h>

// ���� :
class TestContentsCore : public GameEngineCore
{
public:
	// constrcuter destructer
	TestContentsCore();
	~TestContentsCore();

	// delete Function
	TestContentsCore(const TestContentsCore& _Other) = delete;
	TestContentsCore(TestContentsCore&& _Other) noexcept = delete;
	TestContentsCore& operator=(const TestContentsCore& _Other) = delete;
	TestContentsCore& operator=(TestContentsCore&& _Other) noexcept = delete;


	std::string GetWindowTitle() override
	{
		return "MyGameName";
	}

protected:


	void Start() override;

	// �� ���α׷��� ��� ��Ȳ������ ��� Update�ϸ鼭 üũ������ �ִٸ� ���⼭ �Ѵ�.
	// ������ ���� ������ �׷����� ����. ������ ���� �����̰� ���� ����Ű�� ������ �����Ѵ�.
	// Ÿ��Ʋ�� �ѱ�ų� �̷�
	void Update(float _DeltaTime) override;

	void End() override;

private:

};

