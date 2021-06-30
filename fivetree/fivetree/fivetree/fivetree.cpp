// fivetree.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "fivetree.h"
#include "sendPacket.h"
#pragma comment(lib, "msimg32.lib")
#define MAX_LOADSTRING 100

enum EditName {
	ID_LOGIN_SIGNUP_BUTTON = 200,
	ID_LOGIN_SEARCH_PW_BUTTON,
	ID_LOGIN_ID_EDIT,
	ID_LOGIN_PW_EDIT,
	ID_LOGIN_BUTTON,
	ID_WAIT_FIVETREE_GAME_START_BUTTON = 205,
	ID_WAIT_TETRIS_GAME_START_BUTTON,
	ID_WAIT_MY_ID_WINDOW_EDIT,
	ID_WAIT_MY_TOTAL_RESULT_BUTTON,
	ID_WAIT_USER_WINDOW_LISTBOX,
	ID_WAIT_CHAT_READ_WINDOW_LISTBOX,
	ID_WAIT_CHAT_WRITE_WINDOW_EDIT,
	ID_WAIT_CHAT_SEND_BUTTON,
	ID_FIVETREE_GAME_START_BUTTON = 213,
	ID_FIVETREE_GAME_OUT_BUTTON,
	ID_FIVETREE_GAME_CHAT_LISTBOX,
	ID_FIVETREE_GAME_CHAT_WRITE_EDIT,
	ID_FIVETREE_GAME_CHAT_SEND_BUTTON,
	ID_FIVETREE_GAME_MY_NAME_EDIT,
	ID_FIVETREE_GAME_OTHER_NAME_EDIT
};

// 전역 변수:
extern HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

HBITMAP hBitmap;

extern WCHAR myNickname[20];
extern WCHAR otherNickname[20];
extern int myCharacterNum;
extern int homeCharacterNum;
extern int awayCharacterNum;

bool buttonDown = false; //버튼이 눌렀는지 확인하는 변수
int x = 0; //현재 x좌표
int y = 0; //현재 y좌표
bool turn = true; //흰색(0), 검은색(1)돌 설정을 위해 필요한 변수

//HCURSOR cursor1, cursor2; //커서 이미지
//HBITMAP blackBitmap, whiteBitmap; //바둑돌 비트맵
bool curTurn; //현재 흰색(0), 검은색(1)돌인지 확인 변수

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void DrawBitMap(HDC hdc, int x, int y, HBITMAP hBit); //비트맵 그리기

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_FIVETREE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FIVETREE));
    MSG msg;
    // 기본 메시지 루프입니다.
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FIVETREE));
	wcex.hCursor		= LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = CreateSolidBrush(RGB(247, 240, 9)); //(HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = NULL;// MAKEINTRESOURCEW(IDC_FIVETREE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      100, 100, 495, 395, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
//loginWindow login;
//waitRoomWindow waitRoom;
//gameWindow game;
//Cserver server;
SOCKET g_sock;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
    switch (message)
    {
	case WM_CREATE:
		{
			loginWin.CreateLoginWindow(hWnd, hInst);
			server.serverInit(hWnd);
			g_sock = server.getConnectSock();
			waitRoomWin.CreateGameStartWindow(hWnd, hInst);
			waitRoomWin.CreateUserWindow(hWnd, hInst);
			waitRoomWin.CreateChatWindow(hWnd, hInst);
			waitRoomWin.ShowWaitRoomWindow(hWnd, false);
			gameWin.Init(hWnd, hInst);
			gameWin.CreateFivetreeWindow();
			gameWin.CreateChatWindow(hWnd, hInst);
			gameWin.ShowGameWindow(hWnd, false);
			SetWindowPos(hWnd, NULL, 100, 100, 495, 395, 0);
    	}
		break;
	case WM_SOCKET:
		server.ProcessSocketMessage(hWnd, message, wParam, lParam);
		break;
	case WM_LBUTTONDOWN:
		if(gameWin.GetPurStone() == true)
		{
			gameWin.SetStoneColor(gameWin.GetMyStoneColor());
			if (gameWin.Putstone(hWnd, lParam))
			{
				SendPutstone(g_sock);
			}
		}
		else
		{
			break;
		}
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다.
			switch (wmId)
			{
			case ID_LOGIN_SIGNUP_BUTTON:
			{
				DialogBox(hInst, MAKEINTRESOURCE(IDD_SIGNUP_DIALOG), hWnd, (DLGPROC)Dlg_CreateIdProc);
			}
			break;
			case ID_LOGIN_SEARCH_PW_BUTTON:
			{

			}
			break;
			case ID_LOGIN_BUTTON:
			{
				SendLoginUser_REQ(g_sock, loginWin.GethLoginIdEdit(), loginWin.GethLoginPwEdit());
			}
			break;
			case ID_WAIT_FIVETREE_GAME_START_BUTTON:
			{
				SendEnterFivetreeGame_REQ(g_sock);
			}
			break;
			case ID_WAIT_CHAT_SEND_BUTTON:
			{
				SendChat_REQ(g_sock, waitRoomWin.GethChatWriteWindowEdit());
				SetWindowText(waitRoomWin.GethChatWriteWindowEdit(), L"");
			}
			break;
			case ID_WAIT_MY_TOTAL_RESULT_BUTTON:
			{
				if (!IsWindow(waitRoomWin.GethMyTotalResultDlg()))
				{
					waitRoomWin.ShowMyTotalResult(hWnd, hInst);
					SendWinTotal_REQ(g_sock);
				}
			}
			break;
			case ID_FIVETREE_GAME_START_BUTTON:
			{
				if (MessageBox(GetFocus(), L"준비하시겠습니까?", L"게임 준비", MB_YESNO) == IDYES)
				{
					SendStartFivetreeGame_REQ(g_sock);
				}
			}
			break;
			case ID_FIVETREE_GAME_OUT_BUTTON:
			{
				if (MessageBox(GetFocus(), L"나가시겠습니까? 패배처리됩니다.", L"게임 나가기", MB_YESNO) == IDYES)
				{
					homeCharacterNum = 0;
					awayCharacterNum = 0;
					SetTimer(hWnd, 1, 500, NULL);
					gameWin.SetCursorNum(arrow);
					gameWin.ShowGameWindow(hWnd, false);
					waitRoomWin.ShowWaitRoomWindow(hWnd, true);
					SendOutFivertreeGame_REQ(g_sock);
				}
			}
			break;
			case ID_FIVETREE_GAME_CHAT_SEND_BUTTON:
			{
				SendRoomChat_REQ(g_sock, gameWin.GethChatWriteEdit());
				SetWindowText(gameWin.GethChatWriteEdit(), L"");
			}
			break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
				gameWin.DeleteBitmap();
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			InvalidateRect(hWnd, NULL, false);	//전체 맵한번 지워주기
			waitRoomWin.DrawUserCharacter(hWnd, myCharacterNum);
			break;
		}

		return 0;
	case WM_SETCURSOR: //커서의 모양을 설정
		gameWin.CursorSet();
		break;
    case WM_PAINT:
        {
		PAINTSTRUCT ps;
		HDC hdc;
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.
		loginWin.DrawLoginBackground(hdc);
		if (waitRoomWin.GetDrawWindow())
		{
			//waitRoomWin.DrawFivetreeBmp(hdc);
			waitRoomWin.DrawFivetreeBmp(hWnd);
		}
		if(gameWin.GetDrawWindow())
		{
			gameWin.DrawFivetreeLine(hdc);
			gameWin.DrawStone(hdc);
			gameWin.DrawUserCharacter(hWnd, homeCharacterNum, awayCharacterNum);
		}
	
		EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
	//	DeleteObject(blackBitmap);
	//	DeleteObject(whiteBitmap);
		SendOutNickname_REQ(g_sock, myNickname);
		SendExit_REQ(g_sock);
        PostQuitMessage(0);
        break;
    default:
       return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}



void DrawBitMap(HDC hdc, int x, int y, HBITMAP hBit)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	int bx, by;
	BITMAP bit;

	MemDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(MemDC, hBit);

	GetObject(hBit, sizeof(BITMAP), &bit);
	bx = bit.bmWidth;
	by = bit.bmHeight;

	TransparentBlt(hdc, x, y, bx, by, MemDC, 0, 0, bx, by, RGB(255, 0, 0));

	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}

