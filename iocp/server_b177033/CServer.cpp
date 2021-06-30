#include "stdafx.h"


CServer::CServer(){}

CServer::~CServer(){}

BOOL CServer::InitServer()
{
	//���� �ʱ�ȭ
	if (WSAStartup(MAKEWORD(2, 2), &m_Wsadata) != 0)
	{
		CCLog::WriteLog(_T("WSAStartup Error\n"));
		return FALSE;
	}
	CCLog::WriteLog(_T("WSAStartup Success"));
	
	//������ ���� �ʱ�ȭ

	m_LisenSock = WSASocket(AF_INET, SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED);
	if (m_LisenSock == INVALID_SOCKET)
	{
		CCLog::WriteLog(_T("socket Error\n"));
		return FALSE;
	}
	CCLog::WriteLog(_T("Socket Init Success"));

	m_Iocp.CreateIocp();
	return TRUE;
}

//BOOL CServer::LisenBindServer()
//{
//	CThreadSync Sync;
//
//	int retval;
//	//bind
//	ZeroMemory(&m_ServerAddress, sizeof(m_ServerAddress));
//	m_ServerAddress.sin_family = AF_INET;
//	m_ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
//	m_ServerAddress.sin_port = htons(SERVERPORT);
//
//	retval = bind(m_LisenSock, (SOCKADDR *)&m_ServerAddress, sizeof(m_ServerAddress));
//	if (retval == SOCKET_ERROR) 
//	{
//		printf("bind error\n");
//		return FALSE;
//	}
//	// listen()
//	retval = listen(m_LisenSock, SOMAXCONN);
//	if (retval == SOCKET_ERROR)
//	{
//		printf("listen error\n");
//		return FALSE;
//	}
//	return TRUE;
//
//
//}
BOOL CServer::LisenBindServer()
{
	BOOL	optval = TRUE;
	int		retval = 0;
	// SO_REUSEADDR �̹� ��� ���� �ּҸ� BIND�� �� �ְ� ���� 
	// ���� ���� �� ����������� ���ε尡 �Ǿ�������� ��ٸ����ʰ� �ٷ� ���� �����ִ¿ɼ�
	retval = setsockopt(m_LisenSock, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval));
	if (retval == SOCKET_ERROR)
	{ 
		CCLog::WriteLog(_T("Socket Option Change ERROR : FAIL SO_REUSEADDR \n"));
		return FALSE;
	}
	// SO_CONDITIONAL_ACCEPT  ACCEPT ���� �Լ� ȣ�������� �������� �ʰ� ����
	// Acceptex���� Ŭ���̾�Ʈ�� �������� �ʴ� ������
	// listen ������ ���Ƿ� �Ҵ��� �ϱ� ������ ���� �����Ͽ� �ڵ��� listen()�� ����
	retval = setsockopt(m_LisenSock, SOL_SOCKET, SO_CONDITIONAL_ACCEPT, (char*)&optval, sizeof(optval));
	if (retval == SOCKET_ERROR)
	{
		CCLog::WriteLog(_T("Socket Option Chang EROOR : FAIL SO_CONDITIONAL_ACCEPT \n"));
		return FALSE;
	}
	//���� ���� �ּ� ������ �ʱ�ȭ
	ZeroMemory						  (&m_ServerAddress, sizeof(m_ServerAddress));
	m_ServerAddress.sin_family		= AF_INET;
	m_ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	m_ServerAddress.sin_port		= htons(SERVERPORT);

	//bind() 
	retval = bind(m_LisenSock, (SOCKADDR *)&m_ServerAddress, sizeof(m_ServerAddress));
	if (retval == SOCKET_ERROR)
	{
		CCLog::WriteLog(_T("bind ERROR \n"));
		return FALSE;
	}

	//Iocp�� m_LisenSock ���� AcceptEx()�� ó���ϱ� ���Ͽ� lisen ������ �������ش�.
	m_Iocp.ConnectIocp_ListenSocket(m_LisenSock);

	// listen()
	retval = listen(m_LisenSock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
	{
		CCLog::WriteLog(_T("listen ERROR \n"));
		return FALSE;
	}
	CCLog::WriteLog(_T("listen Success"));


	return TRUE;
}




//BOOL CServer::AcceptServer()
//{
//	CThreadSync Sync;
//
//	addrlen = sizeof(m_ClientAddress);
//	m_ClientSock = accept(m_LisenSock, (SOCKADDR *)&m_ClientAddress, &addrlen);
//	if (m_ClientSock == INVALID_SOCKET)
//	{
//		printf("accept ERROR \n");
//		return false;
//	}
//	printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
//		inet_ntoa(m_ClientAddress.sin_addr), ntohs(m_ClientAddress.sin_port));
//	return true;
//}
//BOOL CServer::GetClientSock()
//{
//	return m_ClientSock;
//}
//
