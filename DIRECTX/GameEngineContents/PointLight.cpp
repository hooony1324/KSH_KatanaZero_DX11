#include "PreCompile.h"
#include "PointLight.h"
#include <GameEngineCore/CoreMinimal.h>

#include "GameContentsCustomRenderer.h"

PointLight::PointLight() 
{
}

PointLight::~PointLight() 
{
}

void PointLight::Start()
{
	//DRenderer = CreateComponent<GameContentsCustomRenderer>();
	//
	//DRenderer->SetPipeLine("PointLight");
	//DRenderer->GetTransform().SetWorldPosition({ 400, -600 , GetDepth(ACTOR_DEPTH::BACKGROUND_4)});


}

void PointLight::Update(float _DeltaTime)
{
}

void PointLight::End()
{
}

void PointLight::OnEvent()
{
}

void PointLight::OffEvent()
{
}
