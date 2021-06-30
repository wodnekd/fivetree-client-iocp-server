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

//Session ����
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
		//������ Session ��Ȱ�� ����Ʈ�� �־� ��
		InActiveSessionList.push_back(pSession);
	}

	return TRUE;
}

//Ȱ��ȭ Session
VOID CSessionManager::ActiveSession(CSession * pSession)
{
	CThreadSync Sync;

	for (std::list<CSession*>::iterator iter = InActiveSessionList.begin(); iter != InActiveSessionList.end(); iter++)
	{
		if ((*iter) == pSession)
		{
			//����� Session�� Ȱ��ȭ �ʿ� �־� �ְ� ��Ȱ�� ����Ʈ���� ���� ��
			//ActiveSessionListMap.insert(std::map<SOCKET, CSession*>::value_type(pSession->GetSessionSock(), pSession));
			ActiveSessionListMap.insert(pair<SOCKET, CSession*>(pSession->GetSessionSock(), pSession));
			iter = InActiveSessionList.erase(iter);
			break;
		}
	}
	CCLog::WriteLog(_T("Active_Session : %d"), ActiveSessionListMap.size());
	CCLog::WriteLog(_T("InActive_Session : %d"), InActiveSessionList.size());
}

//��Ȱ��ȭ Session
VOID CSessionManager::InActiveSession(CSession * pSession)
{
	CThreadSync Sync;

	//Ȱ��ȭ map ���� pSession �� ã�� iter�� �־���
	std::map<SOCKET, CSession*>::iterator iter = ActiveSessionListMap.find(pSession->GetSessionSock());
	
	//iter ���� Ȱ��ȭ ����Ʈ �ʿ��� �����ϰ� ��Ȱ��ȭ ����Ʈ�� �ٽ� �־���
	if (iter != ActiveSessionListMap.end())
	{
		ActiveSessionListMap.erase(iter);
		InActiveSessionList.push_back(pSession);

	}
	CCLog::WriteLog(_T("Active_Session : %d"), ActiveSessionListMap.size());
	CCLog::WriteLog(_T("InActive_Session : %d"), InActiveSessionList.size());
	//������ �ݰ� �ٽ� ���� ���
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

