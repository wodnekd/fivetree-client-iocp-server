#pragma once

class CServer : public CMultiThreadSync <CServer>
{
private:
	WSADATA			 m_Wsadata;
	SOCKET			 m_LisenSock;
	SOCKADDR_IN		 m_ServerAddress;	
	int				 addrlen;
	//
	SOCKET			 m_ClientSock;
	SOCKADDR_IN		 m_ClientAddress;
	//
	CIocp			 m_Iocp;
public:
	CServer();
	~CServer();
	BOOL			 InitServer();
	BOOL			 LisenBindServer();
	SOCKET			 GetLisenSock()		{return m_LisenSock;}
	CIocp			 GetIocp()			{return m_Iocp;}
};
