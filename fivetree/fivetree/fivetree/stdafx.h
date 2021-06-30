// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include "loginWindow.h"
#include "gameWindow.h"
#include "waitRoomWindow.h"
#include "server.h"
#include "dialog.h"
#include "singleton.h"



#define loginWin		SINGLETON(loginWindow)
#define waitRoomWin		SINGLETON(waitRoomWindow)
#define gameWin			SINGLETON(gameWindow)
#define server			SINGLETON(Cserver)




//// 변수 messagebox로 출력
//TCHAR text[100];
//wsprintf(text, L"drawfiveLine: %d", drawLine);
//MessageBox(GetFocus(), text, L"1", MB_OK);

//리스트 항목 전체 삭제
//SendMessage(hChatListbox, LB_RESETCONTENT, 0, 0);

//윈도우 내부 에디트 값 변경시
//SetWindowText(waitRoomWin.GethChatWriteWindowEdit(), L"");

//다이얼로그랑 윈도우 경우 사용하는데 둘다 사용은 되긴하지만
//구분 지어사용
//SendMessage(hChatListbox, LB_RESETCONTENT, 0, 0);
//SendDlgItemMessage(hDlg_ChatMessageProc, IDC_CHATTING_ROOM_NAME_EDIT, EM_REPLACESEL, FALSE, (LPARAM)myChatRoomName);


//Set 과 Send 의 차이
//Set은 문자 입력할 떄
//Send는 리스트 박스 같은 것에 사용할 떄 // 메세지를 보냄
//SetDlgItemText(hDlg_ChatMessageProc, IDC_CHAT_MY_ID_EDIT, myId);
//SendDlgItemMessage(hDlg_ChatMessageProc, IDC_CHATTING_ROOM_NAME_EDIT, EM_REPLACESEL, FALSE, (LPARAM)myChatRoomName);