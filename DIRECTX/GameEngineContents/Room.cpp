#include "PreCompile.h"
#include "Room.h"

Room::Room() 
{
}

Room::~Room() 
{
}

void Room::RoomInit()
{
	Background = CreateComponent<GameEngineTextureRenderer>();
	Background->GetTransform().SetWorldPosition({ 0, 0, GetDepth(ACTOR_DEPTH::BACKGROUND_0) });

	Background_Collision = CreateComponent<GameEngineTextureRenderer>();
	Background_Collision->GetTransform().SetWorldPosition({ 0, 0, GetDepth(ACTOR_DEPTH::BACKGROUND_COL) });


	InitCameraClampArea();
}

void Room::InitCameraClampArea()
{
	float4 Scale = (Background->GetTransform().GetLocalScale() - float4{1280, 720}) / 2;
	CamClamp_LeftTop = { -Scale.x, Scale.y };
	CamClamp_RightBottom = { Scale.x, -Scale.y };
}

void Room::GetCameraClampArea(float4& _LeftTop, float4& _RightBottom)
{
	_LeftTop = CamClamp_LeftTop;
	_RightBottom = CamClamp_RightBottom;
}
