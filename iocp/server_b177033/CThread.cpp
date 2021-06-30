#include "stdafx.h"

BOOL CThread::CreateThread()
{
	GetSystemInfo(&m_Si);
	for (int i = 0; i<(int)m_Si.dwNumberOfProcessors * 2; i++)
	{
		m_HandleThread = (HANDLE)_beginthreadex(NULL, 0, CThread::WorkerThread, Server.GetIocp().GetHandle(), 0, NULL);
		if (m_HandleThread == NULL)
		{
			--i;
			CCLog::WriteLog(_T("CreateThread ERROR\n"));
			CloseHandle(m_HandleThread);
			continue;
		}
		ThreadListVector.push_back(m_HandleThread);
	}

	CCLog::WriteLog(_T("Thread Create %d"), ThreadListVector.size());
	return TRUE;
}
BOOL CThread::DestroyThread()
{
	for (int i = 0; i < (int)ThreadListVector.size(); ++i)
	{
		PostQueuedCompletionStatus(Server.GetIocp().GetHandle(), 0, 0, NULL);
	}
	for (int i = 0; i < (int)ThreadListVector.size(); ++i)
	{
		WaitForSingleObject(ThreadListVector[i], INFINITE);
		CloseHandle(ThreadListVector[i]);
	}
	
	ThreadListVector.clear();
	return TRUE;
}

unsigned int CThread::WorkerThread(LPVOID arg)
{
	CSession*		pSession		= NULL;
	DWORD			Transferred;
	CSession*		KeyValue		= 0;
	OverLappedINFO* pOverLappedInfo = NULL;
	BOOL			retval			= 0;
	HANDLE			hcp				= arg;

	while (1)
	{
		ZeroMemory(&pOverLappedInfo, sizeof(pOverLappedInfo));
		//GQCS ����� �Ϸ� ��⿭�� ���� ����� �Ϸ� ���
		retval = GetQueuedCompletionStatus
		(
			//Server.GetIocp().GetHandle(),					//IOCP �ڵ�
			hcp,
			&Transferred,									//����� �Ϸ� ���� ����� ���۵� ������ ũ��
			(PULONG_PTR)&KeyValue,							//IOCP���� �Ѱ��� ������ ����° ����
			(LPOVERLAPPED*)&pOverLappedInfo,				//OVERLAPPED ����ü
			INFINITE										//��⿭�� ��ٸ��� �ð�
		);													//���� TRUE, ���� FALSE ��ȯ
		
		//pOverLappedInfo = (OverLappedINFO*) Overlapped;
		//GQCS 5��° ���ڿ� ���� ���� ���
		if (retval == FALSE && WSAGetLastError() == WAIT_TIMEOUT)
		{
			CCLog::WriteLog(_T("GQCS Runnig Not Error"));
			continue;
		}
		//GQCS ERROR ���� �߻� ��
		if (KeyValue == 0 && pOverLappedInfo == 0)
		{
			CCLog::WriteLog(_T("GQCS ERROR"));
			return -1;
		}
		//GQCS�� AcceptEX() �Լ��� �Ϸ� ����� ������ ��� 
		else if (retval == TRUE && KeyValue == 0 && Transferred == 0 && pOverLappedInfo != 0)
		{
			pOverLappedInfo = (OverLappedINFO*)pOverLappedInfo;
			pSession = (CSession*)pOverLappedInfo->m_pSession;
			if (pOverLappedInfo->m_Workflag == IO_ACCEPT)
			{
				pSession->GetAcceptINFO();
				CCLog::WriteLog(_T("Accept Success"));
			}
		}
		//GQCS�� �ۼ��� ����
		else if (retval == TRUE && Transferred != 0 && KeyValue != 0 && pOverLappedInfo != 0)
		{
			pSession = (CSession*)pOverLappedInfo->m_pSession;
			if (pOverLappedInfo->m_Workflag == IO_READ)
			{
				pSession->PreOnRecv(Transferred);
			}
			else if (pOverLappedInfo->m_Workflag == IO_SEND)
			{
				pSession->PreOnSend(Transferred);
			}
		}

		//Ŭ���̾�Ʈ ���� ����
		else if (retval == TRUE && Transferred == 0)
		{
			pOverLappedInfo = (OverLappedINFO*)pOverLappedInfo;
			pSession = (CSession*)pOverLappedInfo->m_pSession;
			SESSION.InActiveSession(pSession);
			CCLog::WriteLog(_T("Client End"));
		}
		//Ŭ���̾�Ʈ ������ ����
		else if (retval == FALSE && Transferred == 0)
		{
			pOverLappedInfo = (OverLappedINFO*)pOverLappedInfo;
			pSession = (CSession*)pOverLappedInfo->m_pSession;
			SESSION.InActiveSession(pSession);
			CCLog::WriteLog(_T("Client Bed Bed Bed Escape"));
		}
	}
	return 0;
}
