#pragma once
//#include <GameEngineCore/GameEngineActor.h>
#include "LiveActor.h"

// ���� :
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

	// �����
public:
	// �÷��̾�, �Ѿ�, ���� �������� �ִ� �ڽĿ��� �� �����ؾ���
	virtual void PushFrameCpaturedData() override;

	// �������� ����� �� ��, ���� �������� ���� ������� �������� Ŵ
	virtual void ReverseStartSetting() override;
	virtual void ReverseEndSetting() override;


};

