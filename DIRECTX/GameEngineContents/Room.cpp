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

	// room�ʱ�ȭ
}

void Room::RoomOff()
{
	Background->Off();

	// room����
}

