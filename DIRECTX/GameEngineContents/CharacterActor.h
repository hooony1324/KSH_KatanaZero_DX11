#pragma once
#include "LiveActor.h"


// ���� : �÷��̾�(����, �巡��, ...), �Ǵ� ���� ����
class GameEngineTextureRenderer;
class GameEngineCollision;
class CharacterActor : public LiveActor
{
public:
	void SetInputValid(bool _Val)
	{
		InputValid = _Val;
	}

	void SetDead()
	{
		//IsDead = true;
		Hp = 0;
		FlyVector = float4{ -0.3f, 0.3f };
		FlyVector.Normalize();
		PlayerStateManager.ChangeState("Dead");
	}

	inline bool IsRoomChangeAvailable()
	{
		return Red && !IsDead;
	}

	inline bool IsPlayerDead()
	{
		return IsDead;
	}

	bool IsPlayerDeadEnd()
	{
		return IsDead && DeadAniend;
	}

	void Respawn()
	{
		IsDead = false;
		Hp = 1;
		Collision_Character->On();
		Collision_Slash->Off();
		PlayerStateManager.ChangeState("Idle");
	}

	static void CheatModeSwitch()
	{
		CheatMode = !CheatMode;
	}

	static void CheatModeOn()
	{
		CheatMode = true;
	}

	static void CheatModeOff()
	{
		CheatMode = false;
	}

	bool RoomShakeActivate();


	// �� ����� ���� �ʱ�ȭ
	float4& GetMoveVector()
	{
		return MoveVec;
	}

	void SetCharacterIdle()
	{
		PlayerStateManager.ChangeState("Idle");
	}

	void CreateUIShadow();

	bool IsGround()
	{
		return !IsFall;
	}

	GameEngineTextureRenderer* GetRenderer()
	{
		return Renderer_Character;
	}

private:
	static bool CheatMode;

public:
	// constrcuter destructer
	CharacterActor();
	virtual ~CharacterActor();

	// delete Function
	CharacterActor(const CharacterActor& _Other) = delete;
	CharacterActor(CharacterActor&& _Other) noexcept = delete;
	CharacterActor& operator=(const CharacterActor& _Other) = delete;
	CharacterActor& operator=(CharacterActor&& _Other) noexcept = delete;

protected:
	void OnEvent() override;
	void OffEvent() override;

protected:
	enum class STATE_WALL
	{
		NONE,
		RIGHT,
		LEFT,
		UP,
		DOWN,
		UNDERGROUND,
		RIGHTSLOPE,
		LEFTSLOPE,
	};

	STATE_WALL WallState;

// ���� üũ
protected:
	// �ȼ� �浹
	void WallCheck();
	void PixelSetting();

	GameEngineTextureRenderer* CollisionMap;


	void FloatTimeCheck(float _DeltaTime);
	void LookCheck(float _InputOrVelocityDir);
	void StopAtDoor(float _Deltatime);

	// �ǰ�
	void EnemyAttackCheck();
	CollisionReturn Damaged(GameEngineCollision* _This, GameEngineCollision* _Other);
	CollisionReturn HitBullet(GameEngineCollision* _This, GameEngineCollision* _Other);

	GameEngineStateManager PlayerStateManager;

// �⺻
protected:
	
	GameEngineTextureRenderer* Renderer_Character;
	GameEngineCollision* Collision_Character;
	GameEngineTextureRenderer* Renderer_Slash;
	GameEngineCollision* Collision_Slash;

	GameEngineCollision* Collision_SlashingObject;
	bool CamShake;

	// ȿ��
	bool IsActivateSlashEffect(GameEngineCollision* _Left, GameEngineCollision* _Right);
	void CreateBrightShadow();

	GameEngineTextureRenderer* Renderer_SlashLight;
	class ParticleShooter* CloudShooter;


	bool InputValid;
	float4 InputDir;
	float4 CharacterPos;
	float InitSpeed;
	float MoveSpeed; // �ӷ�
	float4 MoveVec;	// ����

	float4 Velocity; // �ӵ�
	float4 GrabityForce; // �߷� ����

	int CurLookDir;	// �ü�
	int PrevLookDir;

	bool Up;
	bool MiddleDown;
	bool Down;
	bool DownBlue;
	bool DoubleDown;
	bool DoubleDownBlue;
	bool Left_Up;
	bool Left_Down;
	bool Right_Up;
	bool Right_Down;
	bool Red;
	bool Left;
	bool Right;
	bool Down_Left;
	bool Down_Right;

	bool IsJump;
	bool IsFall; // �߷� �����ؼ� ������
	bool WallGrab;
	float FloatDeltaTime; // �� ���߿� �ִ� �ð���ŭ �߷� ���� ����

	// ����
protected:
	int Hp;
	float FlyAngle;
	float4 FlyVector;
	bool IsDead;		// hurtground ani ���� true�� ��������
	bool DeadAniend;
	bool Invincible; // ������ ���� ����

	bool DoorBreaking;
	class Door* DoorPtr;

	// ����
protected:
	GameEngineSoundPlayer MoveSoundPlayer;
	GameEngineSoundPlayer SlashSoundPlayer;
	GameEngineSoundPlayer ReflectSoundPlayer;

	// �����
public:
	virtual void PushFrameCpaturedData() override;
	// �������� ����� �� ��, ���� �������� ���� ������� �������� Ŵ
	virtual void ReverseStartSetting() override;
	virtual void ReverseEndSetting() override;


private:
	// Pixel ��ġ
	Pix DownPix = { 0, -34 };
	Pix DownBluePix = { 0, -34 };
	Pix DoubleDownPix = { 0, -35 };
	Pix DoubleDownBluePix = { 0, -35 };
	Pix UpPix = { 0, 34 };
	Pix Left_UpPix = { -20, -0 };
	Pix Right_UpPix = { 20, -0 };
	Pix Right_DownPix = { 20, -35 };
	Pix Left_DownPix = { -20, -35 };
	Pix LeftPix = { -20, -34 };
	Pix RightPix = { 20, -34 };
	Pix RedPix = { 0, 0 };
	Pix Down_LeftPix = { -19, -35 };
	Pix Down_RightPix = { 19, -35 };
};

