#include "stdafx.h"

CIocp::CIocp(){}

CIocp::~CIocp(){}

BOOL CIocp::CreateIocp()
{
	// 입출력 완료 포트 생성
	m_IocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (m_IocpHandle == NULL)
	{
		CCLog::WriteLog(_T("Iocp Create error\n"));
		return FALSE;
	}
	return TRUE;
}

//AcceptEx를 위해 ListenSocket 연결
VOID CIocp::ConnectIocp_ListenSocket(SOCKET ListenSocket)
{
	CreateIoCompletionPort((HANDLE)ListenSocket, m_IocpHandle, (ULONG_PTR)0, 0);
}

//Iocp Session과 연결
VOID CIocp::ConnectIocp_Session(CSession* pSession)
{
	CreateIoCompletionPort((HANDLE)pSession->GetSessionSock(), m_IocpHandle, (ULONG_PTR)pSession, 0);
}