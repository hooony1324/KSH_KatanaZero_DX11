#include "PreCompile.h"
#include "Effect_PointLight.h"
#include <GameEngineCore/CoreMinimal.h>

#include "PlayLevel.h"
#include "CharacterActor.h"

Effect_PointLight* Effect_PointLight::Inst = nullptr;

Effect_PointLight::Effect_PointLight() 
{
}

Effect_PointLight::~Effect_PointLight() 
{
	Inst = nullptr;

	if (nullptr != RimTarget)
	{
		delete RimTarget;
		RimTarget = nullptr;
	}
}

void Effect_PointLight::EffectInit()
{
	Inst = this;

	CopyTarget = new GameEngineRenderTarget();
	CopyTarget->CreateRenderTargetTexture(GameEngineWindow::GetScale(), DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, float4::ZERO);
	RimTarget = new GameEngineRenderTarget();
	RimTarget->CreateRenderTargetTexture(GameEngineWindow::GetScale(), DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, float4::GREEN);

	EffectSet.SetPipeLine("PointLight");
	RimEffectSet.SetPipeLine("RimLight");

	OnOffOption = false;

	Option.DeltaTime = 0.0f;

	// PointLight
	if (true == EffectSet.ShaderResources.IsConstantBuffer("CustomRenderOption"))
	{
		EffectSet.ShaderResources.SetConstantBufferLink("CustomRenderOption", &Option, sizeof(Option));
		Option.OnOff = static_cast<int>(OnOffOption);
	}

	if (true == EffectSet.ShaderResources.IsConstantBuffer("TRANSFORMDATA"))
	{
		EffectSet.ShaderResources.SetConstantBufferLink("TRANSFORMDATA", &TData, sizeof(TData));
	}

	if (true == EffectSet.ShaderResources.IsConstantBuffer("PointLightData"))
	{
		EffectSet.ShaderResources.SetConstantBufferLink("PointLightData", &LightData, sizeof(LightData));
	}



	float4 LightWorldPos = { 400, -600 , GetDepth(ACTOR_DEPTH::BACKGROUND_4), 1.0f };
	LightData.LightWorldPos = LightWorldPos;


	// Rim Light (PointLigt�� ��ġ �ʿ�)
	float4 Size = GameEngineWindow::GetInst()->GetScale();
	ViewPortMatrix.ViewPort(Size.x, Size.y, 0.0f, 0.0f, 0.0f, 1.0f);

}

void Effect_PointLight::Effect(GameEngineRenderTarget* _Target)
{
	if (false == OnOffOption)
	{
		SumDeltaTime = 0.0f;
		return;
	}

	// ���� ȭ�� ī��
	CopyTarget->Copy(_Target);

	// ���̴��� ���� ����
	Option.OnOff = static_cast<int>(OnOffOption);
	float DeltaTime = GameEngineTime::GetDeltaTime();
	SumDeltaTime += DeltaTime;
	Option.DeltaTime = DeltaTime;
	Option.SumDeltaTime = SumDeltaTime;

	

	// PointLight ȭ����ġ ���
	ViewMatrix = MainCam->GetView();
	ProjectionMatrix = MainCam->GetProjectionMatrix();
	{
		float4 Pos = LightData.LightWorldPos;
		Pos *= ViewMatrix;
		Pos *= ProjectionMatrix;
		Pos *= ViewPortMatrix;
		LightData.LightScreenPos = Pos;
		TData.WorldViewProjectionMatrix = MainCam->GetTransform().GetWorldViewProjection();
	}


	// ũ�θ�Ű(��Ÿ��)->������Ʈ �ؽ��� �̾ƿ���
	//{
	//	RimTarget->Clear();
	//	RimTarget->Setting(); // Ÿ�� ��� �ʷϻ�


	//	// ����ȭ�鿡 ��µǴ� �÷��̾� ũ�θ�Ű Ÿ�ٿ� �׸�
	//	CharacterActor* Player = PlayLevel::PlayLevelInst->GetPlayer();
	//	GameEngineTextureRenderer* Renderer = Player->GetRenderer();
	//	
	//	RenderertoChromakey->GetTransform().SetLocalScale(Renderer->GetTransform().GetLocalScale());
	//	RenderertoChromakey->GetTransform().SetWorldPosition(Renderer->GetTransform().GetWorldPosition());


	//	RimEffectSet.ShaderResources.SetConstantBufferLink("Tex", Renderer->GetCurTexture());


	//	CopyTarget->Merge(RimTarget, 0);
	//}


	// ī�ǵ� ����ȭ�� �ؽ��� ����Ʈ����Ʈ���̴��� ����
	EffectSet.ShaderResources.SetTexture("Tex", CopyTarget->GetRenderTargetTexture(0));

	// ����Ʈ ����� ����ȭ�� ����
	_Target->Clear();
	_Target->Setting();
	_Target->Effect(EffectSet);

}
