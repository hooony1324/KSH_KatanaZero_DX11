#pragma once
#include <GameEngineCore/GameEngineActor.h>

class LightColors
{
public:
	LightColors();
	~LightColors() {}
public:
	static std::vector<float4> Colors;
};

// Ό³Έν :
class CharacterShadow : public GameEngineActor
{
public:
	static void SwitchShadowMode();

	void SetShadow(GameEngineTexture* _Texture, int ShadowDir, float4 _InstancePos, float _DeathTime = 0.12f);
	void SetUIShadow(GameEngineTexture* _Texture, int ShadowDir, float4 _InstancePos, float _DeathTime = 0.12f);

public:
	// constrcuter destructer
	CharacterShadow();
	~CharacterShadow();

	// delete Function
	CharacterShadow(const CharacterShadow& _Other) = delete;
	CharacterShadow(CharacterShadow&& _Other) noexcept = delete;
	CharacterShadow& operator=(const CharacterShadow& _Other) = delete;
	CharacterShadow& operator=(CharacterShadow&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

	void OnEvent() override;

private:
	GameEngineTextureRenderer* TexRenderer;
	GameEngineUIRenderer* UIRenderer;
	float SetDeathTime;
	static int ColorIndex;
	static int ShadowCount;
	static bool UIShadowOn;

};

