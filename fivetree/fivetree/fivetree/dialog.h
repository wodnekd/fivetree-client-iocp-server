#pragma once
#include <windows.h>
#include<winsock2.h>

extern SOCKET g_sock;

BOOL CALLBACK	Dlg_CreateIdProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);