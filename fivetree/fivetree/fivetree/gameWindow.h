#pragma once

#define black  0
#define white  1
#define arrow  2

class gameWindow
{
private:
	enum EditName {
		ID_FIVETREE_GAME_START_BUTTON = 213,
		ID_FIVETREE_GAME_OUT_BUTTON,
		ID_FIVETREE_GAME_CHAT_LISTBOX,
		ID_FIVETREE_GAME_CHAT_WRITE_EDIT,
		ID_FIVETREE_GAME_CHAT_SEND_BUTTON,
		ID_FIVETREE_GAME_MY_NAME_EDIT,
		ID_FIVETREE_GAME_OTHER_NAME_EDIT
	};
	HWND hStartButton;
	HWND hOutButton;
	HWND hChatListbox;
	HWND hChatWriteEdit;
	HWND hChatSendButton;

	HWND hHomeNameEdit;
	HWND hAwayNameEdit;

	HCURSOR blackCursor;
	HCURSOR whiteCursor;
	HCURSOR arrowCursor;

	HBITMAP blackBitmap;
	HBITMAP whiteBitmap;

	HBITMAP hBitmap;
	int x;
	int y;
	int cursorNum;

	int board[10][10];

	bool clickButton;

	bool putStone;
	bool myStoneColor;
	bool stoneColor;
	bool changeCursor;
	
	bool drawLine;
	bool drawCharacter;
	bool drawSton;
	bool drawWindow;
public:
	gameWindow();
	~gameWindow();
	void Init(HWND hWnd, HINSTANCE hInst);
	void CreateFivetreeWindow();
	void CreateChatWindow(HWND hWnd, HINSTANCE hInst);
	void DrawFivetreeLine(HDC hdc);
	void CursorSet();
	bool Putstone(HWND hWnd, LPARAM lParam);
	//void Putstone(HWND hWnd, LPARAM lParam, bool pullStone);
	void DrawStone(HDC hdc);
	void DrawUserCharacter(HWND hWnd, int firstPlayer, int SecondPlayer);
	void DeleteBitmap();
	void ShowGameWindow(HWND hWNd, bool TF);
	void DeletGameWindow();

	int (*GetBoard())[10] { return board; }
	void SetBoard(int x, int y, bool blackWhite) { board[x / 50][y / 50] = blackWhite; }
	int GetX() { return x; }
	int GetY() { return y; }
	void SetX(int x) { this->x = x; }
	void SetY(int y) { this->y = y; }

	void  SetCursorNum(int num) { cursorNum = num; }
	int  GetCursorNum() { return cursorNum; }

	void SetPutStone(bool TF) { putStone = TF; }
	bool GetPurStone() { return putStone; }
	
	void SetStoneColor(bool TF) { stoneColor = TF; }
	bool GetStoneColor() { return stoneColor; }
	void SetMyStoneColor(bool TF) { myStoneColor = TF; }
	bool GetMyStoneColor() { return myStoneColor; }

	void SetDrawStone(bool TF) { drawSton = TF; }

	void SetDrawLine(bool TF) { drawLine = TF; }
	bool GetDrawWindow() { return drawWindow; }
	HWND GethHomeNameEdit() { return hHomeNameEdit; }
	HWND GethAwayNameEdit() { return hAwayNameEdit; }
	HWND GethChatListbox() { return hChatListbox; }
	HWND GethChatWriteEdit() { return hChatWriteEdit; }
	
};

