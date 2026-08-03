#pragma once

#include <ctime>

class CCriticalSection
{
public:
    void Lock() {}
    void Unlock() {}
};

class CTime
{
public:
    CTime() = default;
    explicit CTime(std::time_t value) : m_time(value) {}

    static CTime getCurrentTime()
    {
        return CTime(std::time(nullptr));
    }

    std::time_t getTime() const
    {
        return m_time;
    }

private:
    std::time_t m_time = 0;
};
