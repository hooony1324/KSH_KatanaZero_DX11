#include "PreCompile.h"
#include "CharacterShadow.h"
#include <GameEngineCore/CoreMinimal.h>

const float Bright = 15.0f;
const float Dark = 1.0f;

const float4 LightRed = { Bright, Dark, Dark, 0.08f};
const float4 LightGreen = { Dark, Bright, Dark, 0.08f};
const float4 LightBlue = { Dark, Dark, Bright, 0.08f};

std::vector<float4> LightColors::Colors;

LightColors::LightColors()
{
	Colors.push_back(LightRed);
	Colors.push_back(LightGreen);
	Colors.push_back(LightBlue);
}

int CharacterShadow::ColorIndex = 0;
int CharacterShadow::ShadowCount = 0;
LightColors Inst;

void CharacterShadow::SetShadow(GameEngineTexture* _Texture, float _DeathTime, int ShadowDir, float4 _InstancePos)
{
	if (nullptr == _Texture)
	{
		return;
	}
	
	Renderer->SetTexture(_Texture);
	Renderer->ScaleToTexture();
	GetTransform().SetLocalScale({ 1.5f, 1.5f, 1 });
	SetDeathTime = _DeathTime;

	if (ShadowDir > 0)
	{
		Renderer->GetTransform().PixLocalPositiveX();
	}
	else
	{
		Renderer->GetTransform().PixLocalNegativeX();
	}

	Renderer->GetPixelData().MulColor = LightColors::Colors[ColorIndex++];


	if (ColorIndex >= 3)
	{
		ColorIndex = 0;
	}

	// ¿ùµå -> ºä -> ºäÆ÷Æ®
	GetTransform().SetWorldPosition(_InstancePos);
}

CharacterShadow::CharacterShadow()
	: Renderer(nullptr)
	, SetDeathTime(5.0f)
{
}

CharacterShadow::~CharacterShadow() 
{
}

void CharacterShadow::Start()
{
	Renderer = CreateComponent<GameEngineTextureRenderer>();
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
