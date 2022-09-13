#include "PreCompile.h"
#include "CustomPostEffect.h"
#include <GameEngineCore/CoreMinimal.h>


CustomPostEffect::CustomPostEffect() 
	: CopyTarget(nullptr)
{
}

CustomPostEffect::~CustomPostEffect() 
{
	if (nullptr != CopyTarget)
	{
		delete CopyTarget;
		CopyTarget = nullptr;
	}
}
