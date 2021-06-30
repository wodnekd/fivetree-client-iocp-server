#include "stdafx.h"

CUserSession::CUserSession()
{
	packetMap.insert(packetList::value_type(TEST, &CUserSession::Test_PACKET));
	packetMap.insert(packetList::value_type(SIGNUP_REQ, &CUserSession::SignUp_REQ));
	packetMap.insert(packetList::value_type(SIGNUP_ACK, &CUserSession::SignUp_ACK));
	packetMap.insert(packetList::value_type(CHECK_ID_REQ, &CUserSession::CheckID_REQ));
	packetMap.insert(packetList::value_type(CHECK_ID_ACK, &CUserSession::CheckID_ACK));
	packetMap.insert(packetList::value_type(LOGIN_USER_REQ, &CUserSession::LoginUser_REQ));
	packetMap.insert(packetList::value_type(LOGIN_USER_ACK, &CUserSession::LoginUser_ACK));
	packetMap.insert(packetList::value_type(SEND_NICKNAME_REQ, &CUserSession::SendNickname_REQ));
	packetMap.insert(packetList::value_type(SEND_NICKNAME_ACK, &CUserSession::SendNickname_ACK));

	packetMap.insert(packetList::value_type(SEND_OUT_NICKNAME_REQ, &CUserSession::SendOutNickname_REQ));
	packetMap.insert(packetList::value_type(SEND_OUT_NICKNAME_ACK, &CUserSession::SendOutNickname_ACK));

	packetMap.insert(packetList::value_type(ENTER_FIVETREE_GAME_REQ, &CUserSession::EnterFivetreeGame_REQ));
	packetMap.insert(packetList::value_type(ENTER_FIVETREE_GAME_ACK, &CUserSession::EnterFivetreeGame_ACK));
	packetMap.insert(packetList::value_type(OUT_FIVETREE_GAME_REQ, &CUserSession::OutFivetreeGame_REQ));
	packetMap.insert(packetList::value_type(OUT_FIVETREE_GAME_ACK, &CUserSession::OutFivetreeGame_ACK));
	packetMap.insert(packetList::value_type(START_FIVETREE_GAME_REQ, &CUserSession::StartFivetreeGame_REQ));
	packetMap.insert(packetList::value_type(START_FIVETREE_GAME_ACK, &CUserSession::StartFivetreeGame_ACK));
	packetMap.insert(packetList::value_type(FIVETREE_PUTSTONE_REQ, &CUserSession::FivetreePutstone_REQ));
	packetMap.insert(packetList::value_type(FIVETREE_PUTSTONE_ACK, &CUserSession::FivetreePutstone_ACK));

	packetMap.insert(packetList::value_type(CHAT_REQ, &CUserSession::Chat_REQ));
	packetMap.insert(packetList::value_type(CHAT_ACK, &CUserSession::Chat_ACK));
	packetMap.insert(packetList::value_type(ROOM_CHAT_REQ, &CUserSession::RoomChat_REQ));
	packetMap.insert(packetList::value_type(ROOM_CHAT_ACK, &CUserSession::RoomChat_ACK));

	packetMap.insert(packetList::value_type(WIN_TOTAL_REQ, &CUserSession::WinTotal_REQ));
	packetMap.insert(packetList::value_type(WIN_TOTAL_ACK, &CUserSession::WinTotal_ACK));

	packetMap.insert(packetList::value_type(EXIT_REQ, &CUserSession::Exit_REQ));
	packetMap.insert(packetList::value_type(EXIT_ACK, &CUserSession::Exit_ACK));
	
	state = LOGIN;
	memset(userID, 0, sizeof(userID));

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			board[i][j] = 3;
		}
	}
}

CUserSession::~CUserSession()
{
	packetMap.clear();
	SESSION.GetRoomManager()->GetRoomList()[0]->LeaveUser(this);
}
//패킷 파싱

VOID CUserSession::PacketParsing(PacketHeader *pPacket)
{
	if (pPacket == NULL)
	{
		cout << "In Not Packet" << endl;
		return;
	}
	//패킷맵에서 현재 들어온 패킷의 코드를 찾아 iter에 넣어준다
	iter = packetMap.find(pPacket->code);
	//함수 포인터를 이용하여 패킷함수 실행
	if (iter != packetMap.end())
	{
		pF Fp = iter->second; 
		(this->*Fp)(pPacket);
	}
}

VOID CUserSession::Test_PACKET(PacketHeader * pPacket)
{
	cout << "aaa" << endl;
	PACKET_TEST *test = new PACKET_TEST;
	Send((char*)test, test->size);
}

VOID CUserSession::SignUp_REQ(PacketHeader *pPacket)
{
	//파싱된 패킷을 저장
	PACKET_SIGNUP_REQ *packet = (PACKET_SIGNUP_REQ*)pPacket;
	//보낼 패킷 생성
	PACKET_SIGNUP_ACK send_packet;
	state = LOGIN;
	if (!Login.CreateUserID(packet->createID, packet->createPW, packet->createNickname, packet->characterNum))
	{
		send_packet.signupSuccess = false;
		cout << "아이디 생성 실패" << endl;
		CCLog::WriteLog(_T("LoginFaill"));
	}
	else
	{
		send_packet.signupSuccess = true;
		printf("아이디 : [ %S ] 닉네임 : [ %S ] \n", packet->createID, packet->createNickname);
		cout << endl;
	}
	//패킷 보내기
	Send((char*)&send_packet, send_packet.size);
}
VOID CUserSession::SignUp_ACK(PacketHeader * pPacket)
{
	cout << "NewUer_ACK" << endl;
}


VOID CUserSession::CheckID_REQ(PacketHeader * pPacket)
{
	PACKET_CHECKID_REQ *recv_packet = (PACKET_CHECKID_REQ*)pPacket;
	PACKET_CHECKID_ACK send_packet;
	state = LOGIN;
	if (Login.CompareUserID(recv_packet->loginID))
	{
		send_packet.checkIdSuccess = false;
		cout << "중복 된 아이디로 생성 실패" << endl;
	}
	else
	{
		send_packet.checkIdSuccess = true;
		cout << "아이디 생성 가능" << endl;
	}
	Send((char*)&send_packet, send_packet.size);
}

VOID CUserSession::CheckID_ACK(PacketHeader * pPacket)
{
	cout << "NewUer_ACK" << endl;
}

//로그인 처리 패킷
VOID CUserSession::LoginUser_REQ(PacketHeader * pPacket)
{
	PACKET_LOGIN_USER_REQ *recv_packet = (PACKET_LOGIN_USER_REQ*)pPacket;
	PACKET_LOGIN_USER_ACK send_packet; 
	state = LOGIN;
	//로그인 실패 시 처리
	if (!Login.LoginUser(recv_packet->loginID, recv_packet->loginPW))
	{
		send_packet.loginUserSuccess = false;
		cout << "로그인 실패" << endl;
	}
	//로그인 성공 시 처리
	else
	{
		//아이디, 닉네임 정보 저장
		wcscpy(this->userID, recv_packet->loginID);
		wcscpy(this->nickname, Login.GetNickname(recv_packet->loginID));
		this->characterNum = Login.GetCharacterNum(recv_packet->loginID);
		state = LOBBY;
		//UserManager에 유저 정보 저장
		SESSION.GetUserManager()->Enter_User(this);
		//방만들기 첫 접속자가 방 만들기
		if(SESSION.GetRoomManager()->GetRoomCount() == 0)
		{
			SESSION.GetRoomManager()->CreateRoom();
		}
		//대기 방 입장 (대기실 방 인덱스 0번)
		SESSION.GetRoomManager()->GetRoomList()[0]->JoinUser(this);
		gameRoomNumber = 0;
		//접속한 유저 수, 대기실 유저 수 확인
		cout << "유저 수 : " << SESSION.GetUserManager()->GetClientNum() << endl;
		cout << "대기실 유저 수 : " << SESSION.GetRoomManager()->GetRoomList()[0]->GetUserNum() << endl;
		//send 패킷 정보 저장 (닉네임, 케릭터 번호, 로그인 성공)
		wcscpy(send_packet.nickname, this->nickname);
		send_packet.characterNum = Login.GetCharacterNum(recv_packet->loginID);
		send_packet.loginUserSuccess = true;
		cout << "로그인 성공" << endl;
	}
	//패킷 정송
	Send((char*)&send_packet, send_packet.size);
}

VOID CUserSession::LoginUser_ACK(PacketHeader * pPacket)
{
	cout << "LoginUser_ACK" << endl;
}

//다른 유저들에게 접속한 유저의 닉네임을 보내주는 패킷
VOID CUserSession::SendNickname_REQ(PacketHeader * pPacket)
{
	PACKET_SEND_NICKNAME_REQ *recv_packet = (PACKET_SEND_NICKNAME_REQ*)pPacket;
	PACKET_SEND_NICKNAME_ACK send_packet;
	wcscpy(send_packet.nickname, recv_packet->nickname);
	
	//현재 접속한 유저 닉네임을 접속한 다른 유저들에게 보내준다.
	//1.
	//SESSION.GetRoomManager()->GetRoomList()[this->RoomNumber]->Broadcast_User(this, (void *)&send_packet, send_packet.size, false);
	SESSION.GetRoomManager()->GetRoomList()[0]->Broadcast_User(this, (void *)&send_packet, send_packet.size, false);

	map<SOCKET, CUserSession*> m = SESSION.GetUserManager()->GetOnList();
	map<SOCKET, CUserSession*>::iterator iter;
	//다른 유저들의 닉네임을 현재 접속한 유저에게 보내준다.
	wcscpy(send_packet.nickname, L"");
	for (iter = m.begin(); iter != m.end(); iter++)
	{
		if (this != iter->second)
		{
			wcscpy(send_packet.nickname, iter->second->nickname);
			Send((char*)&send_packet, send_packet.size);
		}
		else
		{
			continue;
		}
	}
}

VOID CUserSession::SendNickname_ACK(PacketHeader * pPacket)
{
	cout << "SendID_ACK" << endl;
}
//1.
VOID CUserSession::SendOutNickname_REQ(PacketHeader * pPacket)
{
	PACKET_SEND_OUT_NICKNAME_REQ *recv_packet = (PACKET_SEND_OUT_NICKNAME_REQ*)pPacket;
	PACKET_SEND_OUT_NICKNAME_ACK send_packet;
	wcscpy(send_packet.nickname, recv_packet->nickname);

	SESSION.GetRoomManager()->GetRoomList()[0]->Broadcast_User(this, (void *)&send_packet, send_packet.size, false);
}
//1.
VOID CUserSession::SendOutNickname_ACK(PacketHeader * pPacket)
{
	cout << "SendOtherID_ACK" << endl;
}

//게임방 입장 패킷
VOID CUserSession::EnterFivetreeGame_REQ(PacketHeader * pPacket)
{
	PAKCET_ENTER_FIVETREE_GAME_ACK send_packet;
	//방 전체 확인 1부터
	for (int roomNum = 1; roomNum < MAX_ROOMCOUNT; roomNum++)
	{
		//1번방부터 접속 유저 수 확인
		int userNum = userNum = SESSION.GetRoomManager()->GetRoomList()[roomNum]->GetUserNum();
		//유저수 0명이면 현재 userNum에 입장
		if (userNum == 0)
		{
			//게임방 번호 세션에 저장
			gameRoomNumber = roomNum;
			homeAway = HOME;
			//현재 roomNum 게임방에 유저 입장
			SESSION.GetRoomManager()->GetRoomList()[roomNum]->JoinUser(this);
			//보낼 패킷 정보 저장 (현재 접속한 유저의 닉네임, 케릭터 번호, home, 방 번호)
			wcscpy(send_packet.nickname, this->nickname);
			send_packet.characterNum = this->characterNum;
			send_packet.homeAway = HOME;
			//확인
			cout <<"userNum == 0 // GameRoomNumber : "<< gameRoomNumber << endl;
			//보내기
			Send((char*)&send_packet, send_packet.size);
			return;
		}
		//접속한 유저가 1명 있다면 (2인방이므로 입장 가능)
		else if (userNum == 1)
		{
			//게임방 번호 세션에 저장
			gameRoomNumber = roomNum;
			homeAway = AWAY;
			//roomNum에 입장
			SESSION.GetRoomManager()->GetRoomList()[roomNum]->JoinUser(this);
			//접속해 있던 유저의 세션 정보받아오기 (접속해 있던 유저의 정보를 this유저에게 보내기 위해)
			CUserSession* twoRoomOtherUser = SESSION.GetRoomManager()->GetRoomList()[roomNum]->GetTwoRoomOtherUser(this);
			//1.1.		
			//접속해 있던 유저 정보 저장 후 this 유저에게 전송
			PAKCET_ENTER_FIVETREE_GAME_ACK send_packet;
			wcscpy(send_packet.nickname, twoRoomOtherUser->nickname);
			send_packet.characterNum = twoRoomOtherUser->characterNum;
			send_packet.size = sizeof(send_packet);
			send_packet.homeAway = HOME;
			//접속해 있던 유저 정보 전송
			Send((char*)&send_packet, send_packet.size);

			//this 유저의 정보 패킷에 저장 (현재 접속한 유저의 닉네임, 케릭터 번호, away, 방벙호)
			wcscpy(send_packet.nickname, this->nickname);
			send_packet.characterNum = this->characterNum;
			send_packet.homeAway = AWAY;
			//확인
			cout << "userNum == 1 // gameRoomNumber : " << gameRoomNumber << endl;
			//1.
			SESSION.GetRoomManager()->GetRoomList()[roomNum]->Broadcast_User(this, (void *)&send_packet, send_packet.size, true);
			
			return;
		}
		//접속한 유저가 2명 있다면 방이 꽉찬 것이므로 다음 방을 확인
		else
		{
			continue;
		}
	}
}

VOID CUserSession::EnterFivetreeGame_ACK(PacketHeader * pPacket)
{
	cout << "EnterFivetreeGame_ACK" << endl;
}
VOID CUserSession::OutFivetreeGame_REQ(PacketHeader * pPacket)
{
	//PACKET_OUT_FIVETREE_GAME_REQ *recv_packet = (PACKET_OUT_FIVETREE_GAME_REQ*)pPacket;
	PACKET_OUT_FIVETREE_GAME_ACK send_packet;
	send_packet.homeAway = homeAway;
	cout << "나간 homeAway : " << send_packet.homeAway << endl;
	cout << "OutFivetreeGame_REQ 나갈 방 번호 : " << gameRoomNumber <<endl;

	SESSION.GetRoomManager()->GetRoomList()[gameRoomNumber]->LeaveUser(this, &send_packet, send_packet.size);
	//세션에 저장된 게임방 번호 초기화
	gameRoomNumber = 0;
}

VOID CUserSession::OutFivetreeGame_ACK(PacketHeader * pPacket)
{
	cout << "OutFivetreeGame_ACK" << endl;
}

VOID CUserSession::StartFivetreeGame_REQ(PacketHeader * pPacket)
{
	//PACKET_START_FIVETREE_GAME_REQ* recv_packet = (PACKET_START_FIVETREE_GAME_REQ*)pPacket;
	PACKET_START_FIVETREE_GAME_ACK send_packet;

	send_packet.putStone = true;
	send_packet.blackWhite = black;
	Send((char*)&send_packet, send_packet.size);
	
	send_packet.putStone = false;
	send_packet.blackWhite = white;

	SESSION.GetRoomManager()->GetRoomList()[gameRoomNumber]->Broadcast_User(this, (void *)&send_packet, send_packet.size, false);

}

VOID CUserSession::StartFivetreeGame_ACK(PacketHeader * pPacket)
{
	cout << "StartFivetreeGame_ACK" << endl;
}

VOID CUserSession::FivetreePutstone_REQ(PacketHeader * pPacket)
{
	PACKET_FIVETREE_PUTSTONE_REQ* recv_packet = (PACKET_FIVETREE_PUTSTONE_REQ*)pPacket;
	PACKET_FIVETREE_PUTSTONE_ACK send_packet;
	//보드에 바둑돌 놓은 위치 입력
	//1.
	board[recv_packet->x / 50][recv_packet->y / 50] = recv_packet->blackWhite;
	
	//상대방 유저에게 보내기
	send_packet.turn = true;
	send_packet.x = recv_packet->x;
	send_packet.y = recv_packet->y;
	send_packet.blackWhite = recv_packet->blackWhite;
	SESSION.GetRoomManager()->GetRoomList()[gameRoomNumber]->Broadcast_User(this, (void *)&send_packet, send_packet.size, false);

	//승리 판정
	if (CheckWin(recv_packet->blackWhite) == true)
	{
		if (recv_packet->blackWhite == 0)
		{
			cout << "검은돌 승리" << endl;
		}
		else
		{
			cout << "하얀돌 승리" << endl;
		}
		//데베에 승리 추가
		Result.IncreaseWinResult(userID);
		InitBoard();
		PACKET_FIVETREE_RESULT_ACK send_packet;
		send_packet.winLose = true;
		Send((char*)&send_packet, send_packet.size);
		send_packet.winLose = false;
		SESSION.GetRoomManager()->GetRoomList()[gameRoomNumber]->Broadcast_User(this, (void *)&send_packet, send_packet.size, false);
	}
}

void CUserSession::FivetreePutstone_ACK(PacketHeader * pPacket)
{
	cout << "FivetreePutstone_ACK" << endl;
}



VOID CUserSession::Chat_REQ(PacketHeader * pPacket)
{
	PACKET_CHAT_REQ *recv_packet = (PACKET_CHAT_REQ*)pPacket;
	PACKET_CHAT_ACK send_packet;
	printf("대기실에서 받은 메시지 내용 : %S\n", recv_packet->chat);
	cout << "대기방 코드 : " << recv_packet->code << endl;
	swprintf(send_packet.chat, 20, L"%s : %s", nickname, recv_packet->chat);
	SESSION.GetRoomManager()->GetRoomList()[0]->Broadcast_User(this, (void *)&send_packet, send_packet.size, true);
}

VOID CUserSession::Chat_ACK(PacketHeader * pPacket)
{
	cout << "Chat_ACK" << endl;
}

VOID CUserSession::RoomChat_REQ(PacketHeader * pPacket)
{
	PACKET_ROOM_CHAT_REQ *recv_packet = (PACKET_ROOM_CHAT_REQ*)pPacket;
	PACKET_ROOM_CHAT_ACK send_packet;
	cout << "게임방 번호 : "<< gameRoomNumber << endl;
	cout << "게임방 코드 : " << recv_packet->code << endl;
	printf("방에서 받은 메시지 내용 : %S\n", recv_packet->chat);
	swprintf(send_packet.chat, 20, L"%s : %s", nickname, recv_packet->chat);
	SESSION.GetRoomManager()->GetRoomList()[gameRoomNumber]->Broadcast_User(this, (void *)&send_packet, send_packet.size, true);
}

VOID CUserSession::RoomChat_ACK(PacketHeader * pPacket)
{
	cout << "Chat_ACK" << endl;
}

VOID CUserSession::WinTotal_REQ(PacketHeader * pPacket)
{
	PACKET_WIN_TOTAL_REQ *recv_packet = (PACKET_WIN_TOTAL_REQ*)pPacket;
	PACKET_WIN_TOTAL_ACK send_packet;
	int num = Result.GetUserNum();
	for (int i = 0; i < num; i++)
	{
		Sleep(10);
		wcscpy(send_packet.nickName, Result.GetOtherUserResult(i));
		Send((char*)&send_packet, send_packet.size);
	}
	PACKET_WIN_MY_TOTAL_ACK send_packet2;
	wcscpy(send_packet2.nickName, Result.GetMyResult(nickname));
	Send((char*)&send_packet2, send_packet.size);
}

VOID CUserSession::WinTotal_ACK(PacketHeader * pPacket)
{
	return VOID();
}





VOID CUserSession::Exit_REQ(PacketHeader* pPacket)
{
	SESSION.GetRoomManager()->GetRoomList()[0]->LeaveUser(this);
	SESSION.GetRoomManager()->GetRoomList()[gameRoomNumber]->LeaveUser(this);
	SESSION.GetUserManager()->Leave_User(this);
	cout << "대기방 유저 수 : " << SESSION.GetRoomManager()->GetRoomList()[0]->GetUserNum() << endl;
	cout << "대기실 유저 수 : " << SESSION.GetUserManager()->GetClientNum() << endl;
	
}

VOID CUserSession::Exit_ACK(PacketHeader * pPacket)
{
	cout << "Exit_ACK" << endl;
}

void CUserSession::InitBoard()
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			board[i][j] = 3;
		}
	}
}

bool CUserSession::CheckWin(bool blackWhite)
{
	for (int i = 0; i < 6; i++)
		for (int j = 0; j < 6; j++)
		{
			if (board[i][j] == blackWhite && board[i][j + 1] == blackWhite && board[i][j + 2] == blackWhite && board[i][j + 3] == blackWhite && board[i][j + 4] == blackWhite)
			{
				return true;
			}
			else if (board[i][j] == blackWhite && board[i + 1][j] == blackWhite && board[i + 2][j] == blackWhite && board[i + 3][j] == blackWhite && board[i + 4][j] == blackWhite)
			{
				return true;
			}
			else if (board[i][j] == blackWhite && board[i + 1][j + 1] == blackWhite && board[i + 2][j + 2] == blackWhite && board[i + 3][j + 3] == blackWhite && board[i + 4][j + 4] == blackWhite)
			{
				return true;
			}
			else if (board[i][j] == blackWhite && board[i - 1][j + 1] == blackWhite && board[i - 2][j + 2] == blackWhite && board[i - 3][j + 3] == blackWhite && board[i - 4][j + 4] == blackWhite)
			{
				return true;
			}
		}
	return false;
}
