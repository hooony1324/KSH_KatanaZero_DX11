#include "PreCompile.h"
#include "CharacterShadow.h"
#include <GameEngineCore/CoreMinimal.h>

const float Bright = 90.0f;
const float Dark = 45.0f;

const float4 LightBlue = { Dark, Bright , Bright, 0.01f };
const float4 LightRed = { Bright, Dark, Bright, 0.01f};

std::vector<float4> LightColors::Colors;

LightColors::LightColors()
{
	Colors.push_back(LightBlue);
	Colors.push_back(LightRed);
}

int CharacterShadow::ColorIndex = 0;
int CharacterShadow::ShadowCount = 0;
LightColors Inst;
bool CharacterShadow::UIShadowOn = false;

void CharacterShadow::SwitchShadowMode()
{
	UIShadowOn = !UIShadowOn;
}

void CharacterShadow::SetShadow(GameEngineTexture* _Texture, int ShadowDir, float4 _InstancePos, float _DeathTime)
{
	TexRenderer->On();
	UIRenderer->Off();
	if (true == UIShadowOn)
	{
		return;
	}

	if (nullptr == _Texture)
	{
		return;
	}
	
	TexRenderer->SetTexture(_Texture);
	TexRenderer->ScaleToTexture();
	GetTransform().SetLocalScale({ 1.5f, 1.8f, 1 });
	SetDeathTime = _DeathTime;

	if (ShadowDir > 0)
	{
		TexRenderer->GetTransform().PixLocalPositiveX();
	}
	else
	{
		TexRenderer->GetTransform().PixLocalNegativeX();
	}

	TexRenderer->GetPixelData().MulColor = LightColors::Colors[ColorIndex];
	ShadowCount++;

	if (ShadowCount > 5)
	{
		ColorIndex++;
		if (ColorIndex >= LightColors::Colors.size())
		{
			ColorIndex = 0;
		}
		ShadowCount = 0;
	}

	GetTransform().SetWorldPosition(_InstancePos);
}

void CharacterShadow::SetUIShadow(GameEngineTexture* _Texture, int ShadowDir, float4 _InstancePos, float _DeathTime)
{
	TexRenderer->Off();
	UIRenderer->On();
	if (false == UIShadowOn)
	{
		return;
	}

	if (nullptr == _Texture)
	{
		return;
	}

	UIRenderer->SetTexture(_Texture);
	UIRenderer->ScaleToTexture();
	GetTransform().SetLocalScale({ 2.0f, 2.0f, 1 });
	SetDeathTime = _DeathTime;

	if (ShadowDir > 0)
	{
		UIRenderer->GetTransform().PixLocalPositiveX();
	}
	else
	{
		UIRenderer->GetTransform().PixLocalNegativeX();
	}

	UIRenderer->GetPixelData().MulColor = LightColors::Colors[0];

	// ¿ùµå -> ºä -> ºäÆ÷Æ®
	GameEngineCamera* MainCam = GetLevel()->GetMainCamera();
	float4 Pos = _InstancePos;
	float4 CamPos = MainCam->GetTransform().GetWorldPosition();
	Pos.z = 0;
	CamPos.z = 0;
	float4x4 MVP = MainCam->GetTransform().GetWorldViewProjection();

	Pos.z = _InstancePos.z;
	Pos = (Pos - CamPos) * MVP;

	GetTransform().SetWorldPosition(Pos);
}

CharacterShadow::CharacterShadow()
	: TexRenderer(nullptr)
	, UIRenderer(nullptr)
	, SetDeathTime(0.12f)
{
}

CharacterShadow::~CharacterShadow() 
{
}

void CharacterShadow::Start()
{
	TexRenderer = CreateComponent<GameEngineTextureRenderer>();
	TexRenderer->SetSamplingModePoint();
	UIRenderer = CreateComponent<GameEngineUIRenderer>();
	UIRenderer->SetSamplingModePoint();

}

void CharacterShadow::OnEvent()
{
	Death(SetDeathTime);
}

void CharacterShadow::Update(float _DeltaTime)
{

}

void CharacterShadow::End()
{
}
