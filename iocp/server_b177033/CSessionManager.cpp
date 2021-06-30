#include "stdafx.h"

CSessionManager::CSessionManager()
{
}

CSessionManager::~CSessionManager()
{
	delete userManager;
	delete RoomManager;
}

VOID CSessionManager::Init()
{
	userManager = new CUserManager;

	RoomManager = new CRoomManager;
	if (!RoomManager->CreateRoom()) return;

}

//Session 생성
BOOL CSessionManager::CreateSeccsion(int SessionNumber)
{
	for (int i = 0; i < SessionNumber; i++)
	{
		CSession* pSession = new CUserSession();
		//CSession* pSession = new CSession();
		if (pSession->AcceptExSession() == FALSE)
		{
			i--;
			delete pSession;
			continue;
		}
		//생성된 Session 비활성 리스트에 넣어 줌
		InActiveSessionList.push_back(pSession);
	}

	return TRUE;
}

//활성화 Session
VOID CSessionManager::ActiveSession(CSession * pSession)
{
	CThreadSync Sync;

	for (std::list<CSession*>::iterator iter = InActiveSessionList.begin(); iter != InActiveSessionList.end(); iter++)
	{
		if ((*iter) == pSession)
		{
			//사용할 Session을 활성화 맵에 넣어 주고 비활성 리스트에서 지워 줌
			//ActiveSessionListMap.insert(std::map<SOCKET, CSession*>::value_type(pSession->GetSessionSock(), pSession));
			ActiveSessionListMap.insert(pair<SOCKET, CSession*>(pSession->GetSessionSock(), pSession));
			iter = InActiveSessionList.erase(iter);
			break;
		}
	}
	CCLog::WriteLog(_T("Active_Session : %d"), ActiveSessionListMap.size());
	CCLog::WriteLog(_T("InActive_Session : %d"), InActiveSessionList.size());
}

//비활성화 Session
VOID CSessionManager::InActiveSession(CSession * pSession)
{
	CThreadSync Sync;

	//활성화 map 에서 pSession 을 찾아 iter에 넣어줌
	std::map<SOCKET, CSession*>::iterator iter = ActiveSessionListMap.find(pSession->GetSessionSock());
	
	//iter 값을 활성화 리스트 맵에서 제거하고 비활성화 리스트에 다시 넣어줌
	if (iter != ActiveSessionListMap.end())
	{
		ActiveSessionListMap.erase(iter);
		InActiveSessionList.push_back(pSession);

	}
	CCLog::WriteLog(_T("Active_Session : %d"), ActiveSessionListMap.size());
	CCLog::WriteLog(_T("InActive_Session : %d"), InActiveSessionList.size());
	//소켓을 닫고 다시 접속 허용
	//1
	//pSession->Clear();
	pSession->AcceptExSession();
}

VOID CSessionManager::Clear()
{
	if (InActiveSessionList.size() > 0)
	{
		for (std::list<CSession*>::iterator iter = InActiveSessionList.begin();iter != InActiveSessionList.end(); ++iter)
		{
			delete *iter;
			iter = InActiveSessionList.erase(iter);
		}
		InActiveSessionList.clear();
	}
	if (ActiveSessionListMap.size() > 0)
	{
		for (std::map<SOCKET, CSession*>::iterator iter = ActiveSessionListMap.begin();iter != ActiveSessionListMap.end(); ++iter)
		{
			delete iter->second;
			iter = ActiveSessionListMap.erase(iter);
		}
		ActiveSessionListMap.clear();
	}
	delete userManager;
	delete RoomManager;
}

