#pragma once

#pragma warning(disable: 4996)
//에러 #define
#define _WINSOCK_DEPRECATED_NO_WARNINGS
//서버 define
#define SERVERPORT			9000
#define BUFSIZE				4096
#define  MAX_BUFFER_LENGTH  5200

#define CHOP(x)				 x[strlen(x) - 1] = ' '

//표준 헤더
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include<iostream>
using namespace std;
//STL 헤더
#include <map>
#include <vector>
#include <list>
//윈속 사용에 필요한 헤더
#pragma comment(lib, "ws2_32")
#include<winsock2.h>
#include<process.h>

//#include <my_global.h>
//acceptex 사용에 필요한 헤더
#include <mswsock.h>
#pragma comment(lib,"mswsock.lib")
//사용자 설정 헤더
#include "CLog.h"
#include "CriticalSection.h"
#include "protocol.h"
#include "CCircleQueue.h"
#include "CSession.h"
#include "CUserSession.h"
#include "CIocp.h"
#include "CServer.h"
#include "CThread.h"
#include "CDatabase.h"
#include "CSingleton.h"
#include "CRoom.h"
#include "CUserManager.h"
#include "CRoomManager.h"
#include "CSessionManager.h"
#include "CLogin.h"
#include "CResult.h"
//
enum IO_TYPE
{
	IO_ACCEPT,
	IO_READ,
	IO_SEND
};

#define PACKETHEADER     4
#define MAX_ROOMCOUNT    10
//singleton
#define SESSION        SINGLETON(CSessionManager)
#define Server		   SINGLETON(CServer)
#define Database       SINGLETON(CDatabase)
#define Login		   SINGLETON(CLogin)
#define Iocp		   SINGLETON(CIocp)
#define Thread		   SINGLETON(CThread)
#define Result		   SINGLETON(CResult)