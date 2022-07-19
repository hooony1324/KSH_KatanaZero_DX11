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
}

void Room::RoomOff()
{
	Background->Off();
}

