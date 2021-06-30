#pragma once


void SendSignUp_REQ(SOCKET sock, HWND hDlg, int characterNum);
void SendCheckId_REQ(SOCKET sock, HWND hDlg);
void SendLoginUser_REQ(SOCKET sock, HWND hLoginIdEdit, HWND hLoginPwEdit);
void SendNickname_REQ(SOCKET sock, WCHAR* nickname);
void SendOutNickname_REQ(SOCKET sock, WCHAR* nickname);
void SendEnterFivetreeGame_REQ(SOCKET sock);
void SendOutFivertreeGame_REQ(SOCKET sock);
void SendStartFivetreeGame_REQ(SOCKET sock);
void SendPutstone(SOCKET sock);

void SendChat_REQ(SOCKET sock, HWND hChatWriteWindowEdit);
void SendRoomChat_REQ(SOCKET sock, HWND hChatWriteEdit);

void SendWinTotal_REQ(SOCKET sock);

void SendExit_REQ(SOCKET sock);



//void SendSignUp_REQ(SOCKET sock, HWND hDlg, int characterNum);
//void SendCheckId_REQ(SOCKET sock, HWND hDlg);
//void SendLoginUser_REQ(SOCKET sock, HWND hLoginIdEdit, HWND hLoginPwEdit);
//void SendNickname_REQ(SOCKET sock, WCHAR* nickname);
//void SendEnterFivetreeGame_REQ(SOCKET sock);
//void SendOutFivertreeGame_REQ(SOCKET sock, int roomNumber);
//void SendStartFivetreeGame_REQ(SOCKET sock, int roomNumber);
//void SendPutstone(SOCKET sock, int roomNumber);
//
//void SendChat_REQ(SOCKET sock, HWND hChatWriteWindowEdit);
//void SendRoomChat_REQ(SOCKET sock, HWND hChatWriteEdit, int roomNumber);
//void SendExit_REQ(SOCKET sock);