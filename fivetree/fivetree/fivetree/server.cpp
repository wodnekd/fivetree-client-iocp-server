#include "stdafx.h"
#include "server.h"
#include "protocal.h"
#include "resource.h"
#include "sendPacket.h"
HINSTANCE hInst;

extern WCHAR myNickname[20];
int myCharacterNum;
int homeCharacterNum;
int awayCharacterNum;

Cserver::Cserver()
{
}


Cserver::~Cserver()
{
}

void Cserver::serverInit(HWND hWnd)
{
	//윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return;
	//socket 생성
	SOCKET clientSock = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSock == INVALID_SOCKET)
		MessageBox(hWnd, L"CreateSock, socket error", L"clientSock == INVALID_SOCKET", MB_OK);
	//WSAAsyncSelect()로 connect 등록
	if (WSAAsyncSelect(clientSock, hWnd, WM_SOCKET, FD_CONNECT | FD_CLOSE) == SOCKET_ERROR)
		MessageBox(hWnd, L"CreateSock, WSAAsyncSelect() error", L"WSAAsyncSelect()", MB_OK);
	int retval;
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(clientSock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	//MessageBox(hWnd, L"서버 연결중...!", L"서버 연결 중", MB_OK);
	connect_sock = clientSock;
	
}

SOCKET Cserver::getConnectSock()
{
	return connect_sock;
}
// 사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char *buf, int len, int flags)
{
	int received;
	char *ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}
void Cserver::ProcessSocketMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int retval;
	PacketHeader* header;
	char buf[BUFSIZE];
	if (WSAGETSELECTERROR(lParam))
	{
		//err_display(WSAGETSELECTERROR(lParam));
		//MessageBox(hWnd, (LPCWSTR)WSAGETSELECTERROR(lParam), L"WSAGETSELECTERROR(lParam)", MB_OK);
		return;
	}

	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_CONNECT:
		if (WSAAsyncSelect(wParam, hWnd, WM_SOCKET, FD_READ | FD_WRITE | FD_CLOSE) == SOCKET_ERROR)
		{
			MessageBox(hWnd, L"FD_CONNECT, WSAAsyncSelect() Error", L"WSAAsyncSelect()", MB_OK);
		}
		break;
	case FD_READ:
		retval = recv(wParam, (char*)&buf, BUFSIZE, 0);
		if (retval == SOCKET_ERROR)
		{
			MessageBox(hWnd, L"FD_READ, Packet recv() Error", L"Packet recv()", MB_OK);
			return;
		}

		header = (PacketHeader*)buf;
		// 변수 messagebox로 출력
		/*TCHAR text[100];
		wsprintf(text, L"code: %d", header->code);
		MessageBox(GetFocus(), text, L"1", MB_OK);
		wsprintf(text, L"size: %d", header->size);
		MessageBox(GetFocus(), text, L"1", MB_OK);*/
		switch (header->code)
		{
		case TEST:
			MessageBox(hWnd, L"패킷 테스트, 전송완료", L"Packet recv()", MB_OK);
			break;
		case SIGNUP_ACK:
		{
			PACKET_SIGNUP_ACK* signUp_ack = (PACKET_SIGNUP_ACK*)header;
			if (signUp_ack->signupSuccess == TRUE)
			{
				MessageBox(hWnd, L"아이디 생성 완료", L"회원 가입 결과", MB_OK);
			}
			else
			{
				MessageBox(hWnd, L"생성 실패", L"회원 가입 결과", MB_OK);
			}
		}
		break;
		case CHECK_ID_ACK:
		{
			PACKET_CHECKID_ACK* checkId_ack = (PACKET_CHECKID_ACK*)header;
			if (checkId_ack->checkIdSuccess == TRUE)
			{
				MessageBox(hWnd, L"아이디 사용 가능", L"아이디 확인", MB_OK);
				EnableWindow(GetDlgItem(hCreateIdProc, IDOK), TRUE);
				//EnableWindow(GetDlgItem(hCreateIdProc, IDC_SIGNUP_ID_EDIT), FALSE);
			}
			else
			{
				MessageBox(hWnd, L"중복 아이디 존재 불가능", L"아이디 확인", MB_OK);
				EnableWindow(GetDlgItem(hCreateIdProc, IDOK), FALSE);
				//EnableWindow(GetDlgItem(hCreateIdProc, IDC_SIGNUP_ID_EDIT), FALSE);
			}
		}
		break;
		case LOGIN_USER_ACK:
		{
			PACKET_LOGIN_USER_ACK* loginUser_ack = (PACKET_LOGIN_USER_ACK*)header;
			if (loginUser_ack->loginUserSuccess == TRUE)
			{
				WCHAR nickname[10] = L"";
				wcscpy(nickname, loginUser_ack->nickname);
				myCharacterNum = loginUser_ack->characterNum;
				loginWin.DeleteLoginWindow();
				SetTimer(hWnd, 1, 500, NULL);
				waitRoomWin.ShowWaitRoomWindow(hWnd, true);
				SetWindowText(waitRoomWin.GethMyIdWindowEdit(), nickname);
				wcscpy(myNickname, nickname);
				SendNickname_REQ(wParam, nickname);
			}
			else
			{
				MessageBox(hWnd, L"로그인 실패", L"로그인 결과", MB_OK);
			}
		}
		break;
		case SEND_NICKNAME_ACK:
		{
			PACKET_SEND_NICKNAME_ACK* sendNickname_ack = (PACKET_SEND_NICKNAME_ACK*)header;
			SendDlgItemMessage(hWnd, 209, LB_ADDSTRING, 0, (LPARAM)sendNickname_ack->nickname);
		}
		break;
		case SEND_OUT_NICKNAME_ACK:
		{
			PACKET_SEND_OUT_NICKNAME_ACK* sendOtherNickname_ack = (PACKET_SEND_OUT_NICKNAME_ACK*)header;
			int temp = 0;
			temp = SendMessage(waitRoomWin.GethUserWindowListbox(), LB_FINDSTRINGEXACT, 0, (LPARAM)sendOtherNickname_ack->nickname);
			SendMessage(waitRoomWin.GethUserWindowListbox(), LB_DELETESTRING, temp, 0);
		}
		break;
		case ENTER_FIVETREE_GAME_ACK:
		{
			PAKCET_ENTER_FIVETREE_GAME_ACK* enterFivetreeGame_ack = (PAKCET_ENTER_FIVETREE_GAME_ACK*)header;
			KillTimer(hWnd, 1);
			waitRoomWin.ShowWaitRoomWindow(hWnd, false);
			gameWin.ShowGameWindow(hWnd, true);
			if(enterFivetreeGame_ack->homeAway == HOME)
			{
				SetWindowText(gameWin.GethHomeNameEdit(), enterFivetreeGame_ack->nickname);
				homeCharacterNum = enterFivetreeGame_ack->characterNum;
			}
			else
			{
				SetWindowText(gameWin.GethAwayNameEdit(), enterFivetreeGame_ack->nickname);
				awayCharacterNum = enterFivetreeGame_ack->characterNum;	
				InvalidateRect(hWnd, NULL, false);	//전체 맵한번 지워주기
			}
		}
		break;
		case OUT_FIVETREE_GAME_ACK:
		{
			MessageBox(hWnd, L"상대가 나갔습니다.", L"상대방 상태", MB_OK);
			PACKET_OUT_FIVETREE_GAME_ACK* outFivetreeGame_ack = (PACKET_OUT_FIVETREE_GAME_ACK*)header;
			if (outFivetreeGame_ack->homeAway == HOME)
			{
			//	GetWindowText(hLoginIdEdit, login.loginData.loginID, sizeof(login.loginData.loginID));
				SetWindowText(gameWin.GethHomeNameEdit(), myNickname);
				SetWindowText(gameWin.GethAwayNameEdit(), L"");
				homeCharacterNum = awayCharacterNum;
				awayCharacterNum = 0;
			}
			else if (outFivetreeGame_ack->homeAway == AWAY)
			{
				SetWindowText(gameWin.GethAwayNameEdit(), L"");
				awayCharacterNum = 0;
			}
			else
			{
				MessageBox(hWnd, L"OUT_FIVETREE_GAME_ACK home, away 외의 값 입력되었습니다. ", L"home, away 확인", MB_OK);
			}
			gameWin.SetDrawLine(true);
			InvalidateRect(hWnd, NULL, true);	//전체 맵한번 지워주기
		}
		break;
		case START_FIVETREE_GAME_ACK:
		{
			PACKET_START_FIVETREE_GAME_ACK* startFivetreeGame_ack = (PACKET_START_FIVETREE_GAME_ACK*)header;
			gameWin.SetPutStone(startFivetreeGame_ack->putStone);
			gameWin.SetStoneColor(startFivetreeGame_ack->blackWhite);
			gameWin.SetCursorNum(startFivetreeGame_ack->blackWhite);
			gameWin.SetMyStoneColor(startFivetreeGame_ack->blackWhite);
		}
		break;
		case FIVETREE_PUTSTONE_ACK:
		{
			PACKET_FIVETREE_PUTSTONE_ACK* fivetreePutstone_ack = (PACKET_FIVETREE_PUTSTONE_ACK*)header;
			gameWin.SetBoard(fivetreePutstone_ack->x, fivetreePutstone_ack->y, fivetreePutstone_ack->blackWhite);
			gameWin.SetX(fivetreePutstone_ack->x);
			gameWin.SetY(fivetreePutstone_ack->y);
			gameWin.SetPutStone(fivetreePutstone_ack->turn);
			gameWin.SetStoneColor(fivetreePutstone_ack->blackWhite);
			gameWin.SetDrawStone(true);
			InvalidateRect(hWnd, NULL, false);	//전체 맵한번 지워주기
		}
		break;
		case FIVETREE_RESULT_ACK:
		{
			PACKET_FIVETREE_RESULT_ACK* fivetree_result_ack = (PACKET_FIVETREE_RESULT_ACK*)header;
			if (fivetree_result_ack->winLose == true)
			{
				MessageBox(hWnd, L"게임에서 승리하였습니다.", L"게임 결과", MB_OK);
			}
			else
			{
				MessageBox(hWnd, L"게임에서 패배하였습니다.", L"게임 결과", MB_OK);
			}
			gameWin.CreateFivetreeWindow();
			gameWin.SetDrawLine(true);
			InvalidateRect(hWnd, NULL, true);	//전체 맵한번 지워주기
		}
		break;
		case CHAT_ACK:
		{
			PACKET_CHAT_ACK* chat_ack = (PACKET_CHAT_ACK*)header;
			SendMessage(waitRoomWin.GethChatReadWindowListbox(), LB_ADDSTRING, 0, (LPARAM)chat_ack->chat);
		}
		break;
		case ROOM_CHAT_ACK:
		{
			PACKET_ROOM_CHAT_ACK* chat_ack = (PACKET_ROOM_CHAT_ACK*)header;
			SendMessage(gameWin.GethChatListbox(), LB_ADDSTRING, 0, (LPARAM)chat_ack->chat);
		}
		break;
		case WIN_TOTAL_ACK:
		{
			PACKET_WIN_TOTAL_ACK* winTotal_ack = (PACKET_WIN_TOTAL_ACK*)header;
			waitRoomWin.IncreaseWinRanking();
			SendDlgItemMessage(waitRoomWin.GethMyTotalResultDlg(), IDC_LIST1, LB_ADDSTRING, FALSE, (LPARAM)winTotal_ack->nickName);
		}
		break;
		case WIN_MY_TOTAL_ACK:
		{
			PACKET_WIN_MY_TOTAL_ACK* winMyTotal_ack = (PACKET_WIN_MY_TOTAL_ACK*)header;
			SetDlgItemText(waitRoomWin.GethMyTotalResultDlg(), IDC_EDIT1, winMyTotal_ack->nickName);
		}
		break;
		default:
			break;
		}
		break;
	case FD_WRITE:
		//DisplayText(L"FD_WRITE 발생");
		break;
	}
}

