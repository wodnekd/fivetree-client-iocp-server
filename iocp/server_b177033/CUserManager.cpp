#include "stdafx.h"


CUserManager::CUserManager()
{
	clientNum = 0;

}


CUserManager::~CUserManager()
{
	Clear();
}
//접속한 유저 확인
BOOL CUserManager::Enter_User(CUserSession *user)
{
	CThreadSync Sync;
	//1.수정
	//userMapList.insert(map<SOCKET,  CUserSession*>::value_type(user->GetSessionSock(), user));
	userMapList.insert(pair<SOCKET, CUserSession*>(user->GetSessionSock(), user));
	clientNum++;
	
	return TRUE;
}
//떠난 유저 확인
BOOL CUserManager::Leave_User(CUserSession* leave) {
	CThreadSync Sync;
	//1.수정
	/*
	for (auto iter = userMapList.begin(); iter != userMapList.end();)
	{
		if (userMapList.find(leave->GetSessionSock()) != userMapList.end())
		{
			iter = userMapList.erase(iter);
		}
	}*/

	iter = userMapList.find(leave->GetSessionSock());
	if(iter != userMapList.end())
	{
		iter = userMapList.erase(iter);
	}
	clientNum--;
	//SESSION.InActiveSession(dynamic_cast<CSession*>(leave));
	return TRUE;
}

//전체 유저 리스트 해제
VOID CUserManager::Clear() 
{
	if (clientNum>0)	
		userMapList.clear();
}