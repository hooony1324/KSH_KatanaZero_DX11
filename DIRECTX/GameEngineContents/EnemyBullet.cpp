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

	// ������ ����
	float4 BulletColPos = Collision->GetTransform().GetWorldPosition();

	IsGreen = CollisionMap->GetCurTexture()->GetPixelToPixelColor(BulletColPos.ix(), -BulletColPos.iy()) == GREEN;
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
			// �Ѿ˳��� �ε����� ����
			if (nullptr != dynamic_cast<EnemyBullet*>(_Other->GetActor()))
			{
				return true;
			}
			
			
			Collision->ChangeOrder(COLLISIONGROUP::PLAYER_ATTACK);

			std::list<GameEngineActor*> Enemies = GetLevel()->GetGroup(ACTORGROUP::TIMEGROUP_ENEMY);
			GameEngineActor* Enemy = nullptr;
			float4 BulletVec = GetTransform().GetWorldPosition();
			BulletVec.z = 0;

			// On Enemy����
			for (GameEngineActor* Ptr : Enemies)
			{
				if (true == Ptr->IsUpdate())
				{
					Enemy = Ptr;
					break;
				}
			}

			if (nullptr != Enemy)
			{
				// ������� ������
				float4 EnemyVec = Enemy->GetTransform().GetWorldPosition() + float4{0, 30, 0};
				EnemyVec.z = 0;

				Dir = (EnemyVec - BulletVec).NormalizeReturn();
			}
			else
			{
				// �� ������
				float4 SlashVec = _Other->GetTransform().GetWorldPosition();
				SlashVec.z = 0;

				Dir = (BulletVec - SlashVec).NormalizeReturn();
			}
			// ȸ��
			
			float Rot = float4::VectorXYtoDegree(float4::ZERO, Dir);
			GetTransform().SetWorldRotation({ 0, 0, Rot });
			return true; 
		});
}
