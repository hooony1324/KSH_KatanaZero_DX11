#include "PreCompile.h"
#include "Cursor.h"
#include <GameEngineContents/ContentsCore.h>
#include "EnemyActor.h"

float4 Cursor::ScreenPosition = float4::ZERO;
float4 Cursor::WorldPosition = float4::ZERO;
float4 Cursor::WorldPositionToActor = float4::ZERO;
bool Cursor::CursorValid;

Cursor::Cursor() 
{
}

Cursor::~Cursor() 
{
}

void Cursor::Start()
{
	Renderer = CreateComponent<GameEngineUIRenderer>();
	Renderer->SetTexture("spr_cursor_0.png");
	Renderer->ScaleToTexture();

	GetTransform().SetLocalScale({1.5f, 1.5f, 1 });

	Boundary = ContentsCore::GetContentsWindowSize();

	//Collision = CreateComponent<GameEngineCollision>();
	//Collision->GetTransform().SetLocalScale({ 10, 10, 10000 });
	//Collision->ChangeOrder(COLLISIONGROUP::CURSOR);
	//Collision->SetDebugSetting(CollisionType::CT_AABB2D, float4::YELLOW);

}

void Cursor::Update(float _DeltaTime)
{
	ScreenPosition = GetLevel()->GetMainCamera()->GetMouseScreenPosition();
	WorldPosition = GetLevel()->GetMainCamera()->GetMouseWorldPosition();
	WorldPositionToActor = GetLevel()->GetMainCamera()->GetMouseWorldPositionToActor();
	GetTransform().SetWorldPosition({ WorldPosition.x, WorldPosition.y, GetDepth(ACTOR_DEPTH::CURSOR) });

	//float4 Pos = GetTransform().GetWorldPosition();
	//std::string Output = std::to_string(Pos.x) + " / " + std::to_string(Pos.y) + " / " + std::to_string(Pos.z);
	//GameEngineDebug::OutPutString(Output);
	CursorVisibleOutScreen();

	//if (true == GameEngineInput::GetInst()->IsDown("MouseMid") &&
	//	true == Collision->IsCollision(CollisionType::CT_AABB2D, COLLISIONGROUP::ENEMY, CollisionType::CT_AABB2D
	//		, nullptr))
	//{
	//	
	//}
}

void Cursor::End()
{

}

void Cursor::CursorVisibleOutScreen()
{

	if (ScreenPosition.x >= 0 && ScreenPosition.x <= 1280.0f
		&& ScreenPosition.y >= 0 && ScreenPosition.y <= 720.0f)
	{
		CursorValid = true;
		ShowCursor(false);
	}
	else
	{
		CursorValid = false;
		ShowCursor(true);
	}
}
