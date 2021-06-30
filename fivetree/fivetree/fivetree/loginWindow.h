#pragma once
class loginWindow
{
private:
	enum EditName {
		ID_LOGIN_SIGNUP_BUTTON = 200,
		ID_LOGIN_SEARCH_PW_BUTTON,
		ID_LOGIN_ID_EDIT,
		ID_LOGIN_PW_EDIT,
		ID_LOGIN_BUTTON
	};
	HBITMAP hBitmap;
	HWND hLoginIdEdit;
	HWND hLoginPwEdit;
	HWND hLoginButton;
	HWND hLoginSignUpButton;
public:
	loginWindow();
	~loginWindow();
	void DrawLoginBackground(HDC hdc);
	void CreateLoginWindow(HWND hWnd, HINSTANCE hInst);
	void DeleteLoginWindow();
	HWND GethLoginIdEdit() { return hLoginIdEdit; }
	HWND GethLoginPwEdit() { return hLoginPwEdit; }
};

