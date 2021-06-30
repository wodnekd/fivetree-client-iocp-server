#include "stdafx.h"
#include "waitRoomWindow.h"
#include "sendPacket.h"
#include "resource.h"

WCHAR myNickname[20];

waitRoomWindow::waitRoomWindow()
{
	drawWindow = false;
}


waitRoomWindow::~waitRoomWindow()
{
}

void waitRoomWindow::CreateGameStartWindow(HWND hWnd, HINSTANCE hInst)
{
	SetWindowPos(hWnd, NULL, 100, 100, 650, 540, 0);
	hFivetreeGameStartButton = CreateWindowW(L"button", L"오목 게임 찾기", WS_CHILD | WS_VISIBLE | WS_BORDER,
		210, 10, 200, 200, hWnd, (HMENU)ID_WAIT_FIVETREE_GAME_START_BUTTON, hInst, NULL);
//	hTetrisGameStartButton = CreateWindowW(L"button", L"테트리스 게임 찾기", WS_CHILD | WS_VISIBLE | WS_BORDER,
//		210, 10, 200, 200, hWnd, (HMENU)ID_WAIT_TETRIS_GAME_START_BUTTON, hInst, NULL);
}

void waitRoomWindow::CreateUserWindow(HWND hWnd, HINSTANCE hInst)
{
	hBitmap = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP5));
	hBitmap2 = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP6));
	hMyIdWindowEdit = CreateWindowW(L"edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY,
		420, 185, 100, 25, hWnd, (HMENU)ID_WAIT_MY_ID_WINDOW_EDIT, hInst, NULL);
	hMyTotalResultButton = CreateWindowW(L"button", L"전적 보기", WS_CHILD | WS_VISIBLE | WS_BORDER,
		520, 185, 100, 25, hWnd, (HMENU)ID_WAIT_MY_TOTAL_RESULT_BUTTON, hInst, NULL);
	hUserWindowListbox = CreateWindowW(L"listbox", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
		420, 220, 200, 275, hWnd, (HMENU)ID_WAIT_USER_WINDOW_LISTBOX, hInst, NULL);

}

void waitRoomWindow::CreateChatWindow(HWND hWnd, HINSTANCE hInst)
{
	hChatReadWindowListbox = CreateWindowW(L"listbox", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY,
		10, 220, 400, 245, hWnd, (HMENU)ID_WAIT_CHAT_READ_WINDOW_LISTBOX, hInst, NULL);
	hChatWriteWindowEdit = CreateWindowW(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
		10, 470, 320, 25, hWnd, (HMENU)ID_WAIT_CHAT_WRITE_WINDOW_EDIT, hInst, NULL);
	hChatSendButton = CreateWindowW(L"button", L"보내기", WS_CHILD | WS_VISIBLE | WS_BORDER,
		330, 470, 80, 25, hWnd, (HMENU)ID_WAIT_CHAT_SEND_BUTTON, hInst, NULL);
}


void waitRoomWindow::DrawFivetreeBmp(HWND hWnd)
{
	InvalidateRect(hWnd, NULL, false);
	HDC hdc;
	PAINTSTRUCT ps;
	HBITMAP OldBitmap;
	HDC MemDC;

	hdc = BeginPaint(hWnd, &ps);
	MemDC = CreateCompatibleDC(hdc);	//Bitmap은 MemDC에 그려주고 옮겨주는 방법을 씀
	OldBitmap = (HBITMAP)SelectObject(MemDC, hBitmap2);	//비트맵 GDI 세팅

														//출력위치x,y // 잘라 내는 시작위치x,y // 잘라내는 크기 x,y
	StretchBlt(hdc, 10, 10, 200, 200, MemDC, 0, 0, 403, 263, SRCCOPY);


	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
	EndPaint(hWnd, &ps);

}

void waitRoomWindow::DrawUserCharacter(HWND hWnd, int characterNum)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HBITMAP OldBitmap;
	HDC MemDC;

	static int BitIndexX = 0;
	static int BitWidth = 145;

	hdc = BeginPaint(hWnd, &ps);
	MemDC = CreateCompatibleDC(hdc);	//Bitmap은 MemDC에 그려주고 옮겨주는 방법을 씀
	OldBitmap = (HBITMAP)SelectObject(MemDC, hBitmap);	//비트맵 GDI 세팅

	//출력위치x,y // 잘라 내는 시작위치x,y // 잘라내는 크기 x,y
	switch (characterNum)
	{
	case 0:
		break;
	case 1:
		StretchBlt(hdc, 420, 10, 200, 180, MemDC, BitWidth*BitIndexX, 0, BitWidth, 146, SRCCOPY);
		break;
	case 2:
		StretchBlt(hdc, 420, 10, 200, 180, MemDC, BitWidth*BitIndexX, 145, BitWidth, 162, SRCCOPY);
		break;
	case 3:
		StretchBlt(hdc, 420, 10, 200, 180, MemDC, BitWidth*BitIndexX, 305, BitWidth, 194, SRCCOPY);
		break;
	case 4:
		StretchBlt(hdc, 420, 10, 200, 180, MemDC, BitWidth*BitIndexX, 500, BitWidth, 162, SRCCOPY);
		break;
	case 5:
		StretchBlt(hdc, 420, 10, 200, 180, MemDC, BitWidth*BitIndexX, 660, BitWidth, 200, SRCCOPY);
		break;
	default:
		break;
	}
	if (++BitIndexX == 3)
	{
		BitIndexX = 0;
	}


	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
	EndPaint(hWnd, &ps);
}

void waitRoomWindow::ShowMyTotalResult(HWND hWnd, HINSTANCE hInst)
{
	hMyTotalResultDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_RESULT_DIALOG), hWnd, (DLGPROC)Dlg_CreateMyTotalResultProc);
	ShowWindow(hMyTotalResultDlg, SW_SHOW);
	
}

void waitRoomWindow::ShowWaitRoomWindow(HWND hWnd, bool TF)
{
	if (TF == true)
	{
		SetWindowPos(hWnd, NULL, 100, 100, 650, 540, SWP_NOMOVE);
		drawWindow = true;
	}
	else
	{
		SetWindowPos(hWnd, NULL, 100, 100, 800, 540, SWP_NOMOVE);
		drawWindow = false;
	}
	ShowWindow(hFivetreeGameStartButton, TF);
//	ShowWindow(hTetrisGameStartButton, TF);
	ShowWindow(hMyIdWindowEdit, TF);
	ShowWindow(hMyTotalResultButton, TF);
	ShowWindow(hUserWindowListbox, TF);
	ShowWindow(hUserWindowListbox, TF);
	ShowWindow(hChatReadWindowListbox, TF);
	ShowWindow(hChatWriteWindowEdit, TF);
	ShowWindow(hChatSendButton, TF);
}

void waitRoomWindow::DeleteWaitRoomWindow()
{
	DestroyWindow(hFivetreeGameStartButton);
//	DestroyWindow(hTetrisGameStartButton);
	DestroyWindow(hMyIdWindowEdit);
	DestroyWindow(hMyTotalResultButton);
	DestroyWindow(hUserWindowListbox);
	DestroyWindow(hChatReadWindowListbox);
	DestroyWindow(hChatWriteWindowEdit);
	DestroyWindow(hChatSendButton);
}

BOOL CALLBACK Dlg_CreateMyTotalResultProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_INITDIALOG:
		return 1;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			DestroyWindow(hDlg);
			EndDialog(hDlg, 0);
			break;
		case IDCANCEL:
			DestroyWindow(hDlg);
			EndDialog(hDlg, 0);
			break;
		}
		break;
	}
	return 0;
}
