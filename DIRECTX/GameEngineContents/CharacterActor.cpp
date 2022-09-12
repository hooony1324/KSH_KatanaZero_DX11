#include "PreCompile.h"
#include "CharacterActor.h"
#include "Bullet.h"
#include "SlashFX.h"
#include "Door.h"
#include "CharacterShadow.h"

bool CharacterActor::CheatMode = false;
const float FORCE_REACTION = 1.0f; // 반작용 강도



CharacterActor::CharacterActor()
	: MoveVec(float4::ZERO)
	, PrevLookDir(1)
	, CurLookDir(1)
	, Hp(1)
	, Invincible(false)
	, InputValid(true)
	, InputDir(float4::ZERO)
	, DoorBreaking(false)
	, DoorPtr(nullptr)
	, CamShake(false)
{

}

CharacterActor::~CharacterActor()
{
}

void CharacterActor::OnEvent()
{
	// 역재생
	if (nullptr == FrameDataRenderer)
	{
		LiveActor::FrameDataRenderer = CreateComponent<GameEngineTextureRenderer>();
	}

	InputValid = false;
	CurLookDir = 1;
	FrameDataRenderer->Off();
	InputDir = float4::ZERO;
	MoveVec = float4::ZERO;
	Velocity = float4::ZERO;
	Renderer_Slash->Off();
	WallGrab = false;
	DoorBreaking = false;
	DoorPtr = nullptr;
	CamShake = false;
}

void CharacterActor::OffEvent()
{
	InputDir = float4::ZERO;
	MoveVec = float4::ZERO;
	Velocity = float4::ZERO;
	Renderer_Slash->Off();
	WallGrab = false;
	DoorBreaking = false;
	DoorPtr = nullptr;
	CamShake = false;
}

void CharacterActor::WallCheck()
{
	GameEngineTextureRenderer* CollisionMap = GlobalValueManager::ColMap;
	if (nullptr == CollisionMap)
	{
		MsgBoxAssert("충돌맵이 nullptr 입니다");
	}

	// y값 반전 주의
	GlobalValueManager::PlayerPos = GetTransform().GetWorldPosition();
	CharacterPos = GlobalValueManager::PlayerPos;

	Down = (CollisionMap->GetCurTexture()->GetPixelToFloat4(CharacterPos.ix(), -(CharacterPos.iy() - 34))).CompareInt3D(float4::GREEN);
	DownBlue = (CollisionMap->GetCurTexture()->GetPixelToFloat4(CharacterPos.ix(), -(CharacterPos.iy() - 34))).CompareInt3D(float4::BLUE);
	Up = (CollisionMap->GetCurTexture()->GetPixelToFloat4(CharacterPos.ix(), -(CharacterPos.iy() + 34))).CompareInt3D(float4::GREEN);
	Left_Up = (CollisionMap->GetCurTexture()->GetPixelToFloat4(CharacterPos.ix() - 21, -(CharacterPos.iy() - 20 ))).CompareInt3D(float4::GREEN);
	Right_Up = (CollisionMap->GetCurTexture()->GetPixelToFloat4(CharacterPos.ix() + 21, -(CharacterPos.iy() - 20 ))).CompareInt3D(float4::GREEN);
	Right_Down = (CollisionMap->GetCurTexture()->GetPixelToFloat4(CharacterPos.ix() + 20, -(CharacterPos.iy() - 35))).CompareInt3D(float4::GREEN);
	Left_Down = (CollisionMap->GetCurTexture()->GetPixelToFloat4(CharacterPos.ix() - 20, -(CharacterPos.iy() - 35))).CompareInt3D(float4::GREEN);
	DoubleDownBlue = (CollisionMap->GetCurTexture()->GetPixelToFloat4(CharacterPos.ix(), -(CharacterPos.iy() - 35))).CompareInt3D(float4::BLUE);
	Left = (CollisionMap->GetCurTexture()->GetPixelToFloat4(CharacterPos.ix() - 20, -(CharacterPos.iy() - 30))).CompareInt3D(float4::GREEN);
	Right = (CollisionMap->GetCurTexture()->GetPixelToFloat4(CharacterPos.ix() + 20, -(CharacterPos.iy() - 30))).CompareInt3D(float4::GREEN);
	DoubleDown = (CollisionMap->GetCurTexture()->GetPixelToFloat4(CharacterPos.ix(), -(CharacterPos.iy() - 35))).CompareInt3D(float4::GREEN);
	Red = (CollisionMap->GetCurTexture()->GetPixelToFloat4(CharacterPos.ix(), -(CharacterPos.iy()))).CompareInt3D(float4::RED);


	// 땅에 박힘
	if (Down || DownBlue)
	{
		WallState = STATE_WALL::UNDERGROUND;
		IsFall = false;
		return;
	}

	// 머리 위 부딪힘
	if (Up)
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
	if (!Down && DoubleDown || !DownBlue && DoubleDownBlue)
	{
		WallState = STATE_WALL::DOWN;
		IsFall = false;
	}

	// 왼쪽 오른쪽 벽
	if (Left_Up && Left)
	{
		WallState = STATE_WALL::LEFT;
		return;
	}
	if (Right_Up && Right)
	{
		WallState = STATE_WALL::RIGHT;
		return;
	}

	// 슬로프 체크
	if (!Left_Up && Left && Left_Down)
	{
		IsFall = false;
		WallState = STATE_WALL::LEFTSLOPE;
	}

	if (!Right_Up && Right && Right_Down)
	{
		IsFall = false;
		WallState = STATE_WALL::RIGHTSLOPE;
	}

}

void CharacterActor::FloatTimeCheck(float _DeltaTime)
{
	if (true == IsFall)
	{
		FloatDeltaTime += _DeltaTime;
	}
	else
	{
		FloatDeltaTime = 0;
	}
}

void CharacterActor::LookCheck(float _InputOrVelocityDir)
{
	if (true == IsDead)
	{
		return;
	}

	// 왼쪽 오른쪽 바라보기
	if (_InputOrVelocityDir > 0)
	{
		CurLookDir = 1;
	}
	else if (_InputOrVelocityDir < 0)
	{
		CurLookDir = -1;
	}

	if (PrevLookDir > 0)
	{
		Renderer_Character->GetTransform().PixLocalPositiveX();
	}
	else
	{
		Renderer_Character->GetTransform().PixLocalNegativeX();
	}

	PrevLookDir = CurLookDir;
}

void CharacterActor::StopAtDoor(float _DeltaTime)
{
	// 문
	if (Collision_Character->IsCollision(CollisionType::CT_AABB2D, COLLISIONGROUP::DOOR, CollisionType::CT_AABB2D,
		nullptr))
	{
		if (MoveVec.x > 0)
		{
			MoveVec.x = -1.0f;
		}
		else
		{
			MoveVec.x = 1.0f;
		}
	}
}

void CharacterActor::EnemyAttackCheck()
{
	// 죽은 후에는 체크 안함
	if (Hp <= 0 || true == CheatMode || true == Invincible)
	{
		return;
	}

	Collision_Character->IsCollision(CollisionType::CT_AABB2D, COLLISIONGROUP::ENEMY_ATTACK, CollisionType::CT_AABB2D,
		std::bind(&CharacterActor::Damaged, this, std::placeholders::_1, std::placeholders::_2)
	);

}

bool CharacterActor::Damaged(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	// 공격 위치를 받아서 반대로 날려짐
	Hp--;
	if (Hp <= 0)
	{
		float4 This = _This->GetTransform().GetWorldPosition();
		This.z = 0;
		float4 Other = _Other->GetActor()->GetTransform().GetWorldPosition();
		Other.z = 0;
		FlyVector = This - Other;
		FlyVector.Normalize();
		PlayerStateManager.ChangeState("Dead");
	}

	return true;
}

bool CharacterActor::HitBullet(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	// 공격 위치를 받아서 반대로 날려짐
	Hp--;
	if (Hp <= 0)
	{
		float4 This = _This->GetTransform().GetWorldPosition();
		This.z = 0;
		float4 Other = _Other->GetActor()->GetTransform().GetWorldPosition();
		Other.z = 0;
		FlyVector = This - Other;
		FlyVector.Normalize();
		PlayerStateManager.ChangeState("Dead");
	}

	return true;
}


// 같은 콜리전 계속 부딛히면 안됨
static GameEngineCollision* CurCollision = nullptr;
bool CharacterActor::IsActivateSlashEffect(GameEngineCollision* _This, GameEngineCollision* _Other)
{
	if (CurCollision == _Other)
	{
		return false;
	}

	CurCollision = _Other;


	// 충돌 발생
	//SlashedCol = _Other;

	if (nullptr == DoorPtr)
	{
		float4 OtherPos = _Other->GetTransform().GetWorldPosition();
		float4 ThisPos = _This->GetTransform().GetWorldPosition();
		OtherPos.z = 0;
		ThisPos.z = 0;

		// 화면을 가로지르는 이펙트
		SlashFX* Fx = GetLevel()->CreateActor<SlashFX>(ACTORGROUP::NONE);
		Fx->GetTransform().SetWorldPosition(OtherPos);
		Fx->SetSlashLightDir(OtherPos - ThisPos);
	}


	return true;
}

bool CharacterActor::ShakeMainCamera()
{
	float4 SlashLightDir;

	Collision_Slash->IsCollision(CollisionType::CT_AABB2D, COLLISIONGROUP::DOOR, CollisionType::CT_AABB2D,
		[=](GameEngineCollision* _This, GameEngineCollision* _Other)
		{
			DoorPtr = dynamic_cast<Door*>(_Other->GetActor());
			if (nullptr != DoorPtr)
			{
				DoorPtr->Open();
				DoorBreaking = true;
				DoorPtr = nullptr;
				CamShake = true;
			}

			return false;
		});



	if (Collision_Slash->IsCollision(CollisionType::CT_OBB2D, COLLISIONGROUP::ENEMY, CollisionType::CT_OBB2D,
		std::bind(&CharacterActor::IsActivateSlashEffect, this, std::placeholders::_1, std::placeholders::_2)))
	{
		return true;
	}

	if (Collision_Slash->IsCollision(CollisionType::CT_OBB2D, COLLISIONGROUP::ENEMY_ATTACK, CollisionType::CT_OBB2D,
		std::bind(&CharacterActor::IsActivateSlashEffect, this, std::placeholders::_1, std::placeholders::_2)))
	{
		return true;
	}

	if (true == CamShake)
	{
		CamShake = false;
		return true;
	}

	return false;
}

static float BSSumTime;
void CharacterActor::CreateBrightShadow()
{
	if (BSSumTime > 0.004f)
	{
		BSSumTime = 0;

		// 그림자
		if (IsDead == false && abs(MoveVec.x) > 0 || abs(MoveVec.y) > 0)
		{
			// 현재 애니메이션의 텍스쳐 1장 복사
			CharacterShadow* Shadow = GetLevel()->CreateActor<CharacterShadow>();
			Shadow->DetachObject();
			Shadow->SetShadow(Renderer_Character->GetCurTexture(), PrevLookDir, GetTransform().GetWorldPosition() + float4{ 0, 0, 10 }, 0.12f);
			Shadow->On();
		}
	}
	else
	{
		BSSumTime += GameEngineTime::GetDeltaTime();
	}
}

void CharacterActor::PushFrameCpaturedData()
{
	// 역재생
	FrameCapturedData* Data = new FrameCapturedData();
	Data->Position = GetTransform().GetWorldPosition();
	Data->Texture = Renderer_Character->GetCurTexture();
	Data->TextureScale = Renderer_Character->GetTransform().GetLocalScale();
	CapturedDataList.push_back(Data);
}

void CharacterActor::ReverseStartSetting()
{
	Renderer_Character->Off();
	Renderer_Slash->Off();
	FrameDataRenderer->On();

	// 플레이어 업데이트 중지
	IsReverse = true;
}

void CharacterActor::ReverseEndSetting()
{
	IsReverse = false;
	FrameDataRenderer->Off();
	Renderer_Character->On();
}


// 슬로우상태, 어두운 화면일 때 그림자
void CharacterActor::CreateUIShadow()
{
	// 그림자
	if (IsDead == false)
	{
		// 현재 애니메이션의 텍스쳐 1장 복사
		CharacterShadow* Shadow = GetLevel()->CreateActor<CharacterShadow>();
		Shadow->DetachObject();
		Shadow->SetUIShadow(Renderer_Character->GetCurTexture(), PrevLookDir, GetTransform().GetWorldPosition(), 0.05f);
		Shadow->On();
	}

}
