#pragma once
enum {
	JOIN, LEAVE, EXAM
};
#define MAX_USER 4

class CRoom : public CMultiThreadSync <CRoom>
{
private:
	vector<CUserSession*>::iterator viter;
	std::map<SOCKET, CUserSession*> useMapList;
	std::map<SOCKET, CUserSession*>::iterator iterMap;
	int		room_number;
	int		user_num;
	bool	m_createRoom;
	bool	m_full;
	bool	game_state;
	bool	check;
public:
	CRoom() {}
	CRoom(bool createRoom = true, bool full = false);
	~CRoom();
	VOID Update(CUserSession *user, void* pPacket, DWORD number, bool correct);
	VOID Set(bool check)				{ this->check = check; }
	int  GetUserNum()					{ return user_num; }
	bool IsCreate()						{ return m_createRoom; }
	bool IsFuLL()						{ return m_full; }
	bool JoinUser(CUserSession* user);
	bool LeaveUser(CUserSession* user);
	bool LeaveUser(CUserSession* user, void* pPacket, DWORD size);
	bool Broadcast_User(CUserSession* user, void* pPacket, DWORD size, bool all_user);
	//bool SendOhterUser(CUserSession* user);
	//bool SendOhterUser(CUserSession * user, void* pPacket, DWORD size);
	bool IsRoomUser(CUserSession *user);
	CUserSession* GetTwoRoomOtherUser(CUserSession *user);
	std::map<SOCKET, CUserSession*>  GetuseMapList() { return useMapList; }
	
};

