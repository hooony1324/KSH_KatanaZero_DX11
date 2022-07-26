#include "PreCompile.h"
#include "Room.h"

Room::Room() 
{

}

Room::~Room() 
{
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
