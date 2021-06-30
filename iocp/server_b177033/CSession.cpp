#include "stdafx.h"


OverLappedINFO::OverLappedINFO()
{
	m_pSession = NULL;
}

OverLappedINFO::~OverLappedINFO()
{
}

CSession::CSession()
{
	m_SessionSock = INVALID_SOCKET;
	m_Accept = new OverLappedINFO;
	m_Send = new OverLappedINFO;
	m_Recv = new OverLappedINFO;
	m_delete = new OverLappedINFO;
	m_ReadBuffer.Create(BUFSIZE);
	m_WriteBuffer.Create(BUFSIZE);
}

CSession::~CSession()
{
	closesocket(m_SessionSock);
	delete m_Accept;
	delete m_Send;
	delete m_Recv;
	delete m_delete;
}

BOOL CSession::AcceptExSession()
{
	int   retval = 0;

	//m_Accept OverLappedINFO 초기화
	ZeroMemory(&m_Accept->m_Overlapped, sizeof(OVERLAPPED));
	memset(buffer, 0, sizeof(buffer));
	m_Accept->m_Wsabuf.buf = buffer;
	m_Accept->m_Wsabuf.len = BUFSIZE;
	m_Accept->m_Workflag = IO_ACCEPT;
	m_Accept->m_pSession = (LPVOID) this;

	//m_SessionSock 소켓 생성
	m_SessionSock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (m_SessionSock == SOCKET_ERROR)
	{
		printf("m_SessionSock ERROR \n");
		return FALSE;
	}

	//AcceptEx 비동기 접속 요청 방식
	//쓰레드 하나를 고정해서 사용하지 않고 요청이 있을 때만 사용하여 쓰레드 효율적으로 사용
	retval = AcceptEx
	(
		Server.GetLisenSock(),						//연결 대기 서버 소켓
		m_SessionSock,								//클라이언트 연결 수용 소켓
		m_Accept->m_Wsabuf.buf,						//바로 데이터 받을 경우 사용하는 버퍼
		0,											//수신할 데이터의 크기 
		sizeof(SOCKADDR_IN) + 16,					//소켓 주소 구조체의 크기에 //로컬 주소 정보 용으로 예약된 바이트 수
		sizeof(SOCKADDR_IN) + 16,					//+16 값					  //원격 주소 정보 용으로 예약된 바이트 수
		NULL,										//연결 되자마자 받은 데이터의 크기
		&m_Accept->m_Overlapped						//WSAOVERAPPED 구조체의 포인터
	);

	if (retval == false && WSAGetLastError() != WSA_IO_PENDING)
	{
		CCLog::WriteLog(_T("AcceptEx EROOR "));
		return FALSE;
	}
	CCLog::WriteLog(_T("AcceptEx Success"));
	return TRUE;

}
//소켓 재활용
VOID CSession::CloseSocket()
{
	TransmitFile
	(
		m_SessionSock,								//연결된 소켓 핸들
		NULL,										//전송할 파일의 핸들
		NULL,										//파일로 전송할 크기
		NULL,										//한번 전송할 때 크기
		(LPOVERLAPPED)&m_delete->m_Overlapped,		//OVERLAPPED 구조체
		NULL,										//두번째 인자가 NULL일 경우 현재위치에서부터 전송
		TF_DISCONNECT | TF_REUSE_SOCKET				//파일 전송 관련 동작방식
	);
}

VOID CSession::GetAcceptINFO()
{
	SOCKADDR*	SockAddress = NULL;
	SOCKADDR*	RemoteAddress = NULL;
	int			SockAddressLen = 0;
	int			RemoteAddressLen = 0;

	BOOL		optval = TRUE;
	int			retval = 0;
	//AcceptEx를 통해 접속 완료한 세션의 주소를 받아 오는 함수
	GetAcceptExSockaddrs
	(
		m_Accept->m_Wsabuf.buf,						// AcceptEx 3번째 인자와 동일해야하는 수신 버퍼
		0,											// 첫 번째 데이터를 수신하는데 사용되는 버퍼의 크기
		sizeof(SOCKADDR_IN) + 16,					// AcceptEx 5번째 인자와 동일해야하는 로컬 주소 정보 용으로 예약된 바이트 수
		sizeof(SOCKADDR_IN) + 16,					// AcceptEx 6번째 인자와 동일해야하는 원격 주소 정보 용으로 예약된 바이트 수
		&SockAddress,								// 연결된 로컬 주소를 받는 구조체
		&SockAddressLen,							// 로컬 주소의 크기
		&RemoteAddress,								// 연결된 원격 주소를 받는 구조체
		&RemoteAddressLen							// 원격 주소의 크기
	);

	if (RemoteAddress != 0)
	{

		memcpy(&m_ServerAddress, SockAddress, sizeof(SOCKADDR_IN));
		memcpy(&m_SessionAddress, RemoteAddress, sizeof(SOCKADDR_IN));
		cout << "m_serverAddr  : " << inet_ntoa(m_ServerAddress.sin_addr) << endl;
		cout << "m_server port : " << htons(m_ServerAddress.sin_port) << endl;
		cout << "Session Addr  : " << inet_ntoa(m_SessionAddress.sin_addr) << endl;
		cout << "Session port  : " << htons(m_SessionAddress.sin_port) << endl;
	
		CCLog::WriteLog(_T("INFO Success"));
	}
	else
	{
		CCLog::WriteLog(_T("GetAcceptExSockaddrs ERROR \n"));
		return;
	}
	// Nagle 알고리즘 해제 작은 데이터 전송의 경우 꺼주면 속도 증가
	retval = setsockopt(m_SessionSock, IPPROTO_TCP, TCP_NODELAY, (char*)&optval, sizeof(optval));
	if (retval == SOCKET_ERROR)
	{
		CCLog::WriteLog(_T("TCP_NODELAY ERROR \n"));
		return;
	}

	Server.GetIocp().ConnectIocp_Session(this);
	SESSION.ActiveSession(this);
	//연결 완료 확인 후에 Recv를 실행 시켜 클라의 처음 데이터를 받는다.
	//CCLog::WriteLog(_T("GetAcceptINFO : %d"), m_SessionSock);

	Recv();
}
//패킷 확인 함수 패킷의 크기가 크거나 NULL 이면 false
bool CSession::InVaildPacket(LPVOID pPacket)
{
	//pPacket NULL 일 경우 에러
	if (pPacket == NULL) return false;
	//*packet 에 현재까지 읽은 데이터의 위치에 있는 패킷을 넣어 준다
	PacketHeader *packet = (PacketHeader*)pPacket;
	if (packet->size == 0) return false;
	//패킷의 크기가 초기에 받았던 패킷 크기와 일치하는 경우 true반환
	else if (packet->size <= receivedPaketSize) return true;
	return false;
}


VOID CSession::PreOnRecv(DWORD Databyte)
{
	CThreadSync Sync;
	receivedPaketSize = 0;
	//받은 데이터 저장 및 데이터 크기만큼 tail증가시켜준다.
	//if (m_ReadBuffer.Enqueue(m_Recv->m_Wsabuf.buf, Databyte))
	if (m_ReadBuffer.Enqueue(0, Databyte))
	{
		//받은 데이터 크기를 receivedPaketSize에 저장
		receivedPaketSize += Databyte;
	}
	while (1)
	{
		//받은 데이터 크기가 패킷해더 보다 작을 때까지 반복해서 파싱
		if (receivedPaketSize < PACKETHEADER)
		{
			break;
		}
		else
		{
			//패킷 크기 확인 함수
			if (InVaildPacket(m_ReadBuffer.GetRead()))
			{
				//현재 head의 위치를 패킷해더에 저장
				PacketHeader *pPacket = (PacketHeader*)m_ReadBuffer.GetRead();
				//패킷파싱
				PacketParsing(pPacket);
				receivedPaketSize -= pPacket->size;
			}
		}
	}
	if (!m_ReadBuffer.Dequeue(0, Databyte))
	{
		CCLog::WriteLog(_T("readBuffer.Dequeue() Error"));
		return;
	}
	//while문을 돌면서 받은 데이터를 모두 파싱하고 보냈으므로
	m_ReadBuffer.Clear();  //사용후 버퍼 삭제.
	receivedPaketSize = 0;
	Recv();
}

VOID CSession::PreOnSend(DWORD Databyte)
{
	
}

VOID CSession::Recv()
{
	CThreadSync Sync;
	int		retval = 0;
	DWORD	flag = 0;
	DWORD	receivedbytes = 0;

	//n_Recv OverLappedINFO 초기화
	ZeroMemory(&m_Recv->m_Overlapped, sizeof(OVERLAPPED));
	ZeroMemory(&m_Recv->m_Wsabuf, sizeof(WSABUF));
	m_Recv->m_Wsabuf.buf = m_ReadBuffer.GetWrite();
	m_Recv->m_Wsabuf.len = m_ReadBuffer.GetWritableSize();

	m_Recv->m_Workflag = IO_READ;
	m_Recv->m_pSession = this;

	retval = WSARecv(m_SessionSock, &m_Recv->m_Wsabuf, 1, &receivedbytes, &flag, &m_Recv->m_Overlapped, NULL);
	if (retval == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
	{
		CCLog::WriteLog(_T("WSARecv ERROR \n"));
		return;
	}
	CCLog::WriteLog(_T("WSARecv : %d"), m_SessionSock);
}

VOID CSession::Send(void *pPacket, DWORD size)
{
	CThreadSync Sync;
	int		retval = 0;
	DWORD	flag = 0;
	DWORD	sendbytes = 0;

	if (!m_WriteBuffer.Enqueue((char*)pPacket, size))
	{
		CCLog::WriteLog(_T("writeBuffer.Enqueue() Error"));
		return;
	}
	//
	ZeroMemory(&m_Send->m_Overlapped, sizeof(OVERLAPPED));
	ZeroMemory(&m_Send->m_Wsabuf, sizeof(WSABUF));

	m_Send->m_Wsabuf.buf = m_WriteBuffer.GetRead();
	m_Send->m_Wsabuf.len = m_WriteBuffer.GetDataSize();
	m_Send->m_Workflag = IO_SEND;

	retval = WSASend(m_SessionSock, &m_Send->m_Wsabuf, 1, &sendbytes, flag, &(m_Send->m_Overlapped), NULL);
	if (retval == 0)
	{
		printf("%s : %d 데이터전송 성공\n", inet_ntoa(m_SessionAddress.sin_addr), ntohs(m_SessionAddress.sin_port));
		m_WriteBuffer.Dequeue(0, size);

	}

	if (retval == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
	{
		CCLog::WriteLog(_T("WSARecv ERROR \n"));
		return;
	}
	
}

void CSession::Clear()
{
	m_SessionSock = INVALID_SOCKET;
	m_ReadBuffer.Clear();
	m_WriteBuffer.Clear();

}


