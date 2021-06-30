#pragma once
#pragma comment(lib, "ws2_32")
#include <winsock2.h>

#define SERVERIP	"127.0.0.1"
#define SERVERPORT	9000
#define BUFSIZE		512
#define WM_SOCKET (WM_USER+1)

extern HWND hCreateIdProc;

class Cserver
{
private:
	SOCKET connect_sock;
public:
	Cserver();
	~Cserver();
	void serverInit(HWND hWnd);
	SOCKET getConnectSock();
	void ProcessSocketMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

