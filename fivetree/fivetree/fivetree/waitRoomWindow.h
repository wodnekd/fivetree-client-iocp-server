#pragma once
class waitRoomWindow
{
private:
	enum EditName {
		ID_WAIT_FIVETREE_GAME_START_BUTTON = 205,
		ID_WAIT_TETRIS_GAME_START_BUTTON,
		ID_WAIT_MY_ID_WINDOW_EDIT,
		ID_WAIT_MY_TOTAL_RESULT_BUTTON,
		ID_WAIT_USER_WINDOW_LISTBOX,
		ID_WAIT_CHAT_READ_WINDOW_LISTBOX,
		ID_WAIT_CHAT_WRITE_WINDOW_EDIT,
		ID_WAIT_CHAT_SEND_BUTTON
	};
	int winRanking;
	bool drawWindow;
	HBITMAP hBitmap;
	HBITMAP hBitmap2;
	HWND hFivetreeGameStartButton;
	//HWND hTetrisGameStartButton;
	HWND hMyIdWindowEdit;
	HWND hMyTotalResultButton;
	HWND hUserWindowListbox;
	HWND hChatReadWindowListbox;
	HWND hChatWriteWindowEdit;
	HWND hChatSendButton;
	HWND hMyTotalResultDlg;
public:
	waitRoomWindow();
	~waitRoomWindow();
	void CreateGameStartWindow(HWND hWnd, HINSTANCE hInst);
	void CreateUserWindow(HWND hWnd, HINSTANCE hInst);
	void CreateChatWindow(HWND hWnd, HINSTANCE hInst);

	void DrawFivetreeBmp(HWND hWnd);
	void DrawUserCharacter(HWND hWnd, int characterNum);
	void ShowMyTotalResult(HWND hWnd, HINSTANCE hInst);

	void IncreaseWinRanking() { winRanking++; }
	int  GetWinRanking() { return winRanking; }
	bool GetDrawWindow() { return drawWindow; }
	HWND GethUserWindowListbox() {return hUserWindowListbox;}
	HWND GethChatReadWindowListbox() { return hChatReadWindowListbox; }
	HWND GethChatWriteWindowEdit() { return hChatWriteWindowEdit; }
	HWND GethMyIdWindowEdit() { return hMyIdWindowEdit; }
	HWND GethMyTotalResultDlg() { return hMyTotalResultDlg; }
	void ShowWaitRoomWindow(HWND hWnd, bool TF);
	void DeleteWaitRoomWindow();

	
};

BOOL CALLBACK Dlg_CreateMyTotalResultProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
