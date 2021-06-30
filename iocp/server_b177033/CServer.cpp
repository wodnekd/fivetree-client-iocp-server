#include "stdafx.h"


CServer::CServer(){}

CServer::~CServer(){}

BOOL CServer::InitServer()
{
	//윈속 초기화
	if (WSAStartup(MAKEWORD(2, 2), &m_Wsadata) != 0)
	{
		CCLog::WriteLog(_T("WSAStartup Error\n"));
		return FALSE;
	}
	CCLog::WriteLog(_T("WSAStartup Success"));
	
	//서버용 소켓 초기화

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
	// SO_REUSEADDR 이미 사용 중인 주소를 BIND할 수 있게 설정 
	// 강제 종료 나 비정상종료시 바인드가 되어있을경우 기다리지않고 바로 해제 시켜주는옵션
	retval = setsockopt(m_LisenSock, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval));
	if (retval == SOCKET_ERROR)
	{ 
		CCLog::WriteLog(_T("Socket Option Change ERROR : FAIL SO_REUSEADDR \n"));
		return FALSE;
	}
	// SO_CONDITIONAL_ACCEPT  ACCEPT 유의 함수 호출전에는 응답하지 않게 설정
	// Acceptex사용시 클라이언트가 접속하지 않는 순간에
	// listen 소켓이 임의로 할당을 하기 떄문에 소켓 설정하여 자동적 listen()을 방지
	retval = setsockopt(m_LisenSock, SOL_SOCKET, SO_CONDITIONAL_ACCEPT, (char*)&optval, sizeof(optval));
	if (retval == SOCKET_ERROR)
	{
		CCLog::WriteLog(_T("Socket Option Chang EROOR : FAIL SO_CONDITIONAL_ACCEPT \n"));
		return FALSE;
	}
	//서버 소켓 주소 데이터 초기화
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

	//Iocp와 m_LisenSock 연결 AcceptEx()를 처리하기 위하여 lisen 소켓을 연결해준다.
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
//	printf("[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
//		inet_ntoa(m_ClientAddress.sin_addr), ntohs(m_ClientAddress.sin_port));
//	return true;
//}
//BOOL CServer::GetClientSock()
//{
//	return m_ClientSock;
//}
//
