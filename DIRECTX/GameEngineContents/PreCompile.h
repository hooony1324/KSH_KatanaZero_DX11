#pragma once

// std and direct
#include <string>

#include <map>
#include <list>
#include <set>
#include <vector>
#include <chrono>

#include <Windows.h>

#include <d3d11_4.h>
#include <d3dcompiler.h>
#include <DirectXPackedVector.h>

#include <DirectXCollision.h>

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "dxguid")

// base
#include <GameEngineBase/GameEngineDebug.h>

// core
#include <GameEngineCore/CoreMinimal.h>
#include <GameEngineCore/GEngine.h>
#include <GameEngineCore/GameEngineCollision.h>
#include <GameEngineCore/GameEngineFont.h>
#include <GameEngineCore/GameEngineFontRenderer.h>
#include <GameEngineCore/GameEngineDevice.h>

// contents
#include <GameEngineContents/GlobalValueManager.h>
#include "GameContentsCustomRenderer.h"
#include "GameContentsEffectRenderer.h"
using namespace Depth;
