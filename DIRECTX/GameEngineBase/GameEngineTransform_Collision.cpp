#include "GameEngineTransform.h"
// #include <DirectXCollision.inl>

bool GameEngineTransform::SphereToSphere(const GameEngineTransform& _Left, const GameEngineTransform& _Right)
{
	DirectX::BoundingSphere SphereLeft;
	DirectX::BoundingSphere SphereRight;

	SphereLeft.Center = _Left.WorldPosition;
	SphereLeft.Radius = _Left.WorldScale.x * 0.5f;

	SphereRight.Center = _Right.WorldPosition;
	SphereRight.Radius = _Right.WorldScale.x * 0.5f;

	return SphereLeft.Intersects(SphereRight);
}

bool GameEngineTransform::AABBToAABB(const GameEngineTransform& _Left, const GameEngineTransform& _Right)
{
	DirectX::BoundingBox LeftData;
	DirectX::BoundingBox RightData;

	LeftData.Center = _Left.WorldPosition;
	LeftData.Extents = _Left.WorldScale * 0.5f;

	RightData.Center = _Right.WorldPosition;
	RightData.Extents = _Right.WorldScale * 0.5f;

	return LeftData.Intersects(RightData);
}

bool GameEngineTransform::OBBToOBB(const GameEngineTransform& _Left, const GameEngineTransform& _Right)
{
	DirectX::BoundingOrientedBox LeftData;
	DirectX::BoundingOrientedBox RightData;

	LeftData.Center = _Left.WorldPosition;
	LeftData.Extents = _Left.WorldScale * 0.5f;
	LeftData.Orientation = _Left.WorldRotation.DegreeRotationToQuaternionReturn();

	RightData.Center = _Right.WorldPosition;
	RightData.Extents = _Right.WorldScale * 0.5f;
	RightData.Orientation = _Right.WorldRotation.DegreeRotationToQuaternionReturn();

	return LeftData.Intersects(RightData);
}