#pragma once
class Event final
{
public:
    Event() = default;
    Event(int id) : m_Id{ id } {}

    int GetId() const { return m_Id; }
private:
    int m_Id{};
};

enum EventType : int {
	PLAYER_DIED,
	PLAYER_DAMAGED,
    SCORE_VALUE_CHANGED
};

