// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
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




//// ���� messagebox�� ���
//TCHAR text[100];
//wsprintf(text, L"drawfiveLine: %d", drawLine);
//MessageBox(GetFocus(), text, L"1", MB_OK);

//����Ʈ �׸� ��ü ����
//SendMessage(hChatListbox, LB_RESETCONTENT, 0, 0);

//������ ���� ����Ʈ �� �����
//SetWindowText(waitRoomWin.GethChatWriteWindowEdit(), L"");

//���̾�α׶� ������ ��� ����ϴµ� �Ѵ� ����� �Ǳ�������
//���� ������
//SendMessage(hChatListbox, LB_RESETCONTENT, 0, 0);
//SendDlgItemMessage(hDlg_ChatMessageProc, IDC_CHATTING_ROOM_NAME_EDIT, EM_REPLACESEL, FALSE, (LPARAM)myChatRoomName);


//Set �� Send �� ����
//Set�� ���� �Է��� ��
//Send�� ����Ʈ �ڽ� ���� �Ϳ� ����� �� // �޼����� ����
//SetDlgItemText(hDlg_ChatMessageProc, IDC_CHAT_MY_ID_EDIT, myId);
//SendDlgItemMessage(hDlg_ChatMessageProc, IDC_CHATTING_ROOM_NAME_EDIT, EM_REPLACESEL, FALSE, (LPARAM)myChatRoomName);