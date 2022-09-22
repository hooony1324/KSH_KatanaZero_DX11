#include "PreCompile.h"
#include "PointLight.h"
#include <GameEngineCore/CoreMinimal.h>

#include <GameEngineCore/GameEngineBlend.h>

PointLight::PointLight() 
{
}

PointLight::~PointLight() 
{
}

void PointLight::Start()
{
	// 위치 판별용
	DRenderer = CreateComponent<GameEngineDefaultRenderer>();
	DRenderer->GetTransform().SetLocalScale({ 300, 300, 1 });

	// 포스트이펙트 이용해야됨
	// Tex에 라이트 텍스쳐 세팅은 하지만 출력은 안함 -> 쉐이더에서 하면됨
	// 쉐이더에 필요한 텍스쳐? 
	DRenderer->SetPipeLine("PointLight");
	
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
