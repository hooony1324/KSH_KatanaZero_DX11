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
		NOWALL, // ����
		UP_WALL,
		RIGHT_WALL,
		RIGHT_PASS, // ���� �������� ��Ż
		LEFT_WALL,
		DOWN_WALL, // ����(�߷�X)
		DOWN_GROUND, // ���� ����
		DOWN_PASS, // ��� ����

		RIGHTUP_SLOPE,
		RIGHTDOWN_SLOPE,
		LEFTUP_SLOPE,
		LEFTDOWN_SLOPE,

	};

	STATE WallState;

protected:
	void PixelCheck();
	void VelocityCheck(float _DeltaTime);

protected:
	GameEngineTextureRenderer* Renderer_Character;
	float InitSpeed;
	float MoveSpeed; // �ӷ�
	float4 MoveDir;	// ����
	float4 Velocity; // �ӵ�
	float SumDeltaTime;

	float4 Down;
	float4 DoubleDown;
	float4 Up;
	float4 Left;
	float4 Right;
	float4 UpRight;
	float4 DownRight;
	float4 UpLeft;
	float4 DownLeft;
private:


};

