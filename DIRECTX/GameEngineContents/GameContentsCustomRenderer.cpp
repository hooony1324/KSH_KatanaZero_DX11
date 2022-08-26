#include "PreCompile.h"
#include "GameContentsCustomRenderer.h"
#include <GameEngineCore/CoreMinimal.h>

void CustomFrameAnimation::Reset()
{
	Info.FrameTime = 0.0f;
	Info.CurFrame = 0;
}

void CustomFrameAnimation::Update(float _Delta)
{
	
	Info.FrameTime += _Delta;

	if (nullptr != Time)
	{
		Time(Info, _Delta);
	}

	if (false == bOnceStart
		&& Info.CurFrame == 0)
	{
		if (nullptr != Start)
		{
			Start(Info);
		}
		bOnceStart = true;
		bOnceEnd = false;
	}

	if (Info.Inter <= Info.FrameTime)
	{
		if (Info.CurFrame == (Info.Frames.size() - 1)
			&& false == bOnceEnd
			&& nullptr != End)
		{
			End(Info);
			bOnceEnd = true;
			bOnceStart = false;
		}

		++Info.CurFrame;
		if (nullptr != Frame)
		{
			Frame(Info);
		}

		if (Info.CurFrame >= Info.Frames.size())
		{

			if (true == Info.Loop)
			{
				Info.CurFrame = 0;
			}
			else
			{
				Info.CurFrame = static_cast<unsigned int>(Info.Frames.size()) - 1;
			}
		}
		Info.FrameTime -= Info.Inter;
	}


	if (nullptr != Texture)
	{
		ParentRenderer->CurTex = Texture;
		ParentRenderer->SetTexture(Texture, Info.Frames[Info.CurFrame]);
		ParentRenderer->SetPivot();

		// 잘렸다는 거죠?
		if (Texture->GetCutCount() != 0)
		{
			if (ParentRenderer->ScaleMode == SCALEMODE::IMAGE)
			{
				ParentRenderer->ScaleToCutTexture(Info.Frames[Info.CurFrame]);
			}
		}
		else
		{
			if (ParentRenderer->ScaleMode == SCALEMODE::IMAGE)
			{
				ParentRenderer->ScaleToTexture();
			}
		}
	}
	else if (nullptr != FolderTexture)
	{
		ParentRenderer->FrameDataReset();
		ParentRenderer->CurTex = FolderTexture->GetTexture(Info.Frames[Info.CurFrame]);
		ParentRenderer->SetTexture(FolderTexture->GetTexture(Info.Frames[Info.CurFrame]));
		ParentRenderer->SetPivot();

		if (ParentRenderer->ScaleMode == SCALEMODE::IMAGE)
		{
			ParentRenderer->ScaleToTexture();
		}
	}
	else
	{
		MsgBoxAssert("텍스처가 세팅되지 않은 애니메이션 입니다.");
	}






}

void CustomFrameAnimation::MaskUpdate(float _DeltaTime)
{
	Info.FrameTime += _DeltaTime;

	if (nullptr != Time)
	{
		Time(Info, _DeltaTime);
	}

	if (false == bOnceStart
		&& Info.CurFrame == 0)
	{
		if (nullptr != Start)
		{
			Start(Info);
		}
		bOnceStart = true;
		bOnceEnd = false;
	}

	if (Info.Inter <= Info.FrameTime)
	{
		if (Info.CurFrame == (Info.Frames.size() - 1)
			&& false == bOnceEnd
			&& nullptr != End)
		{
			End(Info);
			bOnceEnd = true;
			bOnceStart = false;
		}

		++Info.CurFrame;
		if (nullptr != Frame)
		{
			Frame(Info);
		}

		if (Info.CurFrame >= Info.Frames.size())
		{

			if (true == Info.Loop)
			{
				Info.CurFrame = 0;
			}
			else
			{
				Info.CurFrame = static_cast<unsigned int>(Info.Frames.size()) - 1;
			}
		}
		Info.FrameTime -= Info.Inter;
	}


	if (nullptr != FolderTexture)
	{
		//MaskRenderer->FrameDataReset();
		//MaskRenderer->CurTex = FolderTexture->GetTexture(Info.Frames[Info.CurFrame]);
		MaskRenderer->SetMask(FolderTexture->GetTexture(Info.Frames[Info.CurFrame]));
		MaskRenderer->SetPivot();

		if (MaskRenderer->ScaleMode == SCALEMODE::IMAGE)
		{
			MaskRenderer->ScaleToTexture();
		}
	}
	else
	{
		MsgBoxAssert("텍스처가 세팅되지 않은 애니메이션 입니다.");
	}
}

GameContentsCustomRenderer::GameContentsCustomRenderer()
	: CurAni(nullptr)
	, CurTex(nullptr)
	, PivotMode(PIVOTMODE::CUSTOM)
	, ScaleMode(SCALEMODE::CUSTOM)
	, ScaleRatio(1.0f)
{
}

GameContentsCustomRenderer::~GameContentsCustomRenderer()
{
}

void GameContentsCustomRenderer::SetTextureRendererSetting()
{
	SetPipeLine("TextureMask");

	AtlasDataInst.FrameData.PosX = 0.0f;
	AtlasDataInst.FrameData.PosY = 0.0f;
	AtlasDataInst.FrameData.SizeX = 1.0f;
	AtlasDataInst.FrameData.SizeY = 1.0f;
	AtlasDataInst.PivotPos = float4::ZERO;

	ShaderResources.SetConstantBufferLink("AtlasData", AtlasDataInst);
	ShaderResources.SetConstantBufferLink("PixelData", PixelDataInst);

}

void GameContentsCustomRenderer::Start()
{
	GameEngineDefaultRenderer::Start();

	PushRendererToMainCamera();

	SetTextureRendererSetting();
}

void GameContentsCustomRenderer::SetSamplingModePoint()
{
	ShaderResources.SetSampler("Smp", "EngineSamplerPoint");
}

void GameContentsCustomRenderer::SetSamplingModeLiner()
{
	ShaderResources.SetSampler("Smp", "EngineSamplerLinear");
}

void GameContentsCustomRenderer::SetPivot()
{
	SetPivot(PivotMode);
}

void GameContentsCustomRenderer::SetPivot(PIVOTMODE _Mode)
{
	switch (_Mode)
	{
	case PIVOTMODE::CENTER:
		AtlasDataInst.PivotPos = float4::ZERO;
		break;
	case PIVOTMODE::BOT:
		AtlasDataInst.PivotPos = float4(0.0f, 0.5f, 0.0f, 0.0f);
		break;
	case PIVOTMODE::TOP:
		AtlasDataInst.PivotPos = float4(0.0f, -0.5f, 0.0f, 0.0f);
		break;
	case PIVOTMODE::LEFT:
		AtlasDataInst.PivotPos = float4(0.5f, 0.0f, 0.0f, 0.0f);
		break;
	case PIVOTMODE::RIGHT:
		AtlasDataInst.PivotPos = float4(-0.5f, 0.0f, 0.0f, 0.0f);
		break;
	case PIVOTMODE::LEFTTOP:
		AtlasDataInst.PivotPos = float4(0.5f, -0.5f, 0.0f, 0.0f);
		break;
	case PIVOTMODE::RIGHTTOP:
		AtlasDataInst.PivotPos = float4(-0.5f, -0.5f, 0.0f, 0.0f);
		break;
	case PIVOTMODE::LEFTBOT:
		AtlasDataInst.PivotPos = float4(0.5f, 0.5f, 0.0f, 0.0f);
		break;
	case PIVOTMODE::RIGHTBOT:
		AtlasDataInst.PivotPos = float4(-0.5f, 0.5f, 0.0f, 0.0f);
		break;
	case PIVOTMODE::CUSTOM:
		break;
	default:
		break;
	}

	PivotMode = _Mode;
}

void GameContentsCustomRenderer::SetPivotToVector(const float4& _Value)
{
	GetTransform().SetLocalPosition(_Value);
}

// 1개 만
void GameContentsCustomRenderer::SetMask(const std::string& _Name)
{
	MaskTex = ShaderResources.SetTexture("Mask", _Name);
}

// 마스크가 애니메이션
void GameContentsCustomRenderer::SetMask(GameEngineTexture* _Texture)
{
	MaskTex = ShaderResources.SetTexture("Mask", _Texture);
}

// 실험중
void GameContentsCustomRenderer::CreateMaskAnimationFolder(const std::string& _AnimationName, const CustomFrameAnimation_DESC& _Desc)
{
	std::string Name = GameEngineString::ToUpperReturn(_AnimationName);

	if (MaskAni.end() != MaskAni.find(Name))
	{
		MsgBoxAssert("이미 존재하는 애니메이션을 또 만들려고 했습니다.");
		return;
	}

	CustomFrameAnimation& NewAni = MaskAni[Name];
	NewAni.Info = _Desc;
	NewAni.MaskRenderer = this;
	NewAni.Texture = nullptr;
	NewAni.FolderTexture = GameEngineFolderTexture::Find(_Desc.TextureName);

	if (NewAni.Info.Frames.size() == 0)
	{
		for (unsigned int i = 0; i < NewAni.FolderTexture->GetTextureCount(); i++)
		{
			NewAni.Info.Frames.push_back(i);
		}
	}
}

void GameContentsCustomRenderer::SetMask(GameEngineTexture* _Texture, UINT _Index)
{
	if (nullptr == _Texture)
	{
		MsgBoxAssert("존재하지 않는 텍스처를 사용하려고 했습니다.");
		return;
	}

	SetMask(_Texture);
	SetFrame(_Index);
}

void GameContentsCustomRenderer::ChangeMaskAnimation(const std::string& _AnimationName)
{
	std::string Name = GameEngineString::ToUpperReturn(_AnimationName);

	if (MaskAni.end() == MaskAni.find(Name))
	{
		MsgBoxAssert("존재하지 않는 애니메이션으로 체인지 하려고 했습니다.");
		return;
	}

	if (CurMask != &MaskAni[Name])
	{
		CurMask = &MaskAni[Name];
		CurMask->Reset();
		if (nullptr != CurMask->Texture)
		{
			SetMask(CurMask->Texture, CurMask->Info.Frames[CurMask->Info.CurFrame]);
			if (ScaleMode == SCALEMODE::IMAGE)
			{
				ScaleToCutTexture(CurMask->Info.CurFrame);
			}
		}
		else if (nullptr != CurMask->FolderTexture)
		{
			SetMask(CurMask->FolderTexture->GetTexture(CurMask->Info.Frames[CurMask->Info.CurFrame]));
			if (ScaleMode == SCALEMODE::IMAGE)
			{
				ScaleToTexture();
			}
		}
	}
}

void GameContentsCustomRenderer::SetTexture(GameEngineTexture* _Texture)
{
	CurTex = _Texture;
	ShaderResources.SetTexture("Tex", _Texture);
}

void GameContentsCustomRenderer::SetTexture(const std::string& _Name)
{
	CurTex = ShaderResources.SetTexture("Tex", _Name);
}

void GameContentsCustomRenderer::SetFrame(UINT _Index)
{
	AtlasDataInst.FrameData = CurTex->GetFrameData(_Index);
}

GameEngineTexture* GameContentsCustomRenderer::GetCurTexture()
{
	return CurTex;
}

void GameContentsCustomRenderer::SetTexture(const std::string& _Name, UINT _Index)
{
	SetTexture(_Name);
	SetFrame(_Index);
}

void GameContentsCustomRenderer::SetTexture(GameEngineTexture* _Texture, UINT _Index)
{
	if (nullptr == _Texture)
	{
		MsgBoxAssert("존재하지 않는 텍스처를 사용하려고 했습니다.");
		return;
	}

	SetTexture(_Texture);
	SetFrame(_Index);
}

void GameContentsCustomRenderer::CreateFrameAnimationFolder(const std::string& _AnimationName, const CustomFrameAnimation_DESC& _Desc)
{
	std::string Name = GameEngineString::ToUpperReturn(_AnimationName);

	if (FrameAni.end() != FrameAni.find(Name))
	{
		MsgBoxAssert("이미 존재하는 애니메이션을 또 만들려고 했습니다.");
		return;
	}

	CustomFrameAnimation& NewAni = FrameAni[Name];
	NewAni.Info = _Desc;
	NewAni.ParentRenderer = this;
	NewAni.Texture = nullptr;
	NewAni.FolderTexture = GameEngineFolderTexture::Find(_Desc.TextureName);

	if (NewAni.Info.Frames.size() == 0)
	{
		for (unsigned int i = 0; i < NewAni.FolderTexture->GetTextureCount(); i++)
		{
			NewAni.Info.Frames.push_back(i);
		}
	}
}

void GameContentsCustomRenderer::CreateFrameAnimationCutTexture(const std::string& _AnimationName, const CustomFrameAnimation_DESC& _Desc)
{
	std::string Name = GameEngineString::ToUpperReturn(_AnimationName);

	if (FrameAni.end() != FrameAni.find(Name))
	{
		MsgBoxAssert("이미 존재하는 애니메이션을 또 만들려고 했습니다.");
		return;
	}

	CustomFrameAnimation& NewAni = FrameAni[Name];
	NewAni.Info = _Desc;
	NewAni.ParentRenderer = this;
	NewAni.Texture = GameEngineTexture::Find(_Desc.TextureName);
	NewAni.FolderTexture = nullptr;
}

void GameContentsCustomRenderer::ChangeFrameAnimation(const std::string& _AnimationName)
{
	std::string Name = GameEngineString::ToUpperReturn(_AnimationName);

	if (FrameAni.end() == FrameAni.find(Name))
	{
		MsgBoxAssert("존재하지 않는 애니메이션으로 체인지 하려고 했습니다.");
		return;
	}

	if (CurAni != &FrameAni[Name])
	{
		CurAni = &FrameAni[Name];
		CurAni->Reset();
		if (nullptr != CurAni->Texture)
		{
			SetTexture(CurAni->Texture, CurAni->Info.Frames[CurAni->Info.CurFrame]);
			if (ScaleMode == SCALEMODE::IMAGE)
			{
				ScaleToCutTexture(CurAni->Info.CurFrame);
			}
		}
		else if (nullptr != CurAni->FolderTexture)
		{
			SetTexture(CurAni->FolderTexture->GetTexture(CurAni->Info.Frames[CurAni->Info.CurFrame]));
			if (ScaleMode == SCALEMODE::IMAGE)
			{
				ScaleToTexture();
			}
		}
	}
}

void GameContentsCustomRenderer::FrameDataReset()
{
	AtlasDataInst.FrameData = { 0.0f , 0.0f, 1.0f, 1.0f };
}


void GameContentsCustomRenderer::Update(float _Delta)
{
	if (nullptr != CurAni)
	{
		CurAni->Update(_Delta);
	}

	if (Option.IsMask == 1)
	{
		CurMask->MaskUpdate(_Delta);
	}
}



void GameContentsCustomRenderer::ScaleToCutTexture(int _Index)
{
	float4 Scale = CurTex->GetCutScale(_Index);

	// 이거는 봐야합니다.
	if (0 > GetTransform().GetLocalScale().x)
	{
		Scale.x = -Scale.x;
	}

	if (0 > GetTransform().GetLocalScale().y)
	{
		Scale.y = -Scale.y;
	}

	GetTransform().SetLocalScale(Scale * ScaleRatio);
}

void GameContentsCustomRenderer::ScaleToTexture()
{
	float4 Scale = CurTex->GetScale();

	// 이거는 봐야합니다.
	if (0 > GetTransform().GetLocalScale().x)
	{
		Scale.x = -Scale.x;
	}

	if (0 > GetTransform().GetLocalScale().y)
	{
		Scale.y = -Scale.y;
	}

	GetTransform().SetLocalScale(Scale * ScaleRatio);
}

void GameContentsCustomRenderer::CurAnimationReset()
{
	CurAni->Reset();
}

void GameContentsCustomRenderer::CurAnimationSetStartPivotFrame(int SetFrame)
{
	CurAni->Info.CurFrame = SetFrame;
}
