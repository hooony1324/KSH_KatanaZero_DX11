#include "PreCompile.h"
#include "EnemyBullet.h"
#include "EnemyActor.h"

#include "ReflectFX.h"
#include "BulletTrail.h"

EnemyBullet::EnemyBullet()
{
	MoveSpeed = 1500;
}

EnemyBullet::~EnemyBullet() 
{
}

GameEngineTextureRenderer* DebugD;

float4 RendererScale;
void EnemyBullet::Start()
{
	Renderer = CreateComponent<GameEngineTextureRenderer>();
	Renderer->SetTexture("spr_bullet_0.png");
	Renderer->ScaleToTexture();

	RendererScale = Renderer->GetTransform().GetLocalScale();
	Collision = CreateComponent<GameEngineCollision>();
	Collision->GetTransform().SetLocalScale({ 5, 5, GetDepth(ACTOR_DEPTH::FX) });
	Collision->ChangeOrder(COLLISIONGROUP::ENEMY_ATTACK);
	Collision->SetDebugSetting(CollisionType::CT_OBB2D, { 1, 1, 1, 0.5f });
	Collision->SetCollisionMode(CollisionMode::Ex);

	GetTransform().SetLocalScale({ 1, 1, 1 });

}

void EnemyBullet::Update(float _DeltaTime)
{
	float TimeScale = GameEngineTime::GetInst()->GetTimeScale(static_cast<int>(ACTORGROUP::TIMEGROUP_PARTICLE));
	float Ratio = std::clamp(TimeScale, 0.2f, 1.0f);
	Renderer->GetTransform().SetLocalScale({ RendererScale.x * Ratio, RendererScale.y, RendererScale.z });

	// Trail
	BulletTrail* Trail = GetLevel()->CreateActor<BulletTrail>();
	Trail->Spawn(GetTransform().GetWorldPosition(), Renderer->GetTransform().GetLocalScale(), 0.06f);

	PlayerSlashCheck();

	GetTransform().SetWorldMove(Dir * MoveSpeed * _DeltaTime);

	PixelWallCheck();
	ScreenOutCheckToDestroy();
}

void EnemyBullet::End()
{
}

void EnemyBullet::OnEvent()
{
	GetTransform().SetLocalScale({ 1, 1, 1 });
}

void EnemyBullet::PixelWallCheck()
{
	GameEngineTextureRenderer* CollisionMap = GlobalValueManager::ColMap;

	// 오른쪽 왼쪽
	float4 BulletColPos = Collision->GetTransform().GetWorldPosition();

	IsGreen = (CollisionMap->GetCurTexture()->GetPixelToFloat4(BulletColPos.ix(), -BulletColPos.iy())).CompareInt3D(float4::GREEN);
	if (true == IsGreen)
	{
		Death();
	}
}

void EnemyBullet::PlayerSlashCheck()
{
	Collision->IsCollisionEnterBase(CollisionType::CT_OBB2D, static_cast<int>(COLLISIONGROUP::PLAYER_ATTACK), CollisionType::CT_OBB2D, 
		[=](GameEngineCollision* _This, GameEngineCollision* _Other) 
		{
			// 총알끼리 부딪히면 무시
			if (nullptr != dynamic_cast<EnemyBullet*>(_Other->GetActor()))
			{
				return CollisionReturn::Break;
			}
			

			ReflectFX* Reflect = GetLevel()->CreateActor<ReflectFX>();
			Reflect->GetTransform().SetWorldPosition(GetTransform().GetWorldPosition());

			
			Collision->ChangeOrder(COLLISIONGROUP::PLAYER_ATTACK);

			std::list<GameEngineActor*> Enemies = GetLevel()->GetGroup(ACTORGROUP::TIMEGROUP_ENEMY);
			EnemyActor* Enemy = nullptr;
			float4 BulletPos = GetTransform().GetWorldPosition();
			BulletPos.z = 0;

			// Hp > 0 Enemy선택
			for (GameEngineActor* Ptr : Enemies)
			{
				Enemy = dynamic_cast<EnemyActor*>(Ptr);
				if (false == Enemy->IsDead())
				{
					Enemy = nullptr;
				}
			}

			if (nullptr != Enemy)
			{
				// 진행방향 적으로
				float4 EnemyVec = Enemy->GetTransform().GetWorldPosition() + float4{0, 30, 0};
				EnemyVec.z = 0;

				Dir = (EnemyVec - BulletPos).NormalizeReturn();
			}
			else
			{
				// 적 없으면 날아온 반대방향으로 
				float4 SlashVec = _Other->GetTransform().GetWorldPosition();
				SlashVec.z = 0;

				Dir.x *= -1;
				Dir.y *= -1;
			}
			// 회전
			
			float Rot = float4::VectorXYtoDegree(float4::ZERO, Dir);
			GetTransform().SetWorldRotation({ 0, 0, Rot });
			return CollisionReturn::Break;
		});
}
