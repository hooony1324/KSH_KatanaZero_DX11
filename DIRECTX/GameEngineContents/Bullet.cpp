#include "PreCompile.h"
#include "Bullet.h"
#include "ContentsCore.h"

void Bullet::Instance(float4 _Position, float4 _Dir)
{
	GetTransform().SetWorldPosition({ _Position.x, _Position.y });
	Dir = _Dir;

	// È¸Àü
	float4 Rot = float4::VectorXYtoDegree(float4::ZERO, _Dir);
	Renderer->GetTransform().SetWorldRotation({ 0, 0, Rot.z });
	Collision->GetTransform().SetWorldRotation({ 0, 0, Rot.z });
}

Bullet::Bullet()
	: Renderer(nullptr)
	, Collision(nullptr)
	, Dir(float4::ZERO)
{
}

Bullet::~Bullet() 
{
}


void Bullet::BoundaryCheckAndDestroy()
{
	float4 CurPos = GetTransform().GetWorldPosition();
	if (CurPos.x < -400 || CurPos.x > 2000 || CurPos.y > 400 || CurPos.y < -2000)
	{
		Death();
	}
}
