#pragma once
#include <GameEngineCore/GameEngineActor.h>

// 설명 :
class Stair : public GameEngineActor
{
public:
	// 플레이어와 가장 가까운 단 하나의 계단
	static Stair* PlayerNearestStair;
	float PlayerDistance;

	void Spawn(float4 _WorldPos, Stair* _Up = nullptr, Stair* _Down = nullptr);
	
	// 1대 n 연결시 사용
	void AddDownStair(Stair* _Down)
	{
		DownStairs.push_back(_Down);
	}
	void AddUpStair(Stair* _Up)
	{
		UpStairs.push_back(_Up);
	}

public:
	// constrcuter destructer
	Stair();
	~Stair();

	// delete Function
	Stair(const Stair& _Other) = delete;
	Stair(Stair&& _Other) noexcept = delete;
	Stair& operator=(const Stair& _Other) = delete;
	Stair& operator=(Stair&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;


private:
	std::list<Stair*> UpStairs;
	std::list<Stair*> DownStairs;
	GameEngineCollision* Collision;
	GameEngineFontRenderer* FRenderer_Debug;

	bool PlayerOnFloor;
};

