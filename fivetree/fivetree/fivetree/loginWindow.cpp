#include "stdafx.h"
#include "loginWindow.h"
#include "resource.h"

loginWindow::loginWindow()
{
	
}


loginWindow::~loginWindow()
{
}

void loginWindow::DrawLoginBackground(HDC hdc)
{
	HDC memdc;
	memdc = CreateCompatibleDC(hdc);
	SelectObject(memdc, this->hBitmap);
	BitBlt(hdc, 0, 0, 480, 480, memdc, 0, 0, SRCCOPY);
	DeleteDC(memdc);
	DeleteObject(this->hBitmap);
}

void loginWindow::CreateLoginWindow(HWND hWnd, HINSTANCE hInst)
{
	hBitmap = (HBITMAP)LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));

	hLoginSignUpButton = CreateWindowW(L"button", L"회원 가입", WS_CHILD | WS_VISIBLE | WS_BORDER,
		180, 310, 100, 22, hWnd, (HMENU)ID_LOGIN_SIGNUP_BUTTON, hInst, NULL);

	hLoginIdEdit = CreateWindowW(L"edit", NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		180, 260, 100, 22, hWnd, (HMENU)ID_LOGIN_ID_EDIT, hInst, NULL);

	hLoginPwEdit = CreateWindowW(L"edit", NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		180, 285, 100, 22, hWnd, (HMENU)ID_LOGIN_PW_EDIT, hInst, NULL);

	hLoginButton = CreateWindowW(L"button", L"로그인", WS_CHILD | WS_VISIBLE | WS_BORDER,
		290, 260, 50, 48, hWnd, (HMENU)ID_LOGIN_BUTTON, hInst, NULL);
}


void loginWindow::DeleteLoginWindow()
{
	DestroyWindow(hLoginSignUpButton);
	DestroyWindow(hLoginIdEdit);
	DestroyWindow(hLoginPwEdit);
	DestroyWindow(hLoginButton);
}

