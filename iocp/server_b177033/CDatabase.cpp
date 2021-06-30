#include "stdafx.h"

//DB define
//mysql ��뿡 �ʿ��� ���


CDatabase::CDatabase()
{
}

CDatabase::~CDatabase()
{
	//���� ���� ����
	mysql_close(m_Connection);
}

//mysql ����
BOOL CDatabase::ConnectSql()
{
	//mysql ���� �ʱ�ȭ
	mysql_init(&m_Conn);
	//���� ����
	m_Connection = mysql_real_connect(&m_Conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 3306,(char*)NULL, 0);
	if (m_Connection == NULL)
	{
		fprintf(stderr, "Mysql connection error %d : %s \n",mysql_errno(&m_Conn), mysql_error(&m_Conn));
		return FALSE;
	}
	//���� �ѱ� �ȃ���
	mysql_query(m_Connection, "set names euckr");
	return TRUE;
}

MYSQL_RES* CDatabase::SetSqlResult(MYSQL_RES * mysql_store_result)
{
	m_SqlResult = mysql_store_result; 
	return m_SqlResult;
}

MYSQL_ROW CDatabase::SetSqlRow(MYSQL_ROW mysql_fetch_row)
{
	m_SqlRow = mysql_fetch_row;
	return m_SqlRow;
}
