#include "PreCompile.h"
#include "BloodLeaf.h"
#include <GameEngineCore/CoreMinimal.h>

#include <GameEngineBase/GameEngineRandom.h>



BloodLeaf::BloodLeaf()
	: StartPoint(float4::ZERO)
	, TurnThick(false)
{
}

BloodLeaf::~BloodLeaf() 
{
}

void BloodLeaf::Start()
{
	Leaf = CreateComponent<GameEngineUIRenderer>();
	Leaf->SetSamplingModePoint();
	Leaf->SetFolderTextureToIndex("spr_blood", (UINT)GameEngineRandom::MainRandom.RandomInt(0, 6));
	Leaf->SetScaleRatio(GameEngineRandom::MainRandom.RandomFloat(1.5f, 2.3f));
	Leaf->ScaleToTexture();

	float ColorStrength[] = { 0.5f, 0.8f, 0.8f, 1.0f, 1.0f };
	Leaf->GetPixelData().MulColor.r = ColorStrength[GameEngineRandom::MainRandom.RandomInt(0, 4)];
	Leaf->GetPixelData().MulColor.a = 0.0f;

	float LeafSpeeds[] = { 70, 80, 100, 120 };
	LeafSpeed = LeafSpeeds[GameEngineRandom::MainRandom.RandomInt(0, 3)];

	SumDeltaTime = 0.0f;
	TimeDiff = GameEngineRandom::MainRandom.RandomFloat(0.5f, 1.5f);
}

void BloodLeaf::Update(float _DeltaTime)
{
	if (true == TurnThick && Leaf->GetPixelData().MulColor.a <= 1.0f)
	{
		Leaf->GetPixelData().MulColor.a += _DeltaTime * 0.3f;
	}

	SumDeltaTime += _DeltaTime * TimeDiff;
	CurPos = GetTransform().GetWorldPosition();

	// 알파값 조정


	// 흩날리기
	float CurCos = cosf(SumDeltaTime); // X축 움직임
	MoveVec.x = -abs(CurCos);
	MoveVec.y = -abs(sinf(SumDeltaTime));

	GetTransform().SetAddWorldRotation(float4{ 0, 0, 360 * _DeltaTime * -CurCos });
	GetTransform().SetWorldMove(MoveVec * _DeltaTime * LeafSpeed);

	if (CurPos.y < -370.0f || CurPos.x < -650.f)
	{
		Respawn();
	}

}

void BloodLeaf::End()
{
}

