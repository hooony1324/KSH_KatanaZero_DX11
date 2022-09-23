#pragma once
#include "GameEngineTransformComponent.h"
#include "GameEngineLevel.h"
#include "GameEngineCamera.h"

struct RenderOption 
{
    float DeltaTime = 0.0f;
    float SumDeltaTime = 0.0f;
	int Temp0;
	int Temp1;
};


// 추상클래스
// 강제 내 자식들을 하나의 인터페이스로 묶는 역할입니다.
// 설명 :
class GameEngineRenderingPipeLine;
class GameEngineShaderResourcesHelper;
class GameEngineRenderer : public GameEngineTransformComponent
{
	friend class GameEngineLevel;
	friend class GameEngineCamera;


public:
	RenderOption renderOption;

	// constrcuter destructer
	GameEngineRenderer();
	~GameEngineRenderer();

	// delete Function
	GameEngineRenderer(const GameEngineRenderer& _Other) = delete;
	GameEngineRenderer(GameEngineRenderer&& _Other) noexcept = delete;
	GameEngineRenderer& operator=(const GameEngineRenderer& _Other) = delete;
	GameEngineRenderer& operator=(GameEngineRenderer&& _Other) noexcept = delete;

	// float4x4 ViewPort;
	void ChangeCamera(CAMERAORDER _Order);

    GameEngineRenderingPipeLine* ClonePipeLine(GameEngineRenderingPipeLine* _Rendering);

    inline int GetRenderingOrder() 
    {
        return RenderingOrder;
    }

    void SetRenderingOrder(int _Order);

	virtual void InstancingOn() 
	{
		IsInstancing_ = true;
	};

	bool IsInstancing(GameEngineRenderingPipeLine* _Rendering);

	void InstancingDataSetting(GameEngineRenderingPipeLine* _Line);

	void EngineShaderResourcesSetting(GameEngineShaderResourcesHelper* _ShaderResources);


	
protected:
	virtual void Start();
	virtual void Update(float _DeltaTime) {}
	virtual void End() {}

	void PushRendererToMainCamera();
	void PushRendererToUICamera();

    class GameEngineCamera* Camera;



private:
	CAMERAORDER CameraOrder;
    int RenderingOrder;
	bool IsInstancing_;

	virtual void Render(float _DeltaTime) = 0;

};

