#include "PreCompile.h"
#include "DiamondTransition.h"
#include <GameEngineCore/CoreMinimal.h>

size_t NumX = 40;
size_t NumY = 22;

DiamondTransition::DiamondTransition() 
	: ChangeBlack(false)
	, ChangeWhite(false)
	, Black(false)
{
}

DiamondTransition::~DiamondTransition() 
{
}

void DiamondTransition::Start()
{

	Diamonds.resize(22);

	for (size_t y = 0; y < NumY; y++)
	{
		for (size_t x = 0; x < NumX; x++)
		{
			GameEngineUIRenderer* Renderer = CreateComponent<GameEngineUIRenderer>();
			Renderer->SetPivot(PIVOTMODE::LEFTTOP);
			Renderer->SetSamplingModePoint();
			Renderer->GetTransform().SetLocalScale({ 32, 32, 2 });
			Renderer->CreateFrameAnimationFolder("changeblack", FrameAnimation_DESC{ "transition_in", 0.01f, false });
			Renderer->CreateFrameAnimationFolder("changewhite", FrameAnimation_DESC{ "transition_out", 0.01f, false });
			Renderer->ChangeFrameAnimation("changewhite");

			// 위치선정
			Renderer->GetTransform().SetLocalPosition({ x * 32.0f, y * -32.0f, 0 });
			Renderer->Off();

			Diamonds[y].push_back(Renderer);

			Renderer->AnimationBindEnd("changeblack", [=](const FrameAnimation_DESC& _Info)
				{
					//Renderer->SetTexture("spr_transition_square.png");
				});

			Renderer->AnimationBindEnd("changewhite", [=](const FrameAnimation_DESC& _Info)
				{
					Renderer->Off();
				});
		}
	}


	StateManager.CreateStateMember("White"
		, std::bind(&DiamondTransition::WhiteUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&DiamondTransition::WhiteStart, this, std::placeholders::_1)
	);

	StateManager.CreateStateMember("ChangeBlack"
		, std::bind(&DiamondTransition::ChangeBlackUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&DiamondTransition::ChangeBlackStart, this, std::placeholders::_1)
	);

	StateManager.CreateStateMember("ChangeWhite"
		, std::bind(&DiamondTransition::ChangeWhiteUpdate, this, std::placeholders::_1, std::placeholders::_2)
		, std::bind(&DiamondTransition::ChangeWhiteStart, this, std::placeholders::_1)
	);
	StateManager.ChangeState("White");



	Diamonds[0][NumX - 1]->AnimationBindEnd("changeblack", [=](const FrameAnimation_DESC& _Info)
		{
			Black = true;
			ChangeBlack = false;
		});

	Diamonds[0][NumX - 1]->AnimationBindEnd("changewhite", [=](const FrameAnimation_DESC& _Info)
		{
			Black = false;
			ChangeWhite = false;
		});
}

void DiamondTransition::WhiteStart(const StateInfo& _Info)
{

}

void DiamondTransition::WhiteUpdate(float _DeltaTime, const StateInfo& _Info)
{
}

int StartY;
float SumTime;
void DiamondTransition::ChangeBlackStart(const StateInfo& _Info)
{
	ChangeBlack = true;
	StartY = static_cast<int>(Diamonds.size()) - 1;
	SumTime = 0.0f;
}

void DiamondTransition::ChangeBlackUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (StartY < 0)
	{
		return;
	}


	for (int x = 0; x < NumX; x++)
	{
		Diamonds[StartY][x]->On();
		Diamonds[StartY][x]->ChangeFrameAnimation("changeblack");
	}

	StartY--;

}


void DiamondTransition::ChangeWhiteStart(const StateInfo& _Info)
{
	ChangeWhite = true;
	StartY = static_cast<int>(Diamonds.size()) - 1;
	SumTime = 0.0f;
}

void DiamondTransition::ChangeWhiteUpdate(float _DeltaTime, const StateInfo& _Info)
{
	if (StartY < 0)
	{
		return;
	}

	for (int x = 0; x < NumX; x++)
	{
		Diamonds[StartY][x]->On();
		Diamonds[StartY][x]->ChangeFrameAnimation("changewhite");
	}

	StartY--;
}

void DiamondTransition::Update(float _DeltaTime)
{
	StateManager.Update(_DeltaTime);
}

void DiamondTransition::End()
{
}


