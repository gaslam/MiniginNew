#pragma once
#include <string>
class Event final
{
public:
	Event(int id) : m_Id{ id } {};

	int GetId() const { return m_Id; }
private:
	const int m_Id;
};

enum EventType : int {
	PLAYER_DIED,
	PLAYER_DAMAGED
};

