#pragma once
#include <GameEngineCore/GameEngineDefaultRenderer.h>

class GameEngineFolderTexture;
class GameContentsCustomRenderer;
class CustomFrameAnimation : public GameEngineNameObject
{
	friend GameContentsCustomRenderer;

	FrameAnimation_DESC Info;

	GameContentsCustomRenderer* ParentRenderer;
	GameEngineTexture* Texture;
	GameEngineFolderTexture* FolderTexture;



	bool bOnceStart;
	bool bOnceEnd;
	std::function<void(const FrameAnimation_DESC&)> Frame;
	std::function<void(const FrameAnimation_DESC&)> End;
	std::function<void(const FrameAnimation_DESC&)> Start;
	std::function<void(const FrameAnimation_DESC&, float)> Time;

	void Reset();

	void Update(float _DeltaTime);

public:
	CustomFrameAnimation()
		: bOnceStart(true)
		, bOnceEnd(false)
	{

	}
};

// ���� :
class GameContentsCustomRenderer : public GameEngineDefaultRenderer
{
	friend CustomFrameAnimation;

public:
	// constrcuter destructer
	GameContentsCustomRenderer();
	~GameContentsCustomRenderer();

	// delete Function
	GameContentsCustomRenderer(const GameContentsCustomRenderer& _Other) = delete;
	GameContentsCustomRenderer(GameContentsCustomRenderer&& _Other) noexcept = delete;
	GameContentsCustomRenderer& operator=(const GameContentsCustomRenderer& _Other) = delete;
	GameContentsCustomRenderer& operator=(GameContentsCustomRenderer&& _Other) noexcept = delete;

	void SetSamplingModePoint();
	void SetSamplingModeLiner();

	void SetScaleModeImage()
	{
		ScaleMode = SCALEMODE::IMAGE;
	}

	void SetScaleRatio(float _Scale)
	{
		ScaleRatio = _Scale;
	}

	float GetScaleRatio()
	{
		return ScaleRatio;
	}

	void SetMask(const std::string& _Name);

	void SetTexture(GameEngineTexture* _Texture);

	void SetTexture(const std::string& _Name);

	void SetFrame(UINT _Index);

	GameEngineTexture* GetCurTexture();

	void SetTexture(const std::string& _Name, UINT _Index);

	void SetPivot();

	void SetPivot(PIVOTMODE _Mode);

	void SetPivotToVector(const float4& _Value);

	void SetTexture(GameEngineTexture* _Texture, UINT _Index);

	void CreateFrameAnimationFolder(const std::string& _AnimationName, const FrameAnimation_DESC& _Desc);

	void CreateFrameAnimationCutTexture(const std::string& _AnimationName, const FrameAnimation_DESC& _Desc);
	void ChangeFrameAnimation(const std::string& _AnimationName);

	void ScaleToTexture();

	void ScaleToCutTexture(int _Index);

	void CurAnimationReset();

	void CurAnimationSetStartPivotFrame(int SetFrame);

	ColorData& GetColorData()
	{
		return ColorData;
	}


	// �ִϸ��̼� ���ε�
	// ���� �����ӿ� ���´�.
	void AnimationBindStart(const std::string& _AnimationName, std::function<void(const FrameAnimation_DESC&)> _Function)
	{
		std::string Name = GameEngineString::ToUpperReturn(_AnimationName);

		if (FrameAni.end() == FrameAni.find(Name))
		{
			MsgBoxAssert("�������� �ʴ� �ִϸ��̼����� ü���� �Ϸ��� �߽��ϴ�.");
			return;
		}

		FrameAni[Name].Start = _Function;
	}
	// ������ �����ӿ� ���´�
	void AnimationBindEnd(const std::string& _AnimationName, std::function<void(const FrameAnimation_DESC&)> _Function)
	{
		std::string Name = GameEngineString::ToUpperReturn(_AnimationName);

		if (FrameAni.end() == FrameAni.find(Name))
		{
			MsgBoxAssert("�������� �ʴ� �ִϸ��̼����� ü���� �Ϸ��� �߽��ϴ�.");
			return;
		}

		FrameAni[Name].End = _Function;
	}
	// �������� �ٲ𶧸��� ���´�
	void AnimationBindFrame(const std::string& _AnimationName, std::function<void(const FrameAnimation_DESC&)> _Function)
	{
		std::string Name = GameEngineString::ToUpperReturn(_AnimationName);

		if (FrameAni.end() == FrameAni.find(Name))
		{
			MsgBoxAssert("�������� �ʴ� �ִϸ��̼����� ü���� �Ϸ��� �߽��ϴ�.");
			return;
		}

		FrameAni[Name].Frame = _Function;
	}
	// Update
	void AnimationBindTime(const std::string& _AnimationName, std::function<void(const FrameAnimation_DESC&, float)> _Function)
	{
		std::string Name = GameEngineString::ToUpperReturn(_AnimationName);

		if (FrameAni.end() == FrameAni.find(Name))
		{
			MsgBoxAssert("�������� �ʴ� �ִϸ��̼����� ü���� �Ϸ��� �߽��ϴ�.");
			return;
		}
		FrameAni[Name].Time = _Function;
	}

protected:
	void Start() override;

	void SetTextureRendererSetting();

	void Update(float _Delta) override;

private:
	PIVOTMODE PivotMode;
	SCALEMODE ScaleMode;
	float ScaleRatio;

	GameEngineTexture* MaskTex;
	GameEngineTexture* CurTex;

	ColorData ColorData;
	AtlasData AtlasDataInst;

	std::map<std::string, CustomFrameAnimation> FrameAni;
	CustomFrameAnimation* CurAni;

	void FrameDataReset();
};

