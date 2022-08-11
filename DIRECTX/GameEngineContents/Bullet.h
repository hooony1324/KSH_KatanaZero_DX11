#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class Bullet : public GameEngineActor
{
public:
	void Instance(float4 _Position, float4 _Dir);

public:
	// constrcuter destructer
	Bullet();
	virtual ~Bullet();

	// delete Function
	Bullet(const Bullet& _Other) = delete;
	Bullet(Bullet&& _Other) noexcept = delete;
	Bullet& operator=(const Bullet& _Other) = delete;
	Bullet& operator=(Bullet&& _Other) noexcept = delete;

protected:
	void ScreenOutCheckToDestroy();

protected:
	class GameEngineTextureRenderer* Renderer;
	class GameEngineCollision* Collision;
	float4 Dir;
	float MoveSpeed;
};

