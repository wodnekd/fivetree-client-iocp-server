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
		//GQCS 입출력 완료 대기열로 부터 입출력 완료 대기
		retval = GetQueuedCompletionStatus
		(
			//Server.GetIocp().GetHandle(),					//IOCP 핸들
			hcp,
			&Transferred,									//입출력 완료 연산 결과로 전송된 데이터 크기
			(PULONG_PTR)&KeyValue,							//IOCP에서 넘겨준 포인터 세번째 인자
			(LPOVERLAPPED*)&pOverLappedInfo,				//OVERLAPPED 구조체
			INFINITE										//대기열을 기다리는 시간
		);													//성공 TRUE, 실패 FALSE 반환
		
		//pOverLappedInfo = (OverLappedINFO*) Overlapped;
		//GQCS 5번째 인자에 값을 넣을 경우
		if (retval == FALSE && WSAGetLastError() == WAIT_TIMEOUT)
		{
			CCLog::WriteLog(_T("GQCS Runnig Not Error"));
			continue;
		}
		//GQCS ERROR 문제 발생 시
		if (KeyValue == 0 && pOverLappedInfo == 0)
		{
			CCLog::WriteLog(_T("GQCS ERROR"));
			return -1;
		}
		//GQCS에 AcceptEX() 함수의 완료 결과가 들어왔을 경우 
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
		//GQCS에 송수신 상태
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

		//클라이언트 정상 종료
		else if (retval == TRUE && Transferred == 0)
		{
			pOverLappedInfo = (OverLappedINFO*)pOverLappedInfo;
			pSession = (CSession*)pOverLappedInfo->m_pSession;
			SESSION.InActiveSession(pSession);
			CCLog::WriteLog(_T("Client End"));
		}
		//클라이언트 비정상 종료
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
