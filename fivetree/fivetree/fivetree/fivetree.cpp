// fivetree.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
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

// ���� ����:
extern HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

HBITMAP hBitmap;

extern WCHAR myNickname[20];
extern WCHAR otherNickname[20];
extern int myCharacterNum;
extern int homeCharacterNum;
extern int awayCharacterNum;

bool buttonDown = false; //��ư�� �������� Ȯ���ϴ� ����
int x = 0; //���� x��ǥ
int y = 0; //���� y��ǥ
bool turn = true; //���(0), ������(1)�� ������ ���� �ʿ��� ����

//HCURSOR cursor1, cursor2; //Ŀ�� �̹���
//HBITMAP blackBitmap, whiteBitmap; //�ٵϵ� ��Ʈ��
bool curTurn; //���� ���(0), ������(1)������ Ȯ�� ����

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void DrawBitMap(HDC hdc, int x, int y, HBITMAP hBit); //��Ʈ�� �׸���

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ���⿡ �ڵ带 �Է��մϴ�.

    // ���� ���ڿ��� �ʱ�ȭ�մϴ�.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_FIVETREE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FIVETREE));
    MSG msg;
    // �⺻ �޽��� �����Դϴ�.
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
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
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
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

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
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND  - ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT    - �� â�� �׸��ϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
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
            // �޴� ������ ���� �м��մϴ�.
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
				if (MessageBox(GetFocus(), L"�غ��Ͻðڽ��ϱ�?", L"���� �غ�", MB_YESNO) == IDYES)
				{
					SendStartFivetreeGame_REQ(g_sock);
				}
			}
			break;
			case ID_FIVETREE_GAME_OUT_BUTTON:
			{
				if (MessageBox(GetFocus(), L"�����ðڽ��ϱ�? �й�ó���˴ϴ�.", L"���� ������", MB_YESNO) == IDYES)
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
			InvalidateRect(hWnd, NULL, false);	//��ü ���ѹ� �����ֱ�
			waitRoomWin.DrawUserCharacter(hWnd, myCharacterNum);
			break;
		}

		return 0;
	case WM_SETCURSOR: //Ŀ���� ����� ����
		gameWin.CursorSet();
		break;
    case WM_PAINT:
        {
		PAINTSTRUCT ps;
		HDC hdc;
		hdc = BeginPaint(hWnd, &ps);
		// TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�.
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

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
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

