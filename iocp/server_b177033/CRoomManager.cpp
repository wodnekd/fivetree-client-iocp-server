#include "stdafx.h"



CRoomManager::CRoomManager()
{
	roomCount = 0;
}

CRoomManager::~CRoomManager()
{
	Clear();
}
//·ë »ý¼º
bool CRoomManager::CreateRoom()
{
	CThreadSync Sync;
	/*if (roomNumber<MAX_ROOMCOUNT)
	{
		CRoom *Temp = new CRoom(true, false);
		if (Temp == NULL)
		{
			cout << "Room Create Fail" << endl;
			return false;
		}
		on_room.push_back(Temp);
		roomNumber++;
	}
	else
	{
		cout << "Full Room" << endl;
		return false;
	}*/
	if (roomCount<MAX_ROOMCOUNT)

		for (int i = 0; i<MAX_ROOMCOUNT; i++)
		{
			CRoom *Temp = new CRoom(true, false);
			if (Temp == NULL)
			{
				cout << "Room Create Fail" << endl;
				return false;
			}
			on_room.push_back(Temp);
			roomCount++;
		}
	else 
	{
		cout << "Full Room" << endl;
		return false;
	}
	return true;
}
//·ë ÆÄ±«
bool CRoomManager::DestroyRoom(CRoom *room) 
{
	CThreadSync Sync;
	if (roomCount >0)
	{
		for (iter = on_room.begin();iter != on_room.end(); ++iter)
		{
			if (*iter == room)
			{
				delete *iter;
				iter = on_room.erase(iter);
			}
		}
		off_room.push_back(room);
		--roomCount;
		return true;
	}
	else if (roomCount == 0)
	{
		cout << "Empty Room" << endl;
		return false;
	}
	return true;

}
VOID CRoomManager::Clear() 
{
	for (iter = on_room.begin();iter != on_room.end(); ++iter) 
	{
		delete *iter;
	}
	for (iter = off_room.begin();iter != off_room.end(); ++iter) 
	{
		delete *iter;
	}
	on_room.clear();
	off_room.clear();
}


