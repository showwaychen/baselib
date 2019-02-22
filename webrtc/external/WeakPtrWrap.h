#pragma once
#include <memory>
#include "Log.h"
template <class T>
class WeakPtrWrap
{
    std::weak_ptr<T> ptrWrap;

  public:
    WeakPtrWrap(std::shared_ptr<T> &sptr)
    {
        ptrWrap = sptr;
    }
    WeakPtrWrap() {}
    WeakPtrWrap<T> operator=(std::shared_ptr<T> &sptr)
    {
        ptrWrap = sptr;
        return *this;
    }
    std::shared_ptr<T> operator->()
    {
        auto tmp = ptrWrap.lock();
        if (tmp == nullptr)
        {
            LOGW << "weak ptr is release";
            return std::shared_ptr<T>(new T());
        }
        return tmp;
    }
    bool IsExpired()
    {
        return ptrWrap.expired();
    }
};