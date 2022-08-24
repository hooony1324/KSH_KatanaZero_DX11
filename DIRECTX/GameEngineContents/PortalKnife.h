#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class PortalKnife : public GameEngineActor
{
public:
	// constrcuter destructer
	PortalKnife();
	~PortalKnife();

	// delete Function
	PortalKnife(const PortalKnife& _Other) = delete;
	PortalKnife(PortalKnife&& _Other) noexcept = delete;
	PortalKnife& operator=(const PortalKnife& _Other) = delete;
	PortalKnife& operator=(PortalKnife&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

private:

};

