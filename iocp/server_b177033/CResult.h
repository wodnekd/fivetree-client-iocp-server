#pragma once
class CResult
{
private:
	int			 win;
	char		 m_Query[255];
	int			 m_QueryState;
public:
	CResult();
	~CResult();
	LPWSTR GetOtherUserResult(int count);
	LPWSTR GetMyResult(LPWSTR loginID);
	bool IncreaseWinResult(LPWSTR id);
	int GetUserNum();
};

