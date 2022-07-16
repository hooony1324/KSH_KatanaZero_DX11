#include "PreCompile.h"
#include "GlobalValueManager.h"

Obstacle* GlobalValueManager::Actors::testObstacle = nullptr;

std::vector<std::string> GlobalValueManager::PlayerAnimations = { 
	"attack", "fall", "idle", "idle_to_run", "jump", "postcrouch", 
	"precrouch", "roll", "run", "run_to_idle", "wallgrab", "wallslide"};