#include "stdafx.h"
#include "dialog.h"
#include "resource.h"
#include "sendPacket.h"

HWND hCreateIdProc;
BOOL CALLBACK Dlg_CreateIdProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	hCreateIdProc = hDlg;
	static int characterNum;
	switch (iMsg)
	{
	case WM_INITDIALOG:
		characterNum = 0;
		return 1;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCHECK:
			SendCheckId_REQ(g_sock, hDlg);
			break;
		case IDC_SIGNUP_RADIO1:
			characterNum = 1;
			break;
		case IDC_SIGNUP_RADIO2:
			characterNum = 2;
			break;
		case IDC_SIGNUP_RADIO3:
			characterNum = 3;
			break;
		case IDC_SIGNUP_RADIO4:
			characterNum = 4;
			break;
		case IDC_SIGNUP_RADIO5:
			characterNum = 5;
			break;
		case IDOK:
			SendSignUp_REQ(g_sock, hDlg, characterNum);
			EndDialog(hDlg, 0);
			break;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			break;
		}
		break;
	}
	return 0;
}
