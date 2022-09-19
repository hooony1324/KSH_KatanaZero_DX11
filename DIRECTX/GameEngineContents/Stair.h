#pragma once
#include <GameEngineCore/GameEngineActor.h>

// ���� :
class Stair : public GameEngineActor
{
public:
	// �÷��̾�� ���� ����� �� �ϳ��� ���
	static Stair* PlayerNearestStair;
	float PlayerDistance;

	void Spawn(float4 _WorldPos, Stair* _Up = nullptr, Stair* _Down = nullptr);
	

	void SearchEnemyPassingDownStairs(float _EnemyPosY, std::vector<Stair*>& _PassbyStairs);

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
	Stair* UpStair;
	Stair* DownStair;
	GameEngineCollision* Collision;
	GameEngineFontRenderer* FRenderer_Debug;

	bool PlayerOnFloor;
};

