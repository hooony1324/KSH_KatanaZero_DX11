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


	// Rim Light (PointLigt의 위치 필요)
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

	// 현재 화면 카피
	CopyTarget->Copy(_Target);

	// 쉐이더용 정보 갱신
	Option.OnOff = static_cast<int>(OnOffOption);
	float DeltaTime = GameEngineTime::GetDeltaTime();
	SumDeltaTime += DeltaTime;
	Option.DeltaTime = DeltaTime;
	Option.SumDeltaTime = SumDeltaTime;

	

	// PointLight 화면위치 계산
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


	// 크로마키(림타겟)->림라이트 텍스쳐 뽑아오기
	//{
	//	RimTarget->Clear();
	//	RimTarget->Setting(); // 타겟 배경 초록색


	//	// 현재화면에 출력되는 플레이어 크로마키 타겟에 그림
	//	CharacterActor* Player = PlayLevel::PlayLevelInst->GetPlayer();
	//	GameEngineTextureRenderer* Renderer = Player->GetRenderer();
	//	
	//	RenderertoChromakey->GetTransform().SetLocalScale(Renderer->GetTransform().GetLocalScale());
	//	RenderertoChromakey->GetTransform().SetWorldPosition(Renderer->GetTransform().GetWorldPosition());


	//	RimEffectSet.ShaderResources.SetConstantBufferLink("Tex", Renderer->GetCurTexture());


	//	CopyTarget->Merge(RimTarget, 0);
	//}


	// 카피된 현재화면 텍스쳐 포스트이펙트쉐이더에 전달
	EffectSet.ShaderResources.SetTexture("Tex", CopyTarget->GetRenderTargetTexture(0));

	// 이펙트 적용된 최종화면 적용
	_Target->Clear();
	_Target->Setting();
	_Target->Effect(EffectSet);

}
