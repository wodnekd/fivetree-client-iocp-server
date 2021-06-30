#pragma once

class OverLappedINFO
{
public:
	OVERLAPPED	 m_Overlapped;
	WSABUF		 m_Wsabuf;
	DWORD		 m_Workflag;
	LPVOID		 m_pSession;

public:
	OverLappedINFO();
	~OverLappedINFO();
};

class CSession : public CMultiThreadSync <CSession>
{
private:
	DWORD				receivedPaketSize;
	char				buffer[BUFSIZE];
	SOCKET				m_SessionSock;
	SOCKADDR_IN			m_SessionAddress;
	SOCKADDR_IN			m_ServerAddress;
	CCircleQueue<char>	m_ReadBuffer;
	CCircleQueue<char>	m_WriteBuffer;
protected:
	OverLappedINFO		*m_Accept;
	OverLappedINFO		*m_Send;
	OverLappedINFO		*m_Recv;
	OverLappedINFO		*m_delete;
public:
	CSession();
	~CSession();
	SOCKET	GetSessionSock() const { return m_SessionSock; };
	BOOL	AcceptExSession();
	VOID	GetAcceptINFO();
	VOID	CloseSocket();

	virtual VOID PacketParsing(PacketHeader *pPacket) = 0;
	bool InVaildPacket(LPVOID pPacket);

	VOID PreOnRecv(DWORD Databyte);
	VOID PreOnSend(DWORD Databyte);
	VOID Recv();
	VOID Send(void *pPacket, DWORD size);

	void Clear();
	OverLappedINFO *GetAcceptOverlapped()	{ return m_Accept;	}
	OverLappedINFO *GetRecvOverlapped()		{ return m_Recv;	}
	OverLappedINFO *GetSendOverlapped()		{ return m_Send;	}
};