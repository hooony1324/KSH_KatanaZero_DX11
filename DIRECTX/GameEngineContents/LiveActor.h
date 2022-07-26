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
		UP_WALL,
		RIGHT_WALL,
		RIGHT_PASS, // ���� �������� ��Ż
		LEFT_WALL,
		DOWN_WALL,
		DOWN_PASS, // ��� ����
	};

	STATE WallState;

protected:
	void PixelCheck();
	void FallCheck(float _DeltaTime);

protected:
	GameEngineTextureRenderer* Renderer_Character;
	float4 MoveDir;	// ����
	float MoveSpeed; // �ӷ�
	float4 Velocity; // �ӵ�

	float4 Up;
	float4 Down;
	float4 Left;
	float4 Right;
	float4 UpRight;
	float4 DownRight;
	float4 UpLeft;
	float4 DownLeft;
private:


};

