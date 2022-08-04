#include "PreCompile.h"
#include "EnemyCop.h"
#include <GameEngineCore/CoreMinimal.h>

EnemyCop::EnemyCop() 
{
}

EnemyCop::~EnemyCop() 
{
}

void EnemyCop::Start()
{
	EnemyName = "cop";

	Renderer_Character = CreateComponent<GameEngineTextureRenderer>();
	Renderer_Character->SetTexture("spr_cop_aim_0.png");
	Renderer_Character->ScaleToTexture();
	CreateAllFolderAnimation();

	Collision_Character = CreateComponent<GameEngineCollision>();
	float4 RendererScale = Renderer_Character->GetTransform().GetLocalScale();
	Collision_Character->GetTransform().SetLocalScale({ RendererScale.x, RendererScale.y, GetDepth(ACTOR_DEPTH::COLLISION) });
	Collision_Character->ChangeOrder(COLLISIONGROUP::ENEMY);

	Collision_ChaseSensor = CreateComponent<GameEngineCollision>();
	Collision_ChaseSensor->GetTransform().SetLocalScale({ 400, 100, GetDepth(ACTOR_DEPTH::COLLISION) });

	GetTransform().SetLocalScale({ 2, 2, 1 });
}

void EnemyCop::Update(float _DeltaTime)
{
	// 피격 체크
	Collision_Character->IsCollision(CollisionType::CT_OBB2D, COLLISIONGROUP::PLAYER_ATTACK, CollisionType::CT_OBB2D,
		std::bind(&EnemyCop::PlayerAttackCheck, this, std::placeholders::_1, std::placeholders::_2)
	);
}

void EnemyCop::End()
{
}

bool EnemyCop::PlayerAttackCheck(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	if (nullptr == Collision_Character)
	{
		MsgBoxAssert("this의 콜리전이 nullptr입니다");
	}

	// 플레이어의 공격 위치를 받아서 반대로 날려짐
	float4 val = _Other->GetTransform().GetWorldPosition();

	return true;
}
