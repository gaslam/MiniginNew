#pragma once

template<typename T, int capacity>
class CyclicBuffer {
public:
   CyclicBuffer() {}

    void write(T value) {
        const int curIdx{ (m_Index + m_NrItems) % capacity };
        m_Buffer[curIdx] = value;
        if(m_NrItems >= capacity)
        {
                return;
        }
        ++m_NrItems;
    }

    T read(int index) const {
        return m_Buffer[(index + m_NrItems) % capacity];
    }

    T pop() {

        T data = std::move(m_Buffer[m_Index]);
   		++m_Index %= capacity;
        --m_NrItems;
        return data;
    }

    int Size() const { return m_NrItems; }

private:
    T m_Buffer[capacity]{};
    int m_Index = 0;
    int m_NrItems{};
};