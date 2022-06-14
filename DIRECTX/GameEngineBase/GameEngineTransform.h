#pragma once
#include "GameEngineMath.h"
#include <DirectXCollision.h>
#include <GameEngineBase/GameEngineDebugObject.h>
#include <list>

enum CollisionType
{
	CT_POINT,
	CT_SPHERE, // 정방원
	CT_AABB, // 회전하지 않은 박스
	CT_OBB, // 회전한 박스
};

// 설명 :
class GameEngineTransform : public GameEngineDebugObject
{
public:
	// constrcuter destructer
	GameEngineTransform();
	~GameEngineTransform();

	// delete Function
	GameEngineTransform(const GameEngineTransform& _Other) = delete;
	GameEngineTransform(GameEngineTransform&& _Other) noexcept = delete;
	GameEngineTransform& operator=(const GameEngineTransform& _Other) = delete;
	GameEngineTransform& operator=(GameEngineTransform&& _Other) noexcept = delete;


public:
	inline void SetLocalScale(const float4& _Value)
	{
		CalculateWorldScale(_Value);

		for (GameEngineTransform* Child : Childs)
		{
			Child->CalculateWorldScale(Child->LocalScale);
			Child->CalculateWorldPosition(Child->LocalPosition);
		}

		CalculateWorld();
	}

	inline void SetLocalRotation(const float4& _Value)
	{
		CalculateWorldRotation(_Value);

		for (GameEngineTransform* Child : Childs)
		{
			Child->CalculateWorldRotation(Child->LocalRotation);
			Child->CalculateWorldPosition(Child->LocalPosition);
		}
		CalculateWorld();
	}

	inline void SetLocalPosition(const float4& _Value)
	{
		CalculateWorldPosition(_Value);

		for (GameEngineTransform* Child : Childs)
		{
			Child->CalculateWorldPosition(Child->LocalPosition);
		}

		CalculateWorld();
	}

	inline void SetWorldScale(const float4& _World)
	{
		float4 Local = _World;
		if (nullptr != Parent)
		{
			Local = _World / Parent->WorldScale;
		}

		CalculateWorldScale(Local);

		for (GameEngineTransform* Child : Childs)
		{
			Child->CalculateWorldScale(Child->LocalScale);
			Child->CalculateWorldPosition(Child->LocalPosition);
		}

		CalculateWorld();
	}

	inline void SetWorldRotation(const float4& _World)
	{
		float4 Local = _World;
		if (nullptr != Parent)
		{
			Local = _World - Parent->WorldRotation;
		}

		CalculateWorldRotation(Local);

		for (GameEngineTransform* Child : Childs)
		{
			Child->CalculateWorldRotation(Child->LocalRotation);
			Child->CalculateWorldPosition(Child->LocalPosition);
		}
		CalculateWorld();
	}

	inline void SetWorldPosition(const float4& _World)
	{
		float4 Local = _World;
		if (nullptr != Parent)
		{
			// 부모의 역행렬을 곱해서 
			Local = _World * Parent->WorldWorldMat.InverseReturn();
		}


		CalculateWorldPosition(Local);

		for (GameEngineTransform* Child : Childs)
		{
			Child->CalculateWorldPosition(Child->LocalPosition);
		}

		CalculateWorld();
	}

	inline void SetLocalRotate(const float4& _Value)
	{
		SetLocalRotation(LocalRotation + _Value);
	}

	inline void SetLocalMove(const float4& _Value)
	{
		SetLocalPosition(LocalPosition + _Value);
	}

	inline void SetWorldMove(const float4& _Value)
	{
		SetLocalPosition(WorldPosition + _Value);
	}

	inline float4 GetLocalScale() const
	{
		return LocalScale;
	}
	inline float4 GetLocalRotation() const
	{
		return LocalRotation;
	}
	inline float4 GetLocalPosition() const
	{
		return LocalPosition;
	}

	inline float4x4 GetLocalWorld() const
	{
		return LocalWorldMat;
	}

	inline float4x4 GetWorldWorld() const
	{
		return WorldWorldMat;
	}

	inline float4x4 GetWorldViewProjection() const
	{
		return WorldViewProjectMat;
	}

	inline float4 GetForwardVector() const
	{
		return WorldWorldMat.ArrV[2].NormalizeReturn();
	}

	inline float4 GetBackVector() const
	{
		return -(WorldWorldMat.ArrV[2].NormalizeReturn());
	}

	inline float4 GetUpVector() const
	{
		return WorldWorldMat.ArrV[1].NormalizeReturn();
	}

	inline float4 GetDownVector() const
	{
		return -(WorldWorldMat.ArrV[1].NormalizeReturn());
	}

	inline float4 GetRightVector() const
	{
		return WorldWorldMat.ArrV[0].NormalizeReturn();
	}

	inline float4 GetLeftVector() const
	{
		return -(WorldWorldMat.ArrV[0].NormalizeReturn());
	}

	void CalculateWorld();

	void CalculateWorldViewProjection();

	void SetParent(GameEngineTransform& _Child);

	// void PushChild(GameEngineTransform* _Child);

	void SetView(const float4x4& _Mat)
	{
		View = _Mat;
	}

	void SetProjection(const float4x4& _Mat)
	{
		Projection = _Mat;
	}

protected:

private:
	GameEngineTransform* Parent;
	std::list<GameEngineTransform*> Childs;

	// 로컬과 월드의 차이가 뭐냐 개념을 확실히 잡아야합니다..
	float4 LocalScale;
	float4 LocalRotation;
	float4 LocalPosition;

	// WorldScale.w 0
	float4 WorldScale;
	// WorldScale.w 0
	float4 WorldRotation;
	// WorldPosition.w 1
	float4 WorldPosition;

	float4x4 LocalScaleMat; // 크
	float4x4 LocalRotateMat; // 자
	float4x4 LocalPositionMat; // 이
	float4x4 LocalWorldMat;

	float4x4 View;
	float4x4 Projection;

	float4x4 WorldWorldMat; // 나의 로컬 world * 부모의 world
	float4x4 WorldViewMat;
	float4x4 WorldViewProjectMat;


	void CalculateWorldScale(const float4& _Local)
	{
		LocalScale = _Local;

		if (nullptr != Parent)
		{
			WorldScale = LocalScale * Parent->WorldScale;
		}
		else
		{
			WorldScale = LocalScale;
		}

		LocalScaleMat.Scale(LocalScale);
	}
	void CalculateWorldRotation(const float4& _Local)
	{
		LocalRotation = _Local;

		if (nullptr != Parent)
		{
			WorldRotation = LocalRotation + Parent->WorldRotation;
		}
		else
		{
			WorldRotation = LocalRotation;
		}

		LocalRotateMat.RotationDegree(LocalRotation);

	}

	void CalculateWorldPosition(const float4& _Local)
	{
		LocalPosition = _Local;

		if (nullptr != Parent)
		{
			WorldPosition = LocalPosition * Parent->WorldWorldMat;
		}
		else
		{
			WorldPosition = LocalPosition;
		}

		LocalPositionMat.Position(LocalPosition);
	}



	/////////////////////////// 충돌관련
public:
	static bool SphereToSphere(const GameEngineTransform& _Left, const GameEngineTransform& _Right);

	static bool AABBToAABB(const GameEngineTransform& _Left, const GameEngineTransform& _Right);

	static bool OBBToOBB(const GameEngineTransform& _Left, const GameEngineTransform& _Right);
};

