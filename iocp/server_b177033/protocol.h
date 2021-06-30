#pragma once

#define USER				16
#define PASSWORD			32
#define CHAT_BUF			256
#define HOME				0
#define AWAY				1
#define black				0
#define white				1
#define arrow				2

enum Protocal
{
	TEST,
	SIGNUP_REQ = 800,
	SIGNUP_ACK,
	CHECK_ID_REQ,
	CHECK_ID_ACK,
	LOGIN_USER_REQ,
	LOGIN_USER_ACK,
	SEND_NICKNAME_REQ,
	SEND_NICKNAME_ACK,
	SEND_OUT_NICKNAME_REQ,
	SEND_OUT_NICKNAME_ACK,
	ENTER_FIVETREE_GAME_REQ,
	ENTER_FIVETREE_GAME_ACK,
	OUT_FIVETREE_GAME_REQ,
	OUT_FIVETREE_GAME_ACK,
	START_FIVETREE_GAME_REQ,
	START_FIVETREE_GAME_ACK,
	FIVETREE_PUTSTONE_REQ,
	FIVETREE_PUTSTONE_ACK,
	FIVETREE_RESULT_REQ,
	FIVETREE_RESULT_ACK,
	CHAT_REQ,
	CHAT_ACK,
	ROOM_CHAT_REQ,
	ROOM_CHAT_ACK,
	WIN_TOTAL_REQ,
	WIN_TOTAL_ACK,
	WIN_MY_TOTAL_REQ,
	WIN_MY_TOTAL_ACK,
	EXIT_REQ,
	EXIT_ACK
};

#pragma pack(1)   
struct PacketHeader
{
	DWORD code;
	DWORD size;
};
//테스트 페킷
struct PACKET_TEST : public PacketHeader
{
	PACKET_TEST()
	{
		code = TEST;
		size = sizeof(*this);
	}
};
//////////////////////////////////////////////
//회원가입 패킷
//////////////////////////////////////////////
struct PACKET_SIGNUP_REQ : public PacketHeader
{
	WCHAR createID[20];
	WCHAR createPW[20];
	WCHAR createNickname[20];
	int characterNum;
	PACKET_SIGNUP_REQ()
	{
		code = SIGNUP_REQ;
		size = sizeof(*this);
		wcscpy(createID, L"");
		wcscpy(createPW, L"");
		wcscpy(createNickname, L"");
		characterNum = 0;
	}
};
struct PACKET_SIGNUP_ACK : public PacketHeader
{
	bool signupSuccess;
	PACKET_SIGNUP_ACK()
	{
		code = SIGNUP_ACK;
		size = sizeof(*this);
		signupSuccess = FALSE;
	}
};
//////////////////////////////////////////////
//회원가입시 아이디 체크 요청 패킷
//////////////////////////////////////////////
struct PACKET_CHECKID_REQ : public PacketHeader
{
	WCHAR loginID[20];
	PACKET_CHECKID_REQ()
	{
		code = CHECK_ID_REQ;
		size = sizeof(*this);
		wcscpy(loginID, L"");
	}
};
struct PACKET_CHECKID_ACK : public PacketHeader
{
	bool checkIdSuccess;
	PACKET_CHECKID_ACK()
	{
		code = CHECK_ID_ACK;
		size = sizeof(*this);
		checkIdSuccess = FALSE;
	}
};
//////////////////////////////////////////////
//로그인 패킷
//////////////////////////////////////////////
struct PACKET_LOGIN_USER_REQ : public PacketHeader
{
	WCHAR loginID[20];
	WCHAR loginPW[20];
	PACKET_LOGIN_USER_REQ()
	{
		code = LOGIN_USER_REQ;
		size = sizeof(*this);
		wcscpy(loginID, L"");
		wcscpy(loginPW, L"");
	}

};
struct PACKET_LOGIN_USER_ACK : public PacketHeader
{
	WCHAR nickname[20];
	int characterNum;
	bool loginUserSuccess;
	PACKET_LOGIN_USER_ACK()
	{
		code = LOGIN_USER_ACK;
		size = sizeof(*this);
		wcscpy(nickname, L"");
		characterNum = 0;
		loginUserSuccess = FALSE;
	}
};
//////////////////////////////////////////////
//닉네임 전송 패킷 
//////////////////////////////////////////////
struct PACKET_SEND_NICKNAME_REQ :public PacketHeader
{
	WCHAR nickname[20];
	PACKET_SEND_NICKNAME_REQ()
	{
		code = SEND_NICKNAME_REQ;
		size = sizeof(*this);
		wcscpy(nickname, L"");
	}
};
struct PACKET_SEND_NICKNAME_ACK : public PacketHeader
{
	WCHAR nickname[20];
	PACKET_SEND_NICKNAME_ACK()
	{
		code = SEND_NICKNAME_ACK;
		size = sizeof(*this);
		wcscpy(nickname, L"");
	}
};
//////////////////////////////////////////////
//다른 유저의 닉네임 전송 패킷
//////////////////////////////////////////////
struct PACKET_SEND_OUT_NICKNAME_REQ :public PacketHeader
{
	WCHAR nickname[20];
	PACKET_SEND_OUT_NICKNAME_REQ()
	{
		code = SEND_OUT_NICKNAME_REQ;
		size = sizeof(*this);
		wcscpy(nickname, L"");
	}
};
struct PACKET_SEND_OUT_NICKNAME_ACK :public PacketHeader
{
	WCHAR nickname[20];
	PACKET_SEND_OUT_NICKNAME_ACK()
	{
		code = SEND_OUT_NICKNAME_ACK;
		size = sizeof(*this);
		wcscpy(nickname, L"");
	}
};
//////////////////////////////////////////////
//게임방 접속 패킷
//////////////////////////////////////////////
struct PAKCET_ENTER_FIVETREE_GAME_REQ : public PacketHeader
{
	PAKCET_ENTER_FIVETREE_GAME_REQ()
	{
		code = ENTER_FIVETREE_GAME_REQ;
		size = sizeof(*this);
	}

};
struct PAKCET_ENTER_FIVETREE_GAME_ACK : public PacketHeader
{
	WCHAR nickname[20];
	int characterNum;
	bool homeAway;
	PAKCET_ENTER_FIVETREE_GAME_ACK()
	{
		code = ENTER_FIVETREE_GAME_ACK;
		size = sizeof(*this);
		wcscpy(nickname, L"");
		characterNum = 0;
		homeAway = 0;
	}
};
//////////////////////////////////////////////
//게임방 나가기 패킷
//////////////////////////////////////////////
struct PACKET_OUT_FIVETREE_GAME_REQ : public PacketHeader
{
	PACKET_OUT_FIVETREE_GAME_REQ()
	{
		code = OUT_FIVETREE_GAME_REQ;
		size = sizeof(*this);
	}
};
struct PACKET_OUT_FIVETREE_GAME_ACK : public PacketHeader
{
	bool homeAway;
	PACKET_OUT_FIVETREE_GAME_ACK()
	{
		code = OUT_FIVETREE_GAME_ACK;
		size = sizeof(*this);
		homeAway = 0;
	}
};
//////////////////////////////////////////////
//게임 시작 패킷
//////////////////////////////////////////////
struct PACKET_START_FIVETREE_GAME_REQ : public PacketHeader
{
	PACKET_START_FIVETREE_GAME_REQ()
	{
		code = START_FIVETREE_GAME_REQ;
		size = sizeof(*this);
	}
};
struct PACKET_START_FIVETREE_GAME_ACK : public PacketHeader
{
	bool putStone;
	bool blackWhite;
	PACKET_START_FIVETREE_GAME_ACK()
	{
		code = START_FIVETREE_GAME_ACK;
		size = sizeof(*this);
		putStone = 0;
		blackWhite = 0;
	}
};
//////////////////////////////////////////////
//바둑돌 놓는 패킷
//////////////////////////////////////////////
struct PACKET_FIVETREE_PUTSTONE_REQ : public PacketHeader
{
	bool blackWhite;
	int x;
	int y;
	PACKET_FIVETREE_PUTSTONE_REQ()
	{
		code = FIVETREE_PUTSTONE_REQ;
		size = sizeof(*this);
		blackWhite = 0;
		x = 0;
		y = 0;
	}
};
struct PACKET_FIVETREE_PUTSTONE_ACK : public PacketHeader
{
	bool turn;
	bool blackWhite;
	int x;
	int y;
	PACKET_FIVETREE_PUTSTONE_ACK()
	{
		code = FIVETREE_PUTSTONE_ACK;
		size = sizeof(*this);
		turn = 0;
		blackWhite = 0;
		x = 0;
		y = 0;
	}

};
//////////////////////////////////////////////
//게임 결과 패킷
//////////////////////////////////////////////
struct PACKET_FIVETREE_RESULT_REQ : public PacketHeader
{
	PACKET_FIVETREE_RESULT_REQ()
	{
		code = FIVETREE_RESULT_REQ;
		size = sizeof(*this);
	}
};

struct PACKET_FIVETREE_RESULT_ACK : public PacketHeader
{
	bool winLose;
	PACKET_FIVETREE_RESULT_ACK()
	{
		code = FIVETREE_RESULT_ACK;
		size = sizeof(*this);
		winLose = 0;
	}
};
//////////////////////////////////////////////
//대기방 채팅 패킷
//////////////////////////////////////////////
struct PACKET_CHAT_REQ : public PacketHeader
{
	WCHAR chat[CHAT_BUF];
	PACKET_CHAT_REQ()
	{
		code = CHAT_REQ;
		size = sizeof(*this);
		wcscpy(chat, L"");
	}
};
struct PACKET_ROOM_CHAT_REQ : public PacketHeader
{
	WCHAR chat[CHAT_BUF];
	PACKET_ROOM_CHAT_REQ()
	{
		code = ROOM_CHAT_REQ;
		size = sizeof(*this);
		wcscpy(chat, L"");
	}
};
//////////////////////////////////////////////
//게임방 채팅 패킷
//////////////////////////////////////////////
struct PACKET_ROOM_CHAT_ACK : public PacketHeader
{
	WCHAR chat[CHAT_BUF];
	PACKET_ROOM_CHAT_ACK()
	{
		code = ROOM_CHAT_ACK;
		size = sizeof(*this);
		wcscpy(chat, L"");
	}
};
struct PACKET_CHAT_ACK : public PacketHeader
{
	WCHAR chat[CHAT_BUF];
	PACKET_CHAT_ACK()
	{
		code = CHAT_ACK;
		size = sizeof(*this);
		wcscpy(chat, L"");
	}
};
//////////////////////////////////////////////
//다른 유저 전적 보는 패킷
//////////////////////////////////////////////
struct PACKET_WIN_TOTAL_REQ : public PacketHeader
{
	PACKET_WIN_TOTAL_REQ()
	{
		code = WIN_TOTAL_REQ;
		size = sizeof(*this);
	}
};

struct PACKET_WIN_TOTAL_ACK : public PacketHeader
{
	WCHAR nickName[20];
	PACKET_WIN_TOTAL_ACK()
	{
		code = WIN_TOTAL_ACK;
		size = sizeof(*this);
		wcscpy(nickName, L"");
	}
};
//////////////////////////////////////////////
//나의 전적 보는 패킷
//////////////////////////////////////////////
struct PACKET_WIN_MY_TOTAL_REQ : public PacketHeader
{
	PACKET_WIN_MY_TOTAL_REQ()
	{
		code = WIN_MY_TOTAL_REQ;
		size = sizeof(*this);
	}
};

struct PACKET_WIN_MY_TOTAL_ACK : public PacketHeader
{
	WCHAR nickName[20];
	PACKET_WIN_MY_TOTAL_ACK()
	{
		code = WIN_MY_TOTAL_ACK;
		size = sizeof(*this);
		wcscpy(nickName, L"");
	}
};
//////////////////////////////////////////////
//클라 종료 패킷
//////////////////////////////////////////////
struct PACKET_EXIT_REQ : public PacketHeader
{
	PACKET_EXIT_REQ()
	{
		code = EXIT_REQ;
		size = sizeof(*this);
	}

};
struct PACKET_EXIT_ACK : public PacketHeader
{
	PACKET_EXIT_ACK()
	{
		code = EXIT_ACK;
		size = sizeof(*this);
	}
};
