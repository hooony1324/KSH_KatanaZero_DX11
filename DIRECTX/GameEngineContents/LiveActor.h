#pragma once
#include <GameEngineCore/GameEngineActor.h>

// ���� :
class LiveActor : public GameEngineActor
{
public:
	// constrcuter destructer
	LiveActor();
	virtual ~LiveActor();

	// delete Function
	LiveActor(const LiveActor& _Other) = delete;
	LiveActor(LiveActor&& _Other) noexcept = delete;
	LiveActor& operator=(const LiveActor& _Other) = delete;
	LiveActor& operator=(LiveActor&& _Other) noexcept = delete;

protected:
	enum class STATE
	{
		NONE,
		UP_WALL,
		RIGHT_WALL,
		RIGHT_PASS, // ���� �������� ��Ż
		LEFT_WALL,
		DOWN_WALL, // ����(�߷�X)
		UNDER_GROUND, // ���� ����
		UNDER_RIGHTUP_SLOPE,
		UNDER_LEFTUP_SLOPE,

		RIGHTUP_SLOPE,
		RIGHTDOWN_SLOPE,
		LEFTUP_SLOPE,
		LEFTDOWN_SLOPE,

	};

	STATE WallState;
	bool IsFly; // ����

protected:
	void PixelCheck();
	void WallCheck();
	void FloatCheck(float _DeltaTime);
	void LookCheck(float _InputOrVelocityDir);

protected:
	GameEngineTextureRenderer* Renderer_Character;
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
	bool Right_Up;
	bool Right_Down;
	bool Left;
	bool Right;

	bool IsJump;
	bool IsFall; // �߷� �����ؼ� ������
	float FloatDeltaTime; // �� ���߿� �ִ� �ð���ŭ �߷� ���� ����

};

