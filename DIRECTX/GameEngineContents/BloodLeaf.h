#pragma once
#include <GameEngineCore/GameEngineActor.h>

// 설명 :
class BloodLeaf : public GameEngineActor
{
public:
	void SetStartPoint(float4 _StartPoint)
	{
		StartPoint = _StartPoint;
		StartPoint.z = GetDepth(ACTOR_DEPTH::FX);
		GetTransform().SetWorldPosition(StartPoint);
	}

	void Respawn()
	{
		GetTransform().SetWorldPosition(StartPoint);
	}

	void SetLeafThick()
	{
		TurnThick = true;
	}

public:
	// constrcuter destructer
	BloodLeaf();
	~BloodLeaf();

	// delete Function
	BloodLeaf(const BloodLeaf& _Other) = delete;
	BloodLeaf(BloodLeaf&& _Other) noexcept = delete;
	BloodLeaf& operator=(const BloodLeaf& _Other) = delete;
	BloodLeaf& operator=(BloodLeaf&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

private:
	GameEngineUIRenderer* Leaf;
	float4 StartPoint;
	float4 MoveVec;
	float4 CurPos;
	float LeafSpeed;
	float TimeDiff;
	float SumDeltaTime;
	bool TurnThick; // 투명 -> 색
};

