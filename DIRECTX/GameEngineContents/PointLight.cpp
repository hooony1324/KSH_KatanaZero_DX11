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
	// ��ġ �Ǻ���
	DRenderer = CreateComponent<GameEngineDefaultRenderer>();
	DRenderer->GetTransform().SetLocalScale({ 300, 300, 1 });

	// ����Ʈ����Ʈ �̿��ؾߵ�
	// Tex�� ����Ʈ �ؽ��� ������ ������ ����� ���� -> ���̴����� �ϸ��
	// ���̴��� �ʿ��� �ؽ���? 
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
