#pragma once
//#include <GameEngineCore/GameEngineActor.h>
#include "LiveActor.h"

// 설명 :
class GameEngineTextureRenderer;
class Door : public LiveActor
{
public:
	void Close();
	void Open();

public:
	// constrcuter destructer
	Door();
	~Door();

	// delete Function
	Door(const Door& _Other) = delete;
	Door(Door&& _Other) noexcept = delete;
	Door& operator=(const Door& _Other) = delete;
	Door& operator=(Door&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

	void OnEvent() override;
	void OffEvent() override;

private:
	GameEngineTextureRenderer* Renderer;
	GameEngineCollision* Collision;
	GameEngineSoundPlayer SoundPlayer;

	// 역재생
public:
	// 플레이어, 총알, 등의 렌더러가 있는 자식에서 꼭 구현해야함
	virtual void PushFrameCpaturedData() override;

	// 렌더러가 역재생 될 때, 기존 렌더러는 끄고 역재생용 렌더러는 킴
	virtual void ReverseStartSetting() override;
	virtual void ReverseEndSetting() override;


};

