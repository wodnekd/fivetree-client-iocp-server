#pragma once

class CUserManager : public CMultiThreadSync <CUserManager>
{
private:
	int clientNum;
	map<SOCKET,  CUserSession*>  userMapList;
	map<SOCKET,  CUserSession*>::iterator iter;

public:
	CUserManager();
	~CUserManager();
	BOOL Enter_User(CUserSession* user);
	BOOL Leave_User(CUserSession* leave);


	VOID Clear();
	// 맵 유저리스트 사용을 위한 함수
	std::map<SOCKET,  CUserSession*>  GetOnList()  {return userMapList;}
	int GetClientNum() { return clientNum; }
};

