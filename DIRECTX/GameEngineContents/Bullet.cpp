#include "PreCompile.h"
#include "Bullet.h"
#include "ContentsCore.h"

void Bullet::Instance(float4 _Position, float4 _Dir)
{
	GetTransform().SetWorldPosition({ _Position.x, _Position.y });
	Dir = _Dir;

	// 회전
	float4 Rot = float4::VectorXYtoDegree(float4::ZERO, _Dir);
	GetTransform().SetWorldRotation({ 0, 0, Rot.z });


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


void Bullet::ScreenOutCheckToDestroy()
{
	// 화면 밖 나가면 파괴
	float4 BulletPos = GetTransform().GetWorldPosition();
	if (BulletPos.x < -400 || BulletPos.x > 2000 || BulletPos.y > 400 || BulletPos.y < -2000)
	{
		Death();
	}

}
