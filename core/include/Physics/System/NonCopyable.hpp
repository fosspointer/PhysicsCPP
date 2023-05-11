#pragma once

namespace physics
{
    /**
     * @brief NonCopyable class, to be overriden by classes which need to be non-copyable (such as singletons)
     */
    class NonCopyable
    {
    public:
        NonCopyable() = default;
        NonCopyable(const NonCopyable&) = delete;
        NonCopyable& operator=(const NonCopyable&) = delete;
    };
}