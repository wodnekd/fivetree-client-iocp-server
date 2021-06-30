#include "stdafx.h"
#include "tetrisWindow.h"


tetrisWindow::tetrisWindow()
{
}


tetrisWindow::~tetrisWindow()
{
}

void tetrisWindow::Init(HWND hWnd)
{
	int x = 100;
	int y = 100;
	HDC hdc = GetDC(hWnd); //DC을얻는 방법 1
	SetTextColor(hdc, RGB(255, 0, 0));
	SetBkColor(hdc, RGB(0, 0, 0));
	//B
	TextOut(hdc, 5+x, 2+y, TEXT("■■"), 2);
	TextOut(hdc, 5+x, 17+y, TEXT("■  ■"), 4);
	TextOut(hdc, 5+x, 32+y, TEXT("■■"), 2);
	TextOut(hdc, 5+x, 47+y, TEXT("■  ■"), 4);
	TextOut(hdc, 5+x, 62+y, TEXT("■■"), 2);
	//A
	TextOut(hdc, 63 + x, 2 + y,  TEXT("  ■  "), 5);
	TextOut(hdc, 55 + x, 17 + y, TEXT(" ■  ■ "), 5);
	TextOut(hdc, 50 + x, 32 + y, TEXT(" ■■■ "), 5);
	TextOut(hdc, 53 + x, 47 + y, TEXT("■    ■ "), 7);
	TextOut(hdc, 53 + x, 62 + y, TEXT("■    ■ "), 7);
	//T
	TextOut(hdc, 110 + x, 2 + y, TEXT(" ■■■■■  "), 5);
	TextOut(hdc, 130 + x, 17 + y, TEXT("  ■  "), 5);
	TextOut(hdc, 130 + x, 32 + y, TEXT("  ■  "), 5);
	TextOut(hdc, 130 + x, 47 + y, TEXT("  ■  "), 5);
	TextOut(hdc, 130 + x, 62 + y, TEXT("  ■  "), 5);
	//T
	TextOut(hdc, 180 + x, 2 + y, TEXT(" ■■■■■  "), 5);
	TextOut(hdc, 200 + x, 17 + y, TEXT("  ■  "), 5);
	TextOut(hdc, 200 + x, 32 + y, TEXT("  ■  "), 5);
	TextOut(hdc, 200 + x, 47 + y, TEXT("  ■  "), 5);
	TextOut(hdc, 200 + x, 62 + y, TEXT("  ■  "), 5);
	//l
	TextOut(hdc, 260 + x, 2 + y,  TEXT("  ■  "), 5);
	TextOut(hdc, 260 + x, 17 + y, TEXT("  ■  "), 5);
	TextOut(hdc, 260 + x, 32 + y, TEXT("  ■  "), 5);
	TextOut(hdc, 260 + x, 47 + y, TEXT("  ■  "), 5);
	TextOut(hdc, 260 + x, 62 + y, TEXT("  ■  "), 5);
	//E
	TextOut(hdc, 300 + x, 2 + y,  TEXT(" ■■■ "), 5);
	TextOut(hdc, 300 + x, 17 + y, TEXT(" ■   "), 5);
	TextOut(hdc, 300 + x, 32 + y, TEXT(" ■■■ "), 5);
	TextOut(hdc, 300 + x, 47 + y, TEXT(" ■   "), 5);
	TextOut(hdc, 300 + x, 62 + y, TEXT(" ■■■ "), 5);
	SetTextColor(hdc, RGB(255, 0, 255));
	SetBkColor(hdc, RGB(0, 0, 0));
	//T
	TextOut(hdc, 20 + x, 100 + y, TEXT(" ▣▣▣▣▣ "), 5);
	TextOut(hdc, 40 + x, 117 + y, TEXT("  ▣  "), 5);
	TextOut(hdc, 40 + x, 132 + y, TEXT("  ▣  "), 5);
	TextOut(hdc, 40 + x, 147 + y, TEXT("  ▣  "), 5);
	TextOut(hdc, 40 + x, 162 + y, TEXT("  ▣  "), 5);
	//E
	TextOut(hdc, 100 + x, 100 + y, TEXT(" ▣▣▣ "), 5);
	TextOut(hdc, 100 + x, 117 + y, TEXT(" ▣   "), 5);
	TextOut(hdc, 100 + x, 132 + y, TEXT(" ▣▣▣ "), 5);
	TextOut(hdc, 100 + x, 147 + y, TEXT(" ▣   "), 5);
	TextOut(hdc, 100 + x, 162 + y, TEXT(" ▣▣▣ "), 5);
	//T
	TextOut(hdc, 160 + x, 100 + y, TEXT(" ▣▣▣▣▣ "), 5);
	TextOut(hdc, 180 + x, 117 + y, TEXT("  ▣  "), 5);
	TextOut(hdc, 180 + x, 132 + y, TEXT("  ▣  "), 5);
	TextOut(hdc, 180 + x, 147 + y, TEXT("  ▣  "), 5);
	TextOut(hdc, 180 + x, 162 + y, TEXT("  ▣  "), 5);
	//R
	TextOut(hdc, 240 + x, 100 + y, TEXT(" ▣▣▣ "), 5);
	TextOut(hdc, 240 + x, 117 + y, TEXT(" ▣    ▣ "), 8);
	TextOut(hdc, 240 + x, 132 + y, TEXT(" ▣▣  "), 5);
	TextOut(hdc, 240 + x, 147 + y, TEXT(" ▣   ▣ "), 7);
	TextOut(hdc, 240 + x, 162 + y, TEXT(" ▣     ▣ "), 9);
	//I
	TextOut(hdc, 310 + x, 100 + y, TEXT(" ▣ "), 3);
	TextOut(hdc, 310 + x, 117 + y, TEXT(" ▣ "), 3);
	TextOut(hdc, 310 + x, 132 + y, TEXT(" ▣ "), 3);
	TextOut(hdc, 310 + x, 147 + y, TEXT(" ▣ "), 3);
	TextOut(hdc, 310 + x, 162 + y, TEXT(" ▣ "), 3);
	//S
	TextOut(hdc, 340 + x, 100 + y, TEXT(" ▣▣▣ "), 5);
	TextOut(hdc, 340 + x, 117 + y, TEXT(" ▣ "), 3);
	TextOut(hdc, 340 + x, 132 + y, TEXT(" ▣▣▣ "), 5);
	TextOut(hdc, 360 + x, 147 + y, TEXT("    ▣ "), 6);
	TextOut(hdc, 340 + x, 162 + y, TEXT(" ▣▣▣ "), 5);
	ReleaseDC(hWnd, hdc);
}
