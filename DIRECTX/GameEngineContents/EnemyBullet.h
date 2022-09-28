#pragma once
#include "Bullet.h"

// Ό³Έν :
class EnemyBullet : public Bullet
{

public:
	// constrcuter destructer
	EnemyBullet();
	~EnemyBullet();

	// delete Function
	EnemyBullet(const EnemyBullet& _Other) = delete;
	EnemyBullet(EnemyBullet&& _Other) noexcept = delete;
	EnemyBullet& operator=(const EnemyBullet& _Other) = delete;
	EnemyBullet& operator=(EnemyBullet&& _Other) noexcept = delete;


protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

	void OnEvent() override;

private:
	void PixelWallCheck();
	void PlayerSlashCheck();
	bool IsGreen;
};

