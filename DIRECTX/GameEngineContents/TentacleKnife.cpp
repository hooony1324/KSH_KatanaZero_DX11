#include "PreCompile.h"
#include "TentacleKnife.h"
#include <GameEngineCore/CoreMinimal.h>

#include <GameEngineBase/GameEngineRandom.h>

#include "BossFloor.h"
#include "PlayLevel.h"

const float IdleY = 300;
const float TakeOutY = -20;
const float LoadY = -30;
const float StabY = -360;

// LerpSpeed
const float FollowSpeed = 6.0f;
const float IdleSpeed = 8.0f;
const float TakeOutSpeed = 6.0f;
const float TakeInSpeed = 6.0f;
const float LoadSpeed = 30.0f;
const float StabSpeed = 35.0f;

void TentacleKnife::Idle()
{
	StateManager.ChangeState("Idle");
}

void TentacleKnife::TakeOut()
{
	StateManager.ChangeState("TakeOut");
}

void TentacleKnife::Load()
{
	StateManager.ChangeState("Load");
}

void TentacleKnife::StabAttackStart(int _Count)
{
	Count = _Count;
	StateManager.ChangeState("TakeOut");
}

TentacleKnife::TentacleKnife()
	: IsStab(false)
{
}

TentacleKnife::~TentacleKnife() 
{
}

void TentacleKnife::CreateAllState()
{
	StateManager.CreateStateMember("Idle"
		, std::bind(&TentacleKnife::IdleUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&TentacleKnife::IdleStart, this, std::placeholders::_1));

	StateManager.CreateStateMember("TakeOut"
		, std::bind(&TentacleKnife::TakeOutUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&TentacleKnife::TakeOutStart, this, std::placeholders::_1));

	StateManager.CreateStateMember("Load"
		, std::bind(&TentacleKnife::LoadUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&TentacleKnife::LoadStart, this, std::placeholders::_1));

	StateManager.CreateStateMember("Stab"
		, std::bind(&TentacleKnife::StabUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&TentacleKnife::StabStart, this, std::placeholders::_1));


	StateManager.CreateStateMember("TakeIn"
		, std::bind(&TentacleKnife::TakeInUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&TentacleKnife::TakeInStart, this, std::placeholders::_1));
}

void TentacleKnife::Start()
{
	Renderer = CreateComponent<GameEngineTextureRenderer>();
	Renderer->SetTexture("spr_psychboss_stabber_0.png");
	Renderer->SetSamplingModePoint();
	Renderer->SetScaleRatio(2.0f);
	Renderer->ScaleToTexture();
	Renderer->Off();
	Renderer->GetTransform().SetLocalRotation({ 0, 0, -90 });

	float4 RendererScale = Renderer->GetTransform().GetLocalScale();
	Collision = CreateComponent<GameEngineCollision>();
	Collision->GetTransform().SetLocalScale(float4(60, 60, GetDepth(ACTOR_DEPTH::COLLISION)));
	Collision->GetTransform().SetLocalPosition({ 0, -420 });
	Collision->ChangeOrder(COLLISIONGROUP::BOSSTENTACLE);
	Collision->SetDebugSetting(CollisionType::CT_AABB2D, float4(1.0f, 0.0f, 0.0f, 0.2f));

	// STATE
	CreateAllState();
}

void TentacleKnife::Update(float _DeltaTime)
{
	Position = GetTransform().GetWorldPosition();

	// X 좌표 플레이어에 조준
	if (false == IsStab)
	{
		Position.x = GameEngineMath::Lerp(Position.x, GlobalValueManager::PlayerPos.x, _DeltaTime * FollowSpeed);
	}

	
	float GroupDeltaScale = GameEngineTime::GetInst()->GetTimeScale(static_cast<int>(ACTORGROUP::TIMEGROUP));
	StateManager.Update(_DeltaTime * GroupDeltaScale);
	GetTransform().SetWorldPosition(Position);
}

void TentacleKnife::End()
{
}

void TentacleKnife::OnEvent()
{
	Renderer->On();
	IsStab = false;
	StateManager.ChangeState("Idle");
}

void TentacleKnife::OffEvent()
{
	Renderer->Off();
}

void TentacleKnife::IdleStart(const StateInfo& _Info)
{
	IsStab = false;
	IdleEnd = false;
}

void TentacleKnife::IdleUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (true == IdleEnd)
	{
		return;
	}

	Position.y = GameEngineMath::Lerp(Position.y, IdleY, _DeltaTime * IdleSpeed);

	if (_Info.StateTime > 1.5f)
	{
		IdleEnd = true;
	}
}

void TentacleKnife::TakeOutStart(const StateInfo& _Info)
{
	IsStab = false;
	AttackEnd = false;

	StabSound = GameEngineSound::SoundPlayControl("sound_boss_akirasyringe_appear_01.wav");
	StabSound.Volume(0.1f);
}

void TentacleKnife::TakeOutUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (_Info.StateTime > 1.2f)
	{
		StateManager.ChangeState("Stab");
		return;
	}

	Position.y = GameEngineMath::Lerp(Position.y, TakeOutY, _DeltaTime * TakeOutSpeed);
}


float RandomLoadY;
float RandomLoadTime;
void TentacleKnife::LoadStart(const StateInfo& _Info)
{
	IsStab = false;

	RandomLoadY = GameEngineRandom::MainRandom.RandomFloat(40, 150) * -1.0f;
	RandomLoadTime = GameEngineRandom::MainRandom.RandomFloat(0.18f, 0.3f);
}

void TentacleKnife::LoadUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (RandomLoadTime <= _Info.StateTime)
	{
		StateManager.ChangeState("Stab");
		return;
	}

	Position.y = GameEngineMath::Lerp(Position.y, RandomLoadY, _DeltaTime * LoadSpeed);

}

void TentacleKnife::StabStart(const StateInfo& _Info)
{
	IsStab = true;
	StabSoundPlayed = false;
}

float CamShakeSpeed = 10;
void TentacleKnife::StabUpdate(float _DeltaTime, const StateInfo& _Info)
{
	Position.y = GameEngineMath::Lerp(Position.y, StabY, _DeltaTime * StabSpeed);

	// 카메라 흔들림
	float DT = _Info.StateTime;
	if (abs(Position.y - StabY) < 5.0f)
	{
		PlayLevel::PlayLevelInst->ShakeRoom(false);
		if (false == StabSoundPlayed)
		{
			int SoundIdx = GameEngineRandom::MainRandom.RandomInt(1, 2);
			StabSound = GameEngineSound::SoundPlayControl("sound_boss_akirasyringe_stab_0" + std::to_string(SoundIdx) + ".wav");
			StabSound.Volume(0.1f);
			StabSoundPlayed = true;
		}

	}

	if (_Info.StateTime > 0.4f)
	{
		IsStab = false; // x축 움직임 가능
		Count--;

		if (Count > 0)
		{

			StateManager.ChangeState("Load");
		}
		else
		{
			StateManager.ChangeState("TakeIn");
		}
	}


	
}

void TentacleKnife::TakeInStart(const StateInfo& _Info)
{

}

void TentacleKnife::TakeInUpdate(float _DeltaTime, const StateInfo& _Info)
{
	Position.y = GameEngineMath::Lerp(Position.y, IdleY, _DeltaTime * TakeInSpeed);

	if (_Info.StateTime > 1.2f)
	{
		AttackEnd = true;
		StateManager.ChangeState("Idle");
	}
}
