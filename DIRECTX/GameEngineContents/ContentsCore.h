#pragma once
#include <GameEngineCore/CoreMinimal.h>
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

	std::string GetWindowTitle() override { return "Katana ZERO"; }
	float4 StartWindowSize() override 
	{ 
		ContentsWindowSize = { 1280.0f, 720.0f };
		return ContentsWindowSize;
	}
	float4 StartWindowPosition() override { return { 0 , 0 }; }

	static float4 GetContentsWindowSize()
	{
		return ContentsWindowSize;
	}
protected:
	virtual void Start() override;
	virtual void Update(float _DeltaTime) override;
	virtual void End() override;
private:
	static float4 ContentsWindowSize;
	void ShaderCompile();
	void PipelineLoad();
	void TextureLoad();
	void SoundLoad();
};

