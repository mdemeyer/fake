#pragma once

#include <shared_mutex>

namespace fake
{

class ProgressCounter
{
public:
    constexpr explicit ProgressCounter(unsigned int endValue = 100) noexcept
        : m_endValue(endValue)
        , m_value(0)
        , m_finished(false)
    {}

    unsigned int get() const
    {
        std::shared_lock<std::shared_mutex> lock(m_mutex);
        return m_value;
    }

    bool finished() const
    {
        std::shared_lock<std::shared_mutex> lock(m_mutex);
        return m_finished;
    }

    void increment()
    {
        std::unique_lock<std::shared_mutex> lock(m_mutex);
        unsigned int newValue = ++m_value;

        if(newValue == m_endValue) {
            m_finished = true;
        }
    }

private:
    mutable std::shared_mutex m_mutex;

    const unsigned int m_endValue;

    unsigned int m_value;
    bool m_finished;
};

}
