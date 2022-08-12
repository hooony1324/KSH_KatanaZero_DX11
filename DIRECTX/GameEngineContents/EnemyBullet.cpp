#include "PreCompile.h"
#include "EnemyBullet.h"


EnemyBullet::EnemyBullet()
{
	MoveSpeed = 200;
}

EnemyBullet::~EnemyBullet() 
{
}

GameEngineTextureRenderer* DebugD;

void EnemyBullet::Start()
{
	Renderer = CreateComponent<GameEngineTextureRenderer>();
	Renderer->SetTexture("spr_bullet_0.png");
	Renderer->ScaleToTexture();

	float4 Scale = Renderer->GetTransform().GetLocalScale();
	Collision = CreateComponent<GameEngineCollision>();
	Collision->GetTransform().SetLocalScale({ 5, 5, GetDepth(ACTOR_DEPTH::FX) });
	Collision->GetTransform().SetLocalPosition({ Scale.x * 0.5f, 0, 0 });
	Collision->ChangeOrder(COLLISIONGROUP::ENEMY_ATTACK);
	Collision->SetDebugSetting(CollisionType::CT_OBB2D, { 1, 1, 1, 0.5f });

	GetTransform().SetLocalScale({ 1, 1, 1 });
}

void EnemyBullet::Update(float _DeltaTime)
{
	PlayerSlashCheck();

	GetTransform().SetWorldMove(Dir * MoveSpeed * _DeltaTime);

	PixelWallCheck();
	ScreenOutCheckToDestroy();
}

void EnemyBullet::End()
{
}

void EnemyBullet::PixelWallCheck()
{
	GameEngineTextureRenderer* CollisionMap = GlobalValueManager::ColMap;

	// 오른쪽 왼쪽
	float4 BulletColPos = Collision->GetTransform().GetWorldPosition();

	IsGreen = CollisionMap->GetCurTexture()->GetPixelToPixelColor(BulletColPos.x, -BulletColPos.y) == GREEN;
	if (true == IsGreen)
	{
		Death();
	}
}

void EnemyBullet::PlayerSlashCheck()
{
	Collision->IsCollision(CollisionType::CT_OBB2D, COLLISIONGROUP::PLAYER_ATTACK, CollisionType::CT_OBB2D, 
		[=](GameEngineCollision* _This, GameEngineCollision* _Other) 
		{
			// 총알끼리 부딪히면 무시
			if (nullptr != dynamic_cast<EnemyBullet*>(_Other->GetActor()))
			{
				return true;
			}
			
			
			Collision->ChangeOrder(COLLISIONGROUP::PLAYER_ATTACK);

			std::list<GameEngineActor*> Enemies = GetLevel()->GetGroup(ACTORGROUP::TIMEGROUP_ENEMY);
			
			if (Enemies.size() > 0)
			{
				// 진행방향 적으로
				GameEngineActor* Enemy = *Enemies.begin();
				float4 EnemyVec = Enemy->GetTransform().GetWorldPosition() + float4{0, 30, 0};
				EnemyVec.z = 0;
				float4 BulletVec = GetTransform().GetWorldPosition();
				Dir = (EnemyVec - BulletVec).NormalizeReturn();
				float Rot = float4::VectorXYtoDegree(float4::ZERO, Dir);
				GetTransform().SetWorldRotation({ 0, 0, Rot });
			}
			else
			{
				// 적 없으면
				Dir.x *= -1;
			}
			
			return true; 
		});
}
