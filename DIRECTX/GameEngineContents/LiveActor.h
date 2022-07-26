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
	GameEngineTextureRenderer* Renderer_Character;
	float4 MoveDir;
	float MoveSpeed;

private:

};

