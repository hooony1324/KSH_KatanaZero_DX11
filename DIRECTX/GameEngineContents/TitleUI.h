#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class TitleUI : public GameEngineActor
{
public:
	// constrcuter destructer
	TitleUI();
	~TitleUI();

	// delete Function
	TitleUI(const TitleUI& _Other) = delete;
	TitleUI(TitleUI&& _Other) noexcept = delete;
	TitleUI& operator=(const TitleUI& _Other) = delete;
	TitleUI& operator=(TitleUI&& _Other) noexcept = delete;

	void LerpStart(float4 _DestPos, float _Speed)
	{
		SumDeltaTime = 0;
		Lerp_Start = { GetTransform().GetWorldPosition().x, GetTransform().GetWorldPosition().y };
		Lerp_Dest = _DestPos;
		Lerp_Speed = _Speed;
	}

	void LerpStart(float4 _StartPos, float4 _DestPos, float _Speed)
	{
		SumDeltaTime = 0;
		Lerp_Start = _StartPos;
		Lerp_Dest = _DestPos;
		Lerp_Speed = _Speed / (_StartPos - _DestPos).Length();
	}

protected:
	void LerpUpdate(float _DeltaTime);

private:
	// Lerp
	float Lerp_Speed;
	float SumDeltaTime;
	float4 Lerp_Start;
	float4 Lerp_Dest;
	float4 CurPos;
};

