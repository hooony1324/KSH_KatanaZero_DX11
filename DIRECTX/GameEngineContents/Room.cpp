#include "PreCompile.h"
#include "Room.h"

Room::Room() 
{
}

Room::~Room() 
{
}

void Room::RoomOn()
{
	Background->On();

	// room초기화
}

void Room::RoomOff()
{
	Background->Off();

	// room제거
}

