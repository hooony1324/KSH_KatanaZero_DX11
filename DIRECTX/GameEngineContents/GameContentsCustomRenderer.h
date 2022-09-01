#pragma once
#include <GameEngineCore/GameEngineDefaultRenderer.h>


struct MaskData
{
public:
	float4 MaskFrameData;
	float4 MaskPivotPos;
};

class CustomFrameAnimation_DESC
{
public:
	std::string TextureName;

	unsigned int CurFrame;

	std::vector<unsigned int> Frames;

	float FrameTime;

	float Inter; // 0.1f

	bool Loop;
	// 아틀라스 애니메이션

	class GameContentsCustomRenderer* Renderer;

public:
	CustomFrameAnimation_DESC()
		: Loop(false)
		, Inter(0.1f)
		, CurFrame(-1)
		, FrameTime(0.0f)
	{

	}

	CustomFrameAnimation_DESC(const std::string _TextureName, unsigned int _Start, unsigned int _End, float _Inter, bool _Loop = true)
		: TextureName(_TextureName)
		, Loop(_Loop)
		, Inter(_Inter)
		, CurFrame(0)
		, FrameTime(0.0f)
	{
		for (unsigned int i = _Start; i <= _End; i++)
		{
			Frames.push_back(i);
		}
	}

	CustomFrameAnimation_DESC(const std::string _TextureName, const std::vector<unsigned int>& _Frames, float _Inter, bool _Loop = true)
		: TextureName(_TextureName)
		, Loop(_Loop)
		, Inter(_Inter)
		, Frames(_Frames)
		, FrameTime(0.0f)
	{

	}


	CustomFrameAnimation_DESC(const std::string _TextureName, float _Inter, bool _Loop = true)
		: TextureName(_TextureName)
		, Loop(_Loop)
		, Inter(_Inter)
		, CurFrame(0)
		, FrameTime(0.0f)
	{

	}
};


class GameEngineFolderTexture;
class GameContentsCustomRenderer;
class CustomFrameAnimation : public GameEngineNameObject
{
private:
	friend GameContentsCustomRenderer;

	CustomFrameAnimation_DESC Info;

	GameContentsCustomRenderer* ParentRenderer;
	GameEngineTexture* Texture;
	GameEngineFolderTexture* FolderTexture;

	GameContentsCustomRenderer* MaskRenderer;


	bool bOnceStart;
	bool bOnceEnd;
	std::function<void(const CustomFrameAnimation_DESC&)> Frame;
	std::function<void(const CustomFrameAnimation_DESC&)> End;
	std::function<void(const CustomFrameAnimation_DESC&)> Start;
	std::function<void(const CustomFrameAnimation_DESC&, float)> Time;

	void Reset();

	void Update(float _DeltaTime);
	void MaskUpdate(float _DeltaTime);

public:
	CustomFrameAnimation()
		: bOnceStart(true)
		, bOnceEnd(false)
	{

	}
};

// 설명 :
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
	void SetMask(GameEngineTexture* _Texture);
	void SetMask(GameEngineTexture* _Texture, UINT _Index);
	void CreateMaskAnimationFolder(const std::string& _AnimationName, const CustomFrameAnimation_DESC& _Desc);
	void ChangeMaskAnimation(const std::string& _AnimationName);
	AtlasData& GetAtlasData()
	{
		return AtlasDataInst;
	}

	MaskData& GetMaskData()
	{
		return MaskDataInst;
	}

	void SetTexture(GameEngineTexture* _Texture);

	void SetTexture(const std::string& _Name);

	void SetFrame(UINT _Index);

	GameEngineTexture* GetCurTexture();

	void SetTexture(const std::string& _Name, UINT _Index);

	void SetPivot();

	void SetPivot(PIVOTMODE _Mode);

	void SetPivotToVector(const float4& _Value);

	void SetTexture(GameEngineTexture* _Texture, UINT _Index);

	void CreateFrameAnimationFolder(const std::string& _AnimationName, const CustomFrameAnimation_DESC& _Desc);

	void CreateFrameAnimationCutTexture(const std::string& _AnimationName, const CustomFrameAnimation_DESC& _Desc);
	void ChangeFrameAnimation(const std::string& _AnimationName);

	void ScaleToTexture();

	void ScaleToCutTexture(int _Index);

	void CurAnimationReset();

	void CurAnimationSetStartPivotFrame(int SetFrame);

	PixelData& GetPixelData()
	{
		return PixelDataInst;
	}


	// 애니메이션 바인드
	// 시작 프레임에 들어온다.
	void AnimationBindStart(const std::string& _AnimationName, std::function<void(const CustomFrameAnimation_DESC&)> _Function)
	{
		std::string Name = GameEngineString::ToUpperReturn(_AnimationName);

		if (FrameAni.end() == FrameAni.find(Name))
		{
			MsgBoxAssert("존재하지 않는 애니메이션으로 체인지 하려고 했습니다.");
			return;
		}

		FrameAni[Name].Start = _Function;
	}
	// 끝나는 프레임에 들어온다
	void AnimationBindEnd(const std::string& _AnimationName, std::function<void(const CustomFrameAnimation_DESC&)> _Function)
	{
		std::string Name = GameEngineString::ToUpperReturn(_AnimationName);

		if (FrameAni.end() == FrameAni.find(Name))
		{
			MsgBoxAssert("존재하지 않는 애니메이션으로 체인지 하려고 했습니다.");
			return;
		}

		FrameAni[Name].End = _Function;
	}
	// 프레임이 바뀔때마다 들어온다
	void AnimationBindFrame(const std::string& _AnimationName, std::function<void(const CustomFrameAnimation_DESC&)> _Function)
	{
		std::string Name = GameEngineString::ToUpperReturn(_AnimationName);

		if (FrameAni.end() == FrameAni.find(Name))
		{
			MsgBoxAssert("존재하지 않는 애니메이션으로 체인지 하려고 했습니다.");
			return;
		}

		FrameAni[Name].Frame = _Function;
	}
	// Update
	void AnimationBindTime(const std::string& _AnimationName, std::function<void(const CustomFrameAnimation_DESC&, float)> _Function)
	{
		std::string Name = GameEngineString::ToUpperReturn(_AnimationName);

		if (FrameAni.end() == FrameAni.find(Name))
		{
			MsgBoxAssert("존재하지 않는 애니메이션으로 체인지 하려고 했습니다.");
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

	AtlasData AtlasDataInst;
	PixelData PixelDataInst;
	MaskData MaskDataInst;


	std::map<std::string, CustomFrameAnimation> FrameAni;
	std::map<std::string, CustomFrameAnimation> MaskAni;
	CustomFrameAnimation* CurAni;
	CustomFrameAnimation* CurMask;

	void FrameDataReset();
};

