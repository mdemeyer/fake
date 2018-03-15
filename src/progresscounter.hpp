#pragma once

#include <shared_mutex>

namespace fake
{

class LockingPolicyShared
{
protected:
    class ReadLock
    {
    public:
        explicit ReadLock(const LockingPolicyShared& parent)
            : m_parent(parent)
        {
            m_parent.m_mutex.lock_shared();
        }

        ~ReadLock()
        {
            m_parent.m_mutex.unlock_shared();
        }
    private:
         const LockingPolicyShared& m_parent;
    };

    class WriteLock
    {
    public:
        explicit WriteLock(const LockingPolicyShared& parent)
            : m_parent(parent)
        {
            m_parent.m_mutex.lock();
        }

        ~WriteLock()
        {
            m_parent.m_mutex.unlock();
        }
    private:
         const LockingPolicyShared& m_parent;
    };

private:
    mutable std::shared_mutex m_mutex;
};

class LockingPolicyNoLock
{
protected:
    struct ReadLock
    {
        explicit constexpr ReadLock(const LockingPolicyNoLock&) noexcept {}
    };

    struct WriteLock
    {
        explicit constexpr WriteLock(const LockingPolicyNoLock&) noexcept {}
    };
};

template<typename LockingPolicy = LockingPolicyShared>
class ProgressCounter : private LockingPolicy
{
    using ReadLock = typename LockingPolicy::ReadLock;
    using WriteLock = typename LockingPolicy::WriteLock;

public:
    constexpr explicit ProgressCounter(unsigned int endValue = 100) noexcept
        : m_endValue(endValue)
        , m_value(0)
        , m_finished(false)
    {}

    constexpr unsigned int get() const noexcept
    {
        ReadLock lock(*this);
        return m_value;
    }

    constexpr bool finished() const noexcept
    {
        ReadLock lock(*this);
        return m_finished;
    }

    constexpr void increment() noexcept
    {
        WriteLock lock(*this);

        if(m_finished) {
            return;
        }

        unsigned int newValue = ++m_value;

        if(newValue == m_endValue) {
            m_finished = true;
        }
    }

private:
    const unsigned int m_endValue;

    unsigned int m_value;
    bool m_finished;
};

constexpr bool start_not_finished() { return ! ProgressCounter<LockingPolicyNoLock>().finished(); }
static_assert (start_not_finished(), "ProgressCounter is not finished on start.");

constexpr bool start_value() { return ProgressCounter<LockingPolicyNoLock>().get() == 0u; }
static_assert (start_value(), "ProgressCounter starts with value 0.");

constexpr bool default_end_value() {
    auto progress = ProgressCounter<LockingPolicyNoLock>();
    while(!progress.finished())
    {
        progress.increment();
    }
    return progress.get() == 100u;
}
static_assert (default_end_value(), "ProgressCounter has 100 as the default end value.");

constexpr bool increment_after_end_value() {
    auto progress = ProgressCounter<LockingPolicyNoLock>(10);
    while(!progress.finished())
    {
        progress.increment();
    }
    progress.increment();
    return progress.get() == 10u;
}
static_assert (increment_after_end_value(), "ProgressCounter is not incremented beyond end value.");

constexpr bool finished_on_end() {
    auto progress = ProgressCounter<LockingPolicyNoLock>(1);
    progress.increment();
    return progress.finished();
}
static_assert (finished_on_end(), "ProgressCounter sets finished when the end value is reached.");

}
