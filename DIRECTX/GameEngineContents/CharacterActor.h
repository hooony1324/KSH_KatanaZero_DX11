#pragma once
#include <GameEngineCore/GameEngineActor.h>

// ���� : �÷��̾�(����, �巡��, ...), �Ǵ� ���� ����
class GameEngineTextureRenderer;
class GameEngineCollision;
class CharacterActor : public GameEngineActor
{
	// ���� �÷��̾ �ѹ� �ν��ϸ� ������ �Ѿƿ´�
public:
	static float4 CharacterPosition;

public:
	// constrcuter destructer
	CharacterActor();
	virtual ~CharacterActor();

	// delete Function
	CharacterActor(const CharacterActor& _Other) = delete;
	CharacterActor(CharacterActor&& _Other) noexcept = delete;
	CharacterActor& operator=(const CharacterActor& _Other) = delete;
	CharacterActor& operator=(CharacterActor&& _Other) noexcept = delete;

	inline bool IsRoomChangeAvailable()
	{
		return Right_UpBlue && Right_DownBlue;
	}

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

protected:
	void WallCheck();
	void FloatTimeCheck(float _DeltaTime);
	void LookCheck(float _InputOrVelocityDir);

// �̵� ����
protected:
	GameEngineTextureRenderer* Renderer_Character;
	GameEngineCollision* Collision_Character;
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
	bool DoubleDown;
	bool DoubleDownBlue;
	bool Left_Up;
	bool Left_Down;
	bool Right_UpBlue;
	bool Right_Up;
	bool Right_Down;
	bool Right_DownBlue;
	bool Left;
	bool Right;

	bool IsJump;
	bool IsFall; // �߷� �����ؼ� ������
	bool WallGrab;
	float FloatDeltaTime; // �� ���߿� �ִ� �ð���ŭ �߷� ���� ����

// ���� ����
protected:

};

