#include "PreCompile.h"
#include "ChromakeyGreen.h"
#include <GameEngineCore/CoreMinimal.h>

bool ChromakeyGreen::Activate = false;

ChromakeyGreen::ChromakeyGreen() 
{
}

ChromakeyGreen::~ChromakeyGreen() 
{
}

void ChromakeyGreen::Start()
{
	Renderer = CreateComponent<GameEngineTextureRenderer>();
	Renderer->SetTexture("Chromakey.png");
	Renderer->GetTransform().SetLocalScale({ 3000, 3000, 1 });
	Renderer->GetPixelData().MulColor.a = 1.0f;
	Renderer->Off();

}

void ChromakeyGreen::Update(float _DeltaTime)
{
	if (false == Activate)
	{
		Off();
	}
	else
	{
		On();
	}
}

void ChromakeyGreen::End()
{
}

void ChromakeyGreen::OnEvent()
{
	Renderer->On();
	float4 CamPos = GetLevel()->GetMainCameraActor()->GetTransform().GetWorldPosition();
	GetTransform().SetWorldPosition(CamPos + float4{ 0, 0, GetDepth(ACTOR_DEPTH::PLAYER) + 110 });
}

void ChromakeyGreen::OffEvent()
{
	Renderer->Off();
}
