#include "stdafx.h"
#include "sendPacket.h"
#include "protocal.h"
#include "resource.h"
#include <windows.h>


void SendSignUp_REQ(SOCKET sock, HWND hDlg, int characterNum)
{
	int retval;
	PACKET_SIGNUP_REQ signup;
	GetDlgItemText(hDlg, IDC_SIGNUP_ID_EDIT, (LPWSTR)signup.createID, sizeof(signup.createID));
	GetDlgItemText(hDlg, IDC_SIGNUP_PW_EDIT, (LPWSTR)signup.createPW, sizeof(signup.createPW));
	GetDlgItemText(hDlg, IDC_SIGNUP_NICKNAME_EDIT, (LPWSTR)signup.createNickname, sizeof(signup.createNickname));
	signup.characterNum = characterNum;
	
	retval = send(sock, (char*)&signup, sizeof(PACKET_SIGNUP_REQ), 0);
	if (retval == SOCKET_ERROR)
	{
		//err_display(L"SendSignUp_RQ()");
		return;
	}

}

void SendCheckId_REQ(SOCKET sock, HWND hDlg)
{
	int retval;
	PACKET_CHECKID_REQ checkid;
	GetDlgItemText(hDlg, IDC_SIGNUP_ID_EDIT, checkid.loginID, sizeof(checkid.loginID));
	retval = send(sock, (char*)&checkid, sizeof(checkid), 0);
	if (retval == SOCKET_ERROR)
	{
		//err_display(L"SendIdCheck_RQ()");
		return;
	}
}

void SendLoginUser_REQ(SOCKET sock, HWND hLoginIdEdit, HWND hLoginPwEdit)
{
	int retval;
	PACKET_LOGIN_USER_REQ loginUser;
	GetWindowText(hLoginIdEdit, loginUser.loginID, sizeof(loginUser.loginID));
	GetWindowText(hLoginPwEdit, loginUser.loginPW, sizeof(loginUser.loginPW));
	retval = send(sock, (char*)&loginUser, sizeof(loginUser), 0);
	if (retval == SOCKET_ERROR)
	{
		//err_display(L"SendIdCheck_RQ()");
		return;
	}
}

void SendNickname_REQ(SOCKET sock, WCHAR* nickname)
{
	int retval;
	PACKET_SEND_NICKNAME_REQ sendNickname;
	wcscpy(sendNickname.nickname, nickname);
	retval = send(sock, (char*)&sendNickname, sizeof(sendNickname), 0);
	if (retval == SOCKET_ERROR)
	{
		//err_display(L"SendIdCheck_RQ()");
		return;
	}
}

void SendOutNickname_REQ(SOCKET sock, WCHAR * nickname)
{

	int retval;
	PACKET_SEND_OUT_NICKNAME_REQ sendOutNickname;
	wcscpy(sendOutNickname.nickname, nickname);
	retval = send(sock, (char*)&sendOutNickname, sizeof(sendOutNickname), 0);
	if (retval == SOCKET_ERROR)
	{
		//err_display(L"sendOutNickname()");
		return;
	}

}

void SendEnterFivetreeGame_REQ(SOCKET sock)
{
	int retval;
	PAKCET_ENTER_FIVETREE_GAME_REQ enterFivetreeGame;
	retval = send(sock, (char*)&enterFivetreeGame, sizeof(enterFivetreeGame), 0);
	if (retval == SOCKET_ERROR)
	{
		//err_display(L"SendIdCheck_RQ()");
		return;
	}
}

void SendOutFivertreeGame_REQ(SOCKET sock)
{
	int retval;
	PACKET_OUT_FIVETREE_GAME_REQ outFivetreeGame;
	retval = send(sock, (char*)&outFivetreeGame, sizeof(outFivetreeGame), 0);
	if (retval == SOCKET_ERROR)
	{
		//err_display(L"SendIdCheck_RQ()");
		return;
	}
}

void SendStartFivetreeGame_REQ(SOCKET sock)
{
	int retval;
	PACKET_START_FIVETREE_GAME_REQ startFivetreeGame;
	retval = send(sock, (char*)&startFivetreeGame, sizeof(startFivetreeGame), 0);
	if (retval == SOCKET_ERROR)
	{
		//err_display(L"startFivetreeGame()");
		return;
	}
}

void SendPutstone(SOCKET sock)
{
	int retval;
	PACKET_FIVETREE_PUTSTONE_REQ fivetreePutstone;
	fivetreePutstone.x = gameWin.GetX();
	fivetreePutstone.y = gameWin.GetY();
	fivetreePutstone.blackWhite = gameWin.GetStoneColor();
	retval = send(sock, (char*)&fivetreePutstone, sizeof(fivetreePutstone), 0);
	if (retval == SOCKET_ERROR)
	{
		//err_display(L"fivetreePutstone()");
		return;
	}
}



void SendChat_REQ(SOCKET sock, HWND hChatWriteWindowEdit)
{
	int retval;
	PACKET_CHAT_REQ chat;
	GetWindowText(hChatWriteWindowEdit, chat.chat, sizeof(chat.chat));
	retval = send(sock, (char*)&chat, sizeof(chat), 0);
	if (retval == SOCKET_ERROR)
	{
		//err_display(L"SendIdCheck_RQ()");
		return;
	}
}

void SendRoomChat_REQ(SOCKET sock, HWND hChatWriteEdit)
{
	int retval;
	PACKET_ROOM_CHAT_REQ chat;
	GetWindowText(hChatWriteEdit, chat.chat, sizeof(chat.chat));
	retval = send(sock, (char*)&chat, sizeof(chat), 0);
	if (retval == SOCKET_ERROR)
	{
		//err_display(L"SendIdCheck_RQ()");
		return;
	}
}

void SendWinTotal_REQ(SOCKET sock)
{
	int retval;
	PACKET_WIN_TOTAL_REQ winTotal;
	retval = send(sock, (char*)&winTotal, sizeof(winTotal), 0);
	if (retval == SOCKET_ERROR)
	{
		//err_display(L"SendIdCheck_RQ()");
		return;
	}

}

void SendExit_REQ(SOCKET sock)
{
	int retval;
	PACKET_EXIT_REQ exit;
	retval = send(sock, (char*)&exit, sizeof(exit), 0);
	if (retval == SOCKET_ERROR)
	{
		//err_display(L"SendIdCheck_RQ()");
		return;
	}
}
