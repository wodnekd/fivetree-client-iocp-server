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

	//m_Accept OverLappedINFO �ʱ�ȭ
	ZeroMemory(&m_Accept->m_Overlapped, sizeof(OVERLAPPED));
	memset(buffer, 0, sizeof(buffer));
	m_Accept->m_Wsabuf.buf = buffer;
	m_Accept->m_Wsabuf.len = BUFSIZE;
	m_Accept->m_Workflag = IO_ACCEPT;
	m_Accept->m_pSession = (LPVOID) this;

	//m_SessionSock ���� ����
	m_SessionSock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (m_SessionSock == SOCKET_ERROR)
	{
		printf("m_SessionSock ERROR \n");
		return FALSE;
	}

	//AcceptEx �񵿱� ���� ��û ���
	//������ �ϳ��� �����ؼ� ������� �ʰ� ��û�� ���� ���� ����Ͽ� ������ ȿ�������� ���
	retval = AcceptEx
	(
		Server.GetLisenSock(),						//���� ��� ���� ����
		m_SessionSock,								//Ŭ���̾�Ʈ ���� ���� ����
		m_Accept->m_Wsabuf.buf,						//�ٷ� ������ ���� ��� ����ϴ� ����
		0,											//������ �������� ũ�� 
		sizeof(SOCKADDR_IN) + 16,					//���� �ּ� ����ü�� ũ�⿡ //���� �ּ� ���� ������ ����� ����Ʈ ��
		sizeof(SOCKADDR_IN) + 16,					//+16 ��					  //���� �ּ� ���� ������ ����� ����Ʈ ��
		NULL,										//���� ���ڸ��� ���� �������� ũ��
		&m_Accept->m_Overlapped						//WSAOVERAPPED ����ü�� ������
	);

	if (retval == false && WSAGetLastError() != WSA_IO_PENDING)
	{
		CCLog::WriteLog(_T("AcceptEx EROOR "));
		return FALSE;
	}
	CCLog::WriteLog(_T("AcceptEx Success"));
	return TRUE;

}
//���� ��Ȱ��
VOID CSession::CloseSocket()
{
	TransmitFile
	(
		m_SessionSock,								//����� ���� �ڵ�
		NULL,										//������ ������ �ڵ�
		NULL,										//���Ϸ� ������ ũ��
		NULL,										//�ѹ� ������ �� ũ��
		(LPOVERLAPPED)&m_delete->m_Overlapped,		//OVERLAPPED ����ü
		NULL,										//�ι�° ���ڰ� NULL�� ��� ������ġ�������� ����
		TF_DISCONNECT | TF_REUSE_SOCKET				//���� ���� ���� ���۹��
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
	//AcceptEx�� ���� ���� �Ϸ��� ������ �ּҸ� �޾� ���� �Լ�
	GetAcceptExSockaddrs
	(
		m_Accept->m_Wsabuf.buf,						// AcceptEx 3��° ���ڿ� �����ؾ��ϴ� ���� ����
		0,											// ù ��° �����͸� �����ϴµ� ���Ǵ� ������ ũ��
		sizeof(SOCKADDR_IN) + 16,					// AcceptEx 5��° ���ڿ� �����ؾ��ϴ� ���� �ּ� ���� ������ ����� ����Ʈ ��
		sizeof(SOCKADDR_IN) + 16,					// AcceptEx 6��° ���ڿ� �����ؾ��ϴ� ���� �ּ� ���� ������ ����� ����Ʈ ��
		&SockAddress,								// ����� ���� �ּҸ� �޴� ����ü
		&SockAddressLen,							// ���� �ּ��� ũ��
		&RemoteAddress,								// ����� ���� �ּҸ� �޴� ����ü
		&RemoteAddressLen							// ���� �ּ��� ũ��
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
	// Nagle �˰��� ���� ���� ������ ������ ��� ���ָ� �ӵ� ����
	retval = setsockopt(m_SessionSock, IPPROTO_TCP, TCP_NODELAY, (char*)&optval, sizeof(optval));
	if (retval == SOCKET_ERROR)
	{
		CCLog::WriteLog(_T("TCP_NODELAY ERROR \n"));
		return;
	}

	Server.GetIocp().ConnectIocp_Session(this);
	SESSION.ActiveSession(this);
	//���� �Ϸ� Ȯ�� �Ŀ� Recv�� ���� ���� Ŭ���� ó�� �����͸� �޴´�.
	//CCLog::WriteLog(_T("GetAcceptINFO : %d"), m_SessionSock);

	Recv();
}
//��Ŷ Ȯ�� �Լ� ��Ŷ�� ũ�Ⱑ ũ�ų� NULL �̸� false
bool CSession::InVaildPacket(LPVOID pPacket)
{
	//pPacket NULL �� ��� ����
	if (pPacket == NULL) return false;
	//*packet �� ������� ���� �������� ��ġ�� �ִ� ��Ŷ�� �־� �ش�
	PacketHeader *packet = (PacketHeader*)pPacket;
	if (packet->size == 0) return false;
	//��Ŷ�� ũ�Ⱑ �ʱ⿡ �޾Ҵ� ��Ŷ ũ��� ��ġ�ϴ� ��� true��ȯ
	else if (packet->size <= receivedPaketSize) return true;
	return false;
}


VOID CSession::PreOnRecv(DWORD Databyte)
{
	CThreadSync Sync;
	receivedPaketSize = 0;
	//���� ������ ���� �� ������ ũ�⸸ŭ tail���������ش�.
	//if (m_ReadBuffer.Enqueue(m_Recv->m_Wsabuf.buf, Databyte))
	if (m_ReadBuffer.Enqueue(0, Databyte))
	{
		//���� ������ ũ�⸦ receivedPaketSize�� ����
		receivedPaketSize += Databyte;
	}
	while (1)
	{
		//���� ������ ũ�Ⱑ ��Ŷ�ش� ���� ���� ������ �ݺ��ؼ� �Ľ�
		if (receivedPaketSize < PACKETHEADER)
		{
			break;
		}
		else
		{
			//��Ŷ ũ�� Ȯ�� �Լ�
			if (InVaildPacket(m_ReadBuffer.GetRead()))
			{
				//���� head�� ��ġ�� ��Ŷ�ش��� ����
				PacketHeader *pPacket = (PacketHeader*)m_ReadBuffer.GetRead();
				//��Ŷ�Ľ�
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
	//while���� ���鼭 ���� �����͸� ��� �Ľ��ϰ� �������Ƿ�
	m_ReadBuffer.Clear();  //����� ���� ����.
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

	//n_Recv OverLappedINFO �ʱ�ȭ
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
		printf("%s : %d ���������� ����\n", inet_ntoa(m_SessionAddress.sin_addr), ntohs(m_SessionAddress.sin_port));
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


