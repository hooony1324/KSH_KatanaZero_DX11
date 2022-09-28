#include "PreCompile.h"
#include "BulletTrail.h"
#include <GameEngineCore/CoreMinimal.h>

BulletTrail::BulletTrail() 
{
}

BulletTrail::~BulletTrail() 
{
}

void BulletTrail::Start()
{
	Renderer = CreateComponent<GameEngineTextureRenderer>();
	Renderer->SetTexture("NONE_yellow.png");

	Renderer->GetPixelData().MulColor.a = 0.2f;

	Renderer->GetPixelData().PlusColor.b = 1;
	
}

void BulletTrail::Update(float _DeltaTime)
{
}

void BulletTrail::End()
{
}
