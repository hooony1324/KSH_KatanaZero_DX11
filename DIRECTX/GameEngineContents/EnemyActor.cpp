#include "PreCompile.h"
#include "EnemyActor.h"
#include <GameEngineCore/CoreMinimal.h>

EnemyActor::EnemyActor() 
	: CurAction(ENEMYACTION::PATROL)
	, EnemyName(std::string())
{
}

EnemyActor::~EnemyActor() 
{
	
}

void EnemyActor::OnEvent()
{
	if (nullptr == Collision_ChaseSensor)
	{
		MsgBoxAssert("Enemy의 콜리전 센서가 nullptr 입니다");
	}
}

void EnemyActor::WallCheck()
{
	GameEngineTextureRenderer* CollisionMap = GlobalValueManager::ColMap;
	if (nullptr == CollisionMap)
	{
		MsgBoxAssert("충돌맵이 nullptr 입니다");
	}


	// y값 반전 주의
	float4 CharacterPos = GetTransform().GetWorldPosition();
	Down = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix(), -(CharacterPos.iy() - 36))
		.CompareInt3D(float4::GREEN);
	Left_Up = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix() - 34, -(CharacterPos.iy() + 37))
		.CompareInt3D(float4::GREEN);
	Right_Up = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix() + 34, -(CharacterPos.iy() + 37))
		.CompareInt3D(float4::GREEN);
	Right_Down = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix() + 34, -(CharacterPos.iy() - 37))
		.CompareInt3D(float4::GREEN);
	Left_Down = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix() - 34, -(CharacterPos.iy() - 37))
		.CompareInt3D(float4::GREEN);
	DoubleDown = CollisionMap->GetCurTexture()->GetPixel(CharacterPos.ix(), -(CharacterPos.iy() - 37))
		.CompareInt3D(float4::GREEN);

	// 벽 체크
	{
		// 땅에 박힘
		if (Down)
		{
			WallState = STATE_WALL::UNDERGROUND;
			IsFall = false;
			return;
		}

		// 머리 위 부딪힘
		if (Right_Up || Left_Up)
		{
			WallState = STATE_WALL::UP;
			IsFall = true;
			return;
		}

		// 공중
		if (!Down && !DoubleDown && !Left_Down && !Right_Down)
		{
			WallState = STATE_WALL::NONE;
			IsFall = true;
		}

		// 바닥 
		if (!Down && DoubleDown)
		{
			WallState = STATE_WALL::DOWN;
			IsFall = false;
		}

		// 왼쪽 오른쪽 벽
		if (Left_Up)
		{
			WallState = STATE_WALL::LEFT;
			return;
		}
		if (Right_Up)
		{
			WallState = STATE_WALL::RIGHT;
			return;
		}

		// 슬로프 체크
		if (!IsFall && Left && Left_Down)
		{
			WallState = STATE_WALL::LEFTSLOPE;
		}

		if (!IsFall && Right && Right_Down)
		{
			WallState = STATE_WALL::RIGHTSLOPE;
		}
	}
}

void EnemyActor::ChooseAction()
{
	// 플레이어가 Chase거리 밖에 있다 -> PATROL
	// 플레이어가 Chase거리 안에 있다 -> Chase
	// Chase이고 플레이어과의 거리가 AttackRange 안이다 -> Attack






	//PrevAction = CurAction;
	CurAction = ENEMYACTION::PATROL;
}

void EnemyActor::Move(float _DeltaTime)
{



	switch (WallState)
	{
	case EnemyActor::STATE_WALL::NONE:
		break;
	case EnemyActor::STATE_WALL::RIGHT:
		break;
	case EnemyActor::STATE_WALL::LEFT:
		break;
	case EnemyActor::STATE_WALL::UP:
		MoveVec.y = -0.02f;
		break;
	case EnemyActor::STATE_WALL::DOWN:
		break;
	case EnemyActor::STATE_WALL::UNDERGROUND:
		MoveVec.y = 1;
		break;
	case EnemyActor::STATE_WALL::RIGHTSLOPE:
		break;
	case EnemyActor::STATE_WALL::LEFTSLOPE:
		break;
	default:
		break;
	}

	Velocity = MoveVec * MoveSpeed * _DeltaTime;
	GetTransform().SetWorldMove(Velocity);
}

void EnemyActor::CreateAllFolderAnimation()
{
	if (EnemyName.empty())
	{
		MsgBoxAssert("Enemy가 자신의 이름을 설정하지 않을 채로 애니메이션을 생성하려고 했습니다");
	}
	
	Renderer_Character->CreateFrameAnimationFolder("idle", FrameAnimation_DESC{ EnemyName + "_idle", 0.1125f });
	Renderer_Character->CreateFrameAnimationFolder("attack", FrameAnimation_DESC{ EnemyName + "_attack", 0.05f , false });
	Renderer_Character->CreateFrameAnimationFolder("walk", FrameAnimation_DESC{ EnemyName + "_walk", 0.1125f });
	Renderer_Character->CreateFrameAnimationFolder("turn", FrameAnimation_DESC{ EnemyName + "_turn", 0.1125f, false });
	Renderer_Character->CreateFrameAnimationFolder("run", FrameAnimation_DESC{ EnemyName + "_run", 0.1125f });
	Renderer_Character->CreateFrameAnimationFolder("hurtfly", FrameAnimation_DESC{ EnemyName + "_hurtfly", 0.1125f, false });
	Renderer_Character->CreateFrameAnimationFolder("hurtground", FrameAnimation_DESC{ EnemyName + "_hurtground", 0.1125f, false });


}


