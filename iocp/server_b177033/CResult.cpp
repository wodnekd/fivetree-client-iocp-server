#include "stdafx.h"
#include "CResult.h"


CResult::CResult()
{
}


CResult::~CResult()
{
}

LPWSTR CResult::GetOtherUserResult(int count)
{
	WCHAR wtempNickname[20] = L"";
	int len;
	//쿼리문 작성
	sprintf(m_Query, "select *from login3 order by win desc limit 1 offset %d", count);
	//쿼리 요청 및 성공여부 
	m_QueryState = mysql_query(Database.GetConnection(), m_Query);	//db 핸들, 쿼리문
	if (m_QueryState != 0)
	{
		//fprintf(stderr, "mysql query error : %s\n", mysql_error(Database.GetConn));
		return nullptr;
	}
	//결과 확인하기 (결과 확인 후 성공결과 구조체에 넣기)
	Database.SetSqlResult(mysql_store_result(Database.GetConnection()));
	//결과 출력
	while (Database.SetSqlRow(mysql_fetch_row(Database.GetSqlResult())) != NULL)
	{
		char result[50] = "";
		sprintf(result, "%d. %s [승 : %s]",count+1, Database.GetSqlRow()[2], Database.GetSqlRow()[4]);
		printf("%s \n", result);

		len = MultiByteToWideChar(CP_ACP, 0, result, strlen(result), NULL, NULL);
		MultiByteToWideChar(CP_ACP, 0, result, strlen(result), wtempNickname, len);
		//결과 비우기
		mysql_free_result(Database.GetSqlResult());
		return wtempNickname;
	}
	
}

LPWSTR CResult::GetMyResult(LPWSTR nickname)
{
	WCHAR wtempNickname[20] = L"";
	char tempNickname[20] = "";
	int len;
	BOOL m_idCheck;
	len = WideCharToMultiByte(CP_ACP, 0, nickname, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, nickname, -1, tempNickname, len, NULL, NULL);

	m_QueryState = mysql_query(Database.GetConnection(), "select * from login3");
	if (m_QueryState != 0)
	{
		fprintf(stderr, "Mysql query error : %s", mysql_error(&Database.GetConn()));
		return FALSE;
	}
	//로그인 테이블 컬럼을 한꺼번에 가져오기
	Database.SetSqlResult(mysql_store_result(Database.GetConnection()));

	while (Database.SetSqlRow(mysql_fetch_row(Database.GetSqlResult())) != NULL)
	{
		if (strcmp(Database.GetSqlRow()[2], tempNickname) == 0)
		{
			char result[50] = "";
			sprintf(result, " %s [ 승 : %s ]", Database.GetSqlRow()[2], Database.GetSqlRow()[4]);
			printf("%s \n", result);
			len = MultiByteToWideChar(CP_ACP, 0, result, strlen(result), NULL, NULL);
			MultiByteToWideChar(CP_ACP, 0, result, strlen(result), wtempNickname, len);
			//결과 비우기
			mysql_free_result(Database.GetSqlResult());
			return wtempNickname;
		}
	}
	mysql_free_result(Database.GetSqlResult());
	return nullptr;
}

bool CResult::IncreaseWinResult(LPWSTR id)
{
	
	sprintf(m_Query, "update login3 set win = win+1 where id = '%S'", id);
	m_QueryState = mysql_query(Database.GetConnection(), m_Query);
	if (m_QueryState != 0)
	{
		fprintf(stderr, "Mysql query error : %s", mysql_error(&Database.GetConn()));
		return FALSE;
	}
}

int CResult::GetUserNum()
{
	win = 0;
	m_QueryState = mysql_query(Database.GetConnection(), "select * from login3");
	Database.SetSqlResult(mysql_store_result(Database.GetConnection()));

	while (Database.SetSqlRow(mysql_fetch_row(Database.GetSqlResult())) != NULL)
	{
		win++;
	}
	mysql_free_result(Database.GetSqlResult());
	return win;
}
