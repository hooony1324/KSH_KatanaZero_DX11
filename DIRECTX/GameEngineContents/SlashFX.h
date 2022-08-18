#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class SlashFX : public GameEngineActor
{
public:

	void SetSlashLightDir(float4 _Vector)
	{
		SlashDir = _Vector.NormalizeReturn();
		SlashLightStartPos = SlashDir * -500; 
		SlashLightStartPos.z = GetDepth(ACTOR_DEPTH::FX);

		float Degree = float4::VectorXYtoDegree(float4::ZERO, SlashDir);
		Renderer_SlashLight->GetTransform().SetWorldMove(SlashLightStartPos);
		Renderer_SlashLight->GetTransform().SetWorldRotation({ 0, 0, Degree });

		SlashStart = true;
	}
	
public:
	// constrcuter destructer
	SlashFX();
	~SlashFX();

	// delete Function
	SlashFX(const SlashFX& _Other) = delete;
	SlashFX(SlashFX&& _Other) noexcept = delete;
	SlashFX& operator=(const SlashFX& _Other) = delete;
	SlashFX& operator=(SlashFX&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;


private:
	GameEngineTextureRenderer* Renderer_SlashScar;
	GameEngineTextureRenderer* Renderer_SlashLight;
	bool SlashStart;
	float4 SlashDir;
	float4 SlashLightStartPos;

};

