#include "PreCompile.h"
#include "TentacleKnife.h"
#include <GameEngineCore/CoreMinimal.h>

#include <GameEngineBase/GameEngineRandom.h>

const float IdleY = 270;
const float TakeOutY = -20;
const float LoadY = -20;
const float StabY = -360;

// LerpSpeed
const float FollowSpeed = 5.0f;
const float IdleSpeed = 8.0f;
const float TakeOutSpeed = 6.0f;
const float TakeInSpeed = 6.0f;
const float LoadSpeed = 20.0f;
const float StabSpeed = 25.0f;

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
	Renderer->ScaleToTexture();
	Renderer->Off();

	Collision = CreateComponent<GameEngineCollision>();
	Collision->GetTransform().SetLocalScale(Renderer->GetTransform().GetLocalScale());
	Collision->Off();

	GetTransform().SetWorldPosition({ 640, 270, GetDepth(ACTOR_DEPTH::BOSSFLOOR) });

	GetTransform().SetLocalRotation({ 0, 0, -90 });
	GetTransform().SetLocalScale({ 2, 2, 1 });


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
	Collision->On();
	IsStab = false;
	StateManager.ChangeState("Idle");
}

void TentacleKnife::OffEvent()
{
	Renderer->Off();
	Collision->Off();
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


void TentacleKnife::LoadStart(const StateInfo& _Info)
{
	IsStab = false;
}

void TentacleKnife::LoadUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (_Info.StateTime > 0.5f)
	{
		StateManager.ChangeState("Stab");
		return;
	}

	Position.y = GameEngineMath::Lerp(Position.y, LoadY, _DeltaTime * LoadSpeed);

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
		if (false == StabSoundPlayed)
		{
			int SoundIdx = GameEngineRandom::MainRandom.RandomInt(1, 2);
			StabSound = GameEngineSound::SoundPlayControl("sound_boss_akirasyringe_stab_0" + std::to_string(SoundIdx) + ".wav");
			StabSound.Volume(0.05f);
			StabSoundPlayed = true;
		}
		float ShakeX = sinf(DT * 10.0f * CamShakeSpeed) * powf(0.94f, DT * CamShakeSpeed);
		float ShakeY = sinf(DT * 10.0f * CamShakeSpeed) * powf(0.6f, DT * CamShakeSpeed);

		GetLevel()->GetMainCameraActor()->GetTransform().SetWorldMove({ ShakeX * 20, ShakeY * 20, 0 });
	}

	if (_Info.StateTime > 0.4f)
	{
		IsStab = false; // x축 움직임 가능
		Count--;

		if (Count > 0)
		{
			StabSoundPlayed = false;
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
