#pragma once
#include <GameEngineCore/GameEngineCore.h>

// Ό³Έν :
class ContentsCore : public GameEngineCore
{
public:
	// constrcuter destructer
	ContentsCore();
	~ContentsCore();

	// delete Function
	ContentsCore(const ContentsCore& _Other) = delete;
	ContentsCore(ContentsCore&& _Other) noexcept = delete;
	ContentsCore& operator=(const ContentsCore& _Other) = delete;
	ContentsCore& operator=(ContentsCore&& _Other) noexcept = delete;

	std::string GetWindowTitle() { return "Katana ZERO"; }

protected:
	virtual void Start() override;
	virtual void Update(float _DeltaTime) override;
	virtual void End() override;
private:

	void TextureLoad();
	void SoundLoad();
};

