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
	// ���� ����
	WSACleanup();
    return 0;
}

