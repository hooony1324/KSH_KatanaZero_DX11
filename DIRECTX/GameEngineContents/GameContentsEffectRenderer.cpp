#include "PreCompile.h"
#include "GameContentsEffectRenderer.h"
#include <GameEngineCore/CoreMinimal.h>

GameContentsEffectRenderer::GameContentsEffectRenderer() 
{
}

GameContentsEffectRenderer::~GameContentsEffectRenderer() 
{
}

void GameContentsEffectRenderer::SetWaveEffect()
{
	SetPipeLine("TextureWave");

	if (true == ShaderResources.IsConstantBuffer("CustomRenderOption"))
	{
		ShaderResources.SetConstantBufferLink("CustomRenderOption", &CustomOption, sizeof(CustomOption));
	}

	if (true == ShaderResources.IsConstantBuffer("AtlasData"))
	{
		ShaderResources.SetConstantBufferLink("AtlasData", &AtlasDataInst, sizeof(AtlasDataInst));
	}

	AtlasDataInst.FrameData.PosX = 0.0f;
	AtlasDataInst.FrameData.PosY = 0.0f;
	AtlasDataInst.FrameData.SizeX = 1.0f;
	AtlasDataInst.FrameData.SizeY = 1.0f;
	AtlasDataInst.PivotPos = float4::ZERO;


	CustomOption.DeltaTime = 0.0f;
	CustomOption.IsMask = 0;
	CustomOption.SumDeltaTime = 0.0f;
	CustomOption.Version = 0;
}

void GameContentsEffectRenderer::SetConcaveEffect()
{
	SetPipeLine("TextureConcave");

	if (true == ShaderResources.IsConstantBuffer("CustomRenderOption"))
	{
		ShaderResources.SetConstantBufferLink("CustomRenderOption", &CustomOption, sizeof(CustomOption));
	}

	if (true == ShaderResources.IsConstantBuffer("AtlasData"))
	{
		ShaderResources.SetConstantBufferLink("AtlasData", &AtlasDataInst, sizeof(AtlasDataInst));
	}

	AtlasDataInst.FrameData.PosX = 0.0f;
	AtlasDataInst.FrameData.PosY = 0.0f;
	AtlasDataInst.FrameData.SizeX = 1.0f;
	AtlasDataInst.FrameData.SizeY = 1.0f;
	AtlasDataInst.PivotPos = float4::ZERO;


	CustomOption.DeltaTime = 0.0f;
	CustomOption.IsMask = 0;
	CustomOption.SumDeltaTime = 0.0f;
	CustomOption.Version = 0;
}

void GameContentsEffectRenderer::SetPivot(PIVOTMODE _Mode)
{
	switch (_Mode)
	{
	case PIVOTMODE::CENTER:
		AtlasDataInst.PivotPos = float4::ZERO;
		break;
	case PIVOTMODE::BOT:
		AtlasDataInst.PivotPos = float4(0.0f, 0.5f, 0.0f, 0.0f);
		break;
	case PIVOTMODE::TOP:
		AtlasDataInst.PivotPos = float4(0.0f, -0.5f, 0.0f, 0.0f);
		break;
	case PIVOTMODE::LEFT:
		AtlasDataInst.PivotPos = float4(0.5f, 0.0f, 0.0f, 0.0f);
		break;
	case PIVOTMODE::RIGHT:
		AtlasDataInst.PivotPos = float4(-0.5f, 0.0f, 0.0f, 0.0f);
		break;
	case PIVOTMODE::LEFTTOP:
		AtlasDataInst.PivotPos = float4(0.5f, -0.5f, 0.0f, 0.0f);
		break;
	case PIVOTMODE::RIGHTTOP:
		AtlasDataInst.PivotPos = float4(-0.5f, -0.5f, 0.0f, 0.0f);
		break;
	case PIVOTMODE::LEFTBOT:
		AtlasDataInst.PivotPos = float4(0.5f, 0.5f, 0.0f, 0.0f);
		break;
	case PIVOTMODE::RIGHTBOT:
		AtlasDataInst.PivotPos = float4(-0.5f, 0.5f, 0.0f, 0.0f);
		break;
	case PIVOTMODE::CUSTOM:
		break;
	default:
		break;
	}
}
