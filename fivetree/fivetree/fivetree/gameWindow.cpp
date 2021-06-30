#include "stdafx.h"
#include "gameWindow.h"
#include "Resource.h"
#include "sendPacket.h"
#include <windows.h>

gameWindow::gameWindow()
{
	cursorNum = arrow;
	drawLine = false;
	stoneColor = false;
	drawCharacter = false;
	changeCursor = false;
	drawSton = false;
	memset(board, 3, sizeof(board));

}


gameWindow::~gameWindow()
{
	//SendOutFivertreeGame_REQ(g_sock);
}

void gameWindow::Init(HWND hWnd, HINSTANCE hInst)
{
	InvalidateRect(hWnd, NULL, false);	//전체 맵한번 지워주기
	drawLine = true;
	drawCharacter = true;
	changeCursor = true;
	
	SetWindowPos(hWnd, NULL, 100, 100, 800, 540, 0);
	
	blackCursor = LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR1));
	whiteCursor = LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR2));
	arrowCursor = LoadCursor(nullptr, IDC_ARROW);

	blackBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
	whiteBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP3));
	
	hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP5));
}


void gameWindow::CreateFivetreeWindow()
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			board[i][j] = 3;
		}
	}
}
void gameWindow::CreateChatWindow(HWND hWnd, HINSTANCE hInst)
{
	hStartButton = CreateWindowW(L"button", L"READT / START", WS_CHILD | WS_VISIBLE | WS_BORDER,
		500, 230, 140, 50, hWnd, (HMENU)ID_FIVETREE_GAME_START_BUTTON, hInst, NULL);
	hOutButton = CreateWindowW(L"button", L"OUT (패배처리)", WS_CHILD | WS_VISIBLE | WS_BORDER,
		640, 230, 140, 50, hWnd, (HMENU)ID_FIVETREE_GAME_OUT_BUTTON, hInst, NULL);

	hChatListbox = CreateWindowW(L"listbox", NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY,
		500, 280, 280, 170, hWnd, (HMENU)ID_FIVETREE_GAME_CHAT_LISTBOX, hInst, NULL);
	hChatWriteEdit = CreateWindowW(L"edit", NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		500, 450, 230, 25, hWnd, (HMENU)ID_FIVETREE_GAME_CHAT_WRITE_EDIT, hInst, NULL);
	hChatSendButton = CreateWindowW(L"button", L"SEND", WS_CHILD | WS_VISIBLE | WS_BORDER,
		730, 450, 50, 25, hWnd, (HMENU)ID_FIVETREE_GAME_CHAT_SEND_BUTTON, hInst, NULL);

	hHomeNameEdit = CreateWindowW(L"edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_CENTER,
		520, 200, 100, 25, hWnd, (HMENU)ID_FIVETREE_GAME_MY_NAME_EDIT, hInst, NULL);
	hAwayNameEdit = CreateWindowW(L"edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | ES_CENTER,
		660, 200, 100, 25, hWnd, (HMENU)ID_FIVETREE_GAME_OTHER_NAME_EDIT, hInst, NULL);
}

void gameWindow::DrawFivetreeLine(HDC hdc)
{
	if (drawLine == true)
	{
		for (int i = 1; i < 20; i = i + 2)
		{
			MoveToEx(hdc, 25, 25 * i, NULL);
			LineTo(hdc, 475, 25 * i);
			MoveToEx(hdc, 25 * i, 25, NULL);
			LineTo(hdc, 25 * i, 475);
		}
		//한번만 그리기
		drawLine = false;
	}
}

void gameWindow::CursorSet()
{
	if (cursorNum == arrow)
	{
		SetCursor(arrowCursor);
	}
	else if (cursorNum == black)
	{
		SetCursor(blackCursor);
	}
	else if (cursorNum == white)
	{
		SetCursor(whiteCursor);
	}

}
//1
bool gameWindow::Putstone(HWND hWnd, LPARAM lParam)
{
	x = LOWORD(lParam);
	y = HIWORD(lParam);

	if (board[x / 50][y / 50] == 3) //놓으려는 장소에 바둑알에 없을 때만 바둑알을 화면에 그림
	{
		putStone = false;
		drawSton = true;
		clickButton = true;
		if (stoneColor == white)
		{
			board[x / 50][y / 50] = white;
			InvalidateRect(hWnd, NULL, false);
			return true;
		}
		else if (stoneColor == black)
		{
			board[x / 50][y / 50] = black;
			InvalidateRect(hWnd, NULL, false);
			return true;
		}
	}
	return false;

}
//
//void gameWindow::Putstone(HWND hWnd, LPARAM lParam, bool pullStone)
//{
//	x = LOWORD(lParam);
//	y = HIWORD(lParam);
//	if (pullStone == true)
//	{
//		drawSton = true;
//
//	
//		if (board[x / 50][y / 50] == 3) //놓으려는 장소에 바둑알에 없을 때만 바둑알을 화면에 그림
//		{
//			clickButton = true;
//			if (stoneColor == true)
//			{
//				board[x / 50][y / 50] = 0;
//				InvalidateRect(hWnd, NULL, false);
//			}
//			else if (stoneColor == false)
//			{
//				board[x / 50][y / 50] = 1;
//				InvalidateRect(hWnd, NULL, false);
//			}
//		}
//	}
//}
void gameWindow::DrawStone(HDC hdc)
{
	if (drawSton == true)
	{
		x = (x / 50) * 50;
		y = (y / 50) * 50;
		if (stoneColor == black )
		{
			HDC MemDC;
			HBITMAP OldBitmap;
			int bx, by;
			BITMAP bit;

			MemDC = CreateCompatibleDC(hdc);
			OldBitmap = (HBITMAP)SelectObject(MemDC, blackBitmap);

			GetObject(blackBitmap, sizeof(BITMAP), &bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;

			TransparentBlt(hdc, x, y, bx, by, MemDC, 0, 0, bx, by, RGB(255, 0, 0));

			SelectObject(MemDC, OldBitmap);
			DeleteDC(MemDC);
		}
		else if (stoneColor == white )
		{
			HDC MemDC;
			HBITMAP OldBitmap;
			int bx, by;
			BITMAP bit;

			MemDC = CreateCompatibleDC(hdc);
			OldBitmap = (HBITMAP)SelectObject(MemDC, whiteBitmap);

			GetObject(whiteBitmap, sizeof(BITMAP), &bit);
			bx = bit.bmWidth;
			by = bit.bmHeight;

			TransparentBlt(hdc, x, y, bx, by, MemDC, 0, 0, bx, by, RGB(255, 0, 0));

			SelectObject(MemDC, OldBitmap);
			DeleteDC(MemDC);
		}
	}
	drawSton = false;
	
}

void gameWindow::DrawUserCharacter(HWND hWnd, int firstPlayer, int SecondPlayer)
{
	
	if (drawCharacter == true)
	{
		InvalidateRect(hWnd, NULL, false);	//전체 맵한번 지워주기
		HDC hdc;
		PAINTSTRUCT ps;
		HDC MemDC;
		int bx, by;
		BITMAP bit;
		HBITMAP OldBitmap;

		hdc = BeginPaint(hWnd, &ps);
		MemDC = CreateCompatibleDC(hdc);	//Bitmap은 MemDC에 그려주고 옮겨주는 방법을 씀
		OldBitmap = (HBITMAP)SelectObject(MemDC, hBitmap);	//비트맵 GDI 세팅

															//GDI오브젝트의 정보를 구함0
		GetObject(hBitmap, sizeof(BITMAP), &bit);
		bx = bit.bmWidth;
		by = bit.bmHeight;

		// 소스와 같은 비율로 출력
		switch (firstPlayer)
		{
		case 0:
			//StretchBlt(hdc, 500, 250, 140, 170, MemDC, 0, 0, 10, 10, WHITENESS);
			break;
		case 1:
			StretchBlt(hdc, 500, 25, 140, 170, MemDC, 0, 0, 145, 146, SRCCOPY);
			break;
		case 2:
			StretchBlt(hdc, 500, 25, 140, 170, MemDC, 0, 145, 145, 162, SRCCOPY);
			break;
		case 3:
			StretchBlt(hdc, 500, 25, 140, 170, MemDC, 0, 305, 145, 194, SRCCOPY);
			break;
		case 4:
			StretchBlt(hdc, 500, 25, 140, 170, MemDC, 0, 499, 145, 162, SRCCOPY);
			break;
		case 5:
			StretchBlt(hdc, 500, 25, 140, 175, MemDC, 0, 660, 146, 200, SRCCOPY);
			break;
		default:
			break;
		}
		switch (SecondPlayer)
		{
		case 0:
			//StretchBlt(hdc, 500, 250, 140, 170, MemDC, 0, 0, 10, 10, WHITENESS);
			break;
		case 1:
			StretchBlt(hdc, 640, 25, 140, 170, MemDC, 0, 0, 145, 146, SRCCOPY);
			break;
		case 2:
			StretchBlt(hdc, 640, 25, 140, 170, MemDC, 0, 145, 145, 162, SRCCOPY);
			break;
		case 3:
			StretchBlt(hdc, 640, 25, 140, 170, MemDC, 0, 306, 145, 194, SRCCOPY);
			break;
		case 4:
			StretchBlt(hdc, 640, 25, 140, 170, MemDC, 0, 499, 145, 162, SRCCOPY);
			break;
		case 5:
			StretchBlt(hdc, 640, 25, 140, 175, MemDC, 0, 660, 146, 200, SRCCOPY);
			break;
		default:
			break;
		}
		SelectObject(MemDC, OldBitmap);
		DeleteDC(MemDC);
		EndPaint(hWnd, &ps);
	}
}

void gameWindow::DeleteBitmap()
{
	DeleteObject(blackBitmap);
	DeleteObject(whiteBitmap);
}

void gameWindow::ShowGameWindow(HWND hWnd, bool TF)
{
	if(TF == true)
	{
		SetWindowPos(hWnd, NULL, 100, 100, 800, 540, SWP_NOMOVE);
		drawWindow = true;
	}
	else
	{
		SetWindowPos(hWnd, NULL, 100, 100, 650, 540, SWP_NOMOVE);
		SetWindowText(hHomeNameEdit, L"");
		SetWindowText(hAwayNameEdit, L"");
		SetWindowText(hChatWriteEdit, L"");
		SendMessage(hChatListbox, LB_RESETCONTENT, 0, 0);
		drawWindow = false;
	}
	drawLine = TF;
	//drawCharacter = TF;
	//changeCursor = TF;
	//drawSton = TF;
	ShowWindow(hStartButton, TF);
	ShowWindow(hOutButton, TF);
	ShowWindow(hChatListbox, TF);
	ShowWindow(hChatWriteEdit, TF);
	ShowWindow(hChatSendButton, TF);
	ShowWindow(hHomeNameEdit, TF);
	ShowWindow(hAwayNameEdit, TF);
}

void gameWindow::DeletGameWindow()
{
	drawLine = false;
	stoneColor = false;
	drawCharacter = false;
	changeCursor = false;
	drawSton = false;
	memset(board, 3, sizeof(board));
	DestroyWindow(hStartButton);
	DestroyWindow(hOutButton);
	DestroyWindow(hChatListbox);
	DestroyWindow(hChatWriteEdit);
	DestroyWindow(hChatSendButton);
	DestroyWindow(hHomeNameEdit);
	DestroyWindow(hAwayNameEdit);
}
