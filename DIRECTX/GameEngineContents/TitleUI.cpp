#include "PreCompile.h"
#include "TitleUI.h"

TitleUI::TitleUI() 
{
}

TitleUI::~TitleUI() 
{
}

void TitleUI::LerpUpdate(float _DeltaTime)
{

	float4 Pos = float4::LerpLimit(Lerp_Start, Lerp_Dest, SumDeltaTime);
	GetTransform().SetWorldPosition(Pos);

	// �ε巯�� Lerp
	CurPos = GetTransform().GetTransformData().WorldPosition;
	float LerpSpeed = (Lerp_Dest - CurPos).Length() * Lerp_Speed;
	SumDeltaTime += _DeltaTime * LerpSpeed;
}

