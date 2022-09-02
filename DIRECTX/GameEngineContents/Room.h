#pragma once
#include <GameEngineCore/GameEngineActor.h>

// ���� :
class GameEngineTextureRenderer;
class CharacterActor;
class EnemyActor;
class Room : public GameEngineActor
{
public:
	static int CurRoomIndex;

	bool IsTimeLimit()
	{
		return TimeLimit;
	}

	const float GetTimeLimit()
	{
		return TimeLimitSeconds;
	}

protected:
	void SetCurTimeLimitSeconds(float _Seconds)
	{
		TimeLimitSeconds = _Seconds;
	}

public:
	// constrcuter destructer
	Room();
	virtual ~Room();

	// delete Function
	Room(const Room& _Other) = delete;
	Room(Room&& _Other) noexcept = delete;
	Room& operator=(const Room& _Other) = delete;
	Room& operator=(Room&& _Other) noexcept = delete;

	virtual void Setting() {}
	virtual void Clear() {}
	virtual void StairSetting() = 0;

	// �ش��ϴ� ������ Ŭ���������� ��������
	void SetCameraClampArea(float4& _LeftTop, float4& _RightBottom);
	void PlayerSpawn(CharacterActor* _Player);

	float4 CamClamp_Center;

	bool IsEnemyAllDead();
protected:
	void EmptyRoomInit();
	void InitCameraClampArea();

protected:
	// �� ���� �� �������־�� ��
	GameEngineTextureRenderer* BackBackGround;
	GameEngineTextureRenderer* Background;
	GameEngineTextureRenderer* Background_ColMap;

	float4 CamClamp_LeftTop;
	float4 CamClamp_RightBottom;

	float4 PlayerSpawnPos; 
	std::vector<EnemyActor*> Enemies;
	bool TimeLimit;
	float TimeLimitSeconds;

	CharacterActor* PlayerActor;

private:

};

