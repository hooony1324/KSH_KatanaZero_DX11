#include "PreCompile.h"
#include "Ending_Logo.h"

Ending_Logo::Ending_Logo() 
{
}

Ending_Logo::~Ending_Logo() 
{
}

void Ending_Logo::Start()
{
	Logo = CreateComponent<GameEngineTextureRenderer>();
	Logo->SetScaleRatio(0.9f);
	Logo->SetTexture("spr_ending_title_0.png");
	Logo->GetPixelData().MulColor.a = 0.0f;
	Logo->ScaleToTexture();
	Logo->GetTransform().SetWorldPosition({ 0, 0, GetDepth(ACTOR_DEPTH::LOGO) });
}

void Ending_Logo::Update(float _DeltaTime)
{
	if (Logo->GetPixelData().MulColor.a < 1.0f)
	{
		Logo->GetPixelData().MulColor.a += _DeltaTime;
	}
}

void Ending_Logo::End()
{
}
