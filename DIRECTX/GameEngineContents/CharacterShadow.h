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
	void SetShadow(GameEngineTexture* _Texture, float _DeathTime, int ShadowDir, float4 _InstancePos);

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
	GameEngineTextureRenderer* Renderer;
	float SetDeathTime;
	static int ColorIndex;

};

