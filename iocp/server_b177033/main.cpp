#include "stdafx.h"


int main()
{
	Database.ConnectSql();
	Server.InitServer();
	Server.LisenBindServer();
	SESSION.Init();
	SESSION.CreateSeccsion(30);
	CThread* thread = new CThread;
	thread->CreateThread();
	getchar();
	thread->DestroyThread();
	// 윈속 종료
	WSACleanup();
    return 0;
}

