#pragma once

class CIocp
{
private:
	HANDLE m_IocpHandle;

public:
	CIocp();
	~CIocp();

	BOOL	CreateIocp();
	HANDLE	GetHandle() {return m_IocpHandle;}
	VOID	ConnectIocp_ListenSocket(SOCKET ListenSocket);
	VOID	ConnectIocp_Session(CSession* pSession);
};
